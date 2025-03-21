/* main.c - Application main entry point */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <errno.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include <zephyr/drivers/gpio.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/bluetooth/gap.h>

#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
bool flag = 0;

uint8_t adres = 0;

uint32_t msg[] = {0, 0, 0};

static void start_scan(void);

static struct bt_conn *default_conn;

// Funkcja do wyświetlania danych reklamowych

static void ad_data(struct net_buf_simple *ad)
{
    while (ad->len > 1) {
        uint8_t len = net_buf_simple_pull_u8(ad);
        uint8_t type;

        if (len == 0) {
            return;
        }

        type = net_buf_simple_pull_u8(ad);
        len--;

        switch (type) {
        case BT_DATA_FLAGS:
            adres = net_buf_simple_pull_u8(ad);
            len--;
            break;

        case BT_DATA_MANUFACTURER_DATA:
            if (adres == 100) {
                if (len >= sizeof(msg)) {
                    uint32_t received_msg[sizeof(msg) / sizeof(uint32_t)];
                    memcpy(received_msg, ad->data, sizeof(received_msg));

					printk("ID czujnika: %u \n", received_msg[0]);
					printk("Natezenie swiatla: %u \n", received_msg[1]);
					
					if(received_msg[2] == 0){
						printk("Wykryto ruch !\n");
					}	else{
						printk("Brak obecnosci !\n");
					}
				    /*
					printk("Wartosc: ");
                    for (size_t i = 0; i < sizeof(received_msg) / sizeof(uint32_t); i++) {
                        printk("%u ", received_msg[i]);
                    }
                    printk("\n");
					*/

                    net_buf_simple_pull(ad, sizeof(received_msg));
                    len -= sizeof(received_msg);
                } else {
                    printk("Insufficient length\n");
                    net_buf_simple_pull(ad, len);
                    len = 0;
                }
            }
            break;

        default:
            net_buf_simple_pull(ad, len);
            len = 0;
            break;
        }
    }
}

static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
			 struct net_buf_simple *ad)
{
	char addr_str[BT_ADDR_LE_STR_LEN];
	int err;

	if (default_conn) {
		return;
	}

	/* We're only interested in connectable events */
	if (type != BT_GAP_ADV_TYPE_ADV_IND &&
	    type != BT_GAP_ADV_TYPE_ADV_DIRECT_IND) {
		return;
	}

	bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
	//printk("Device found: %s (RSSI %d)\n", addr_str, rssi);

	ad_data(ad);

	/* connect only to devices in close proximity */
	if (rssi < -50) {
		return;
	}

	if (bt_le_scan_stop()) {
		return;
	}

	if(adres == 100){
	err = bt_conn_le_create(addr, BT_CONN_LE_CREATE_CONN,
				BT_LE_CONN_PARAM_DEFAULT, &default_conn);
		if (err) {
			printk("Create conn to %s failed (%d)\n", addr_str, err);
			start_scan();
		}
	}	else start_scan();
}

static void start_scan(void)
{
	int err;

	/* This demo doesn't require active scan */
	err = bt_le_scan_start(BT_LE_SCAN_PASSIVE, device_found);
	if (err) {
		printk("Scanning failed to start (err %d)\n", err);
		return;
	}

	printk("Scanning successfully started\n");
}

static void connected(struct bt_conn *conn, uint8_t err)
{	
	flag = 1;
	char addr[BT_ADDR_LE_STR_LEN];

	
		bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

		if (err) {
			printk("Failed to connect to %s (%u)\n", addr, err);

			bt_conn_unref(default_conn);
			default_conn = NULL;

			start_scan();
			return;
		}

		if (conn != default_conn) {
			return;
		}

		printk("Connected: %s\n", addr);
		k_msleep(250);
		//rozlaczenie jako potwierdzenie odebrania wiadomosci przez ADV
		//bt_conn_disconnect(conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN);
	
	bt_conn_disconnect(conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN);
}

static void blink(void){
	if(flag == 0){
		gpio_pin_toggle_dt(&led);
		k_msleep(100);
	}
	else{
		gpio_pin_set_dt(&led,1);
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	flag = 0;
	char addr[BT_ADDR_LE_STR_LEN];

	if (conn != default_conn) {
		return;
	}

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
	printk("Disconnected: %s (reason 0x%02x)\n", addr, reason);
	bt_conn_unref(default_conn);
	default_conn = NULL;

	start_scan();
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

int main(void)
{
	int err;

	//blink
	gpio_is_ready_dt(&led);
	gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);

	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return 0;
	}

	printk("Bluetooth initialized\n");
	
	start_scan();
	while(1)
		blink();
		k_sleep(K_MSEC(500));
	return 0;
}
