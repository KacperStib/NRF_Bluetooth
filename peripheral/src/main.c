/* main.c - Application main entry point */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>

#include <zephyr/drivers/gpio.h>

#include <zephyr/settings/settings.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/services/bas.h>
#include <zephyr/bluetooth/services/hrs.h>
#include <zephyr/bluetooth/services/ias.h>

#define LED0_NODE DT_ALIAS(led0) //blink
bool flag = 0; //blink flag
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

uint8_t id = 0;
uint32_t msg = 10; // test

uint64_t time_stamp = 0;

// | 1 bajt flagi | 1 bajt id | 4 bajty wartosc |
static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_MANUFACTURER_DATA, &id, sizeof(id)),
    BT_DATA(BT_DATA_MANUFACTURER_DATA, &msg, sizeof(msg)),
};


static void blink(void){
	if(flag == 0){
		gpio_pin_toggle_dt(&led);
		k_msleep(100);
	}
	else{
		gpio_pin_set_dt(&led,1);
	}
}

static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		printk("Connection failed (err 0x%02x)\n", err);
	} else {
		printk("Connected\n");
		flag = 1;
		bt_le_adv_stop();
	}
}
static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	printk("Disconnected (reason 0x%02x)\n", reason);
	flag = 0;
	time_stamp = k_uptime_get();
	int disc = bt_disable();
}
BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

static void bt_start(void){
	int err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return 0;
	}
}

static void adv_start(void)
{
	int err;

	printk("Bluetooth initialized\n");
	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
	}

	err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		return;
	}

	printk("Advertising successfully started\n");
}

int main(void)
{
	//blink
	gpio_is_ready_dt(&led);
	gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);

	bt_start();
	adv_start();

	while (1) {
		
		blink();
		//value to send
		if(k_uptime_get() - time_stamp >= 10000){
			bt_start();
			msg += 10;
			time_stamp = k_uptime_get();
			adv_start();
		}
		k_sleep(K_MSEC(100));
	}
	return 0;
}
