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

#include "tsl.h"

#define LED0_NODE DT_ALIAS(led0) //blink
#define PIR1_NODE DT_ALIAS(pir) //pir

bool flag = 0; //blink flag
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec pir = GPIO_DT_SPEC_GET_OR(PIR1_NODE, gpios, {0});

//uint32_t msg = 10; // test

uint64_t time_stamp = 0;

const uint32_t id = 0;
volatile uint32_t lux = 0;
volatile uint8_t movement = 1;

// ID | LUX | PIR
volatile uint32_t msg [] = {id, 0, 0};

// | 1 bajt flagi | 4 bajty ID | 4 bajty LUX | 4 bajty PIR |
static struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, 100),
    BT_DATA(BT_DATA_MANUFACTURER_DATA, &msg, sizeof(msg)),
};


static void blink(void){
	if(flag == 0){
		//gpio_pin_toggle_dt(&led);
		gpio_pin_set_dt(&led,0);
		//    k_msleep(100);
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
		//flag = 1;
		bt_le_adv_stop();
	}
}
static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	printk("Disconnected (reason 0x%02x)\n", reason);
	//flag = 0;
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

static struct gpio_callback pin_cb_data;

void pin_isr(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{	
	bt_le_adv_stop();
	bt_start();
	lux = READlux();		
	msg[1] = lux;		
	adv_start();
}

static void pin_cfg(void){
	gpio_is_ready_dt(&led);
	gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	gpio_pin_configure_dt(&pir, GPIO_INPUT);
	gpio_pin_interrupt_configure_dt(&pir, GPIO_INT_EDGE_TO_INACTIVE);
	gpio_init_callback(&pin_cb_data, pin_isr, BIT(pir.pin));
	gpio_add_callback(pir.port, &pin_cb_data);
}

int main(void)
{	
	INITtsl();
	//blink
	
	pin_cfg();

	bt_start();
	adv_start();

	while (1) {
		
		blink();
		//value to send
		movement = gpio_pin_get_dt(&pir);
		msg[2] = movement;
		flag = movement;

		if(k_uptime_get() - time_stamp >= 10000){
			bt_start();
			lux = READlux();
			msg[1] = lux;
			adv_start();
		}

		/*
		if(movement == 0){
			bt_start();
			lux = READlux();		
			msg[1] = lux;		
			adv_start();
		}
		*/
		
		k_sleep(K_MSEC(5000));
		//pm_state_force(0, &(struct pm_state_info){PM_STATE_SUSPEND_TO_IDLE, 0, 0});
	}
	return 0;
}
