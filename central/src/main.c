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

uint8_t ctr = 0;

static void start_scan(void);

static struct bt_conn *default_conn;

// Funkcja do wyświetlania danych reklamowych

static void print_ad_data(struct net_buf_simple *ad)
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
            //printk("  Flags: 0x%02x\n", net_buf_simple_pull_u8(ad));
			uint8_t adres = net_buf_simple_pull_u8(ad);
            len--;
            break;
		/*
        case BT_DATA_UUID16_SOME:
        case BT_DATA_UUID16_ALL:
            printk("  UUID16: ");
            while (len >= 2) {
                uint16_t uuid = net_buf_simple_pull_le16(ad);
                printk("0x%04x ", uuid);
                len -= 2;
            }
            printk("\n");
            break;
		*/
		case BT_DATA_MANUFACTURER_DATA:
		if(adres == 6){
            if (len >=  sizeof(uint32_t)) {
                uint32_t msg;
                memcpy(&msg, ad->data, sizeof(msg));
				printk("  Manufacturer Data:\n Heartrate: %u\n", msg);
                net_buf_simple_pull(ad, sizeof(msg));
                len -= sizeof(msg);
            } else {
                printk("  Manufacturer Data: Insufficient length\n");
                net_buf_simple_pull(ad, len);
                len = 0;
            }
            break;
		}
		/*
        case BT_DATA_NAME_SHORTENED:
        case BT_DATA_NAME_COMPLETE:
            printk("  Name: %.*s\n", len, (char *)ad->data);
            net_buf_simple_pull(ad, len);
            break;
		
        default:
            printk("  Unknown AD type: 0x%02x, length: %u\n", type, len);
            net_buf_simple_pull(ad, len);
            break;
		*/
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
	printk("Device found: %s (RSSI %d)\n", addr_str, rssi);

	print_ad_data(ad);

	/* connect only to devices in close proximity */
	if (rssi < -50) {
		return;
	}

	if (bt_le_scan_stop()) {
		return;
	}

	err = bt_conn_le_create(addr, BT_CONN_LE_CREATE_CONN,
				BT_LE_CONN_PARAM_DEFAULT, &default_conn);
	if (err) {
		printk("Create conn to %s failed (%d)\n", addr_str, err);
		start_scan();
	}
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

	//if(addr != '6D:2F:90:19:F3:11')
	k_msleep(5000);
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
	return 0;
}
