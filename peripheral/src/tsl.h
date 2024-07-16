#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>
#include <zephyr/device.h>

/*to main file
#define I2C0_NODE DT_NODELABEL(mysensor)
static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C0_NODE);
*/

/*to overlay
&i2c0 {
    mysensor: mysensor@29{
        compatible = "i2c-device";
        reg = < 0x29 >;
        label = "MYSENSOR";
    };
};
*/

/*to proj.conf
CONFIG_GPIO=y
CONFIG_I2C=y
*/

#define I2C0_NODE DT_NODELABEL(mysensor)
static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C0_NODE);

uint8_t reg;
int ret;

static void INITtsl(void){
    uint8_t config[2] = {0x00, 0x03};   
    ret = i2c_write_dt(&dev_i2c, config, sizeof(config));
    if (ret != 0) {
        printk("Failed to write to I2C device address %x at reg. %x\n", dev_i2c.addr, config[0]);
    }
	//gain tsl.setGain(TSL2591_GAIN_MED); 0x10
	//timeing tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS); 0x02
	uint8_t gain[2] = {(0xA0 | 0x01), (0x02 | 0x10)};
	ret = i2c_write_dt(&dev_i2c, gain, sizeof(gain));
    if (ret != 0) {
        printk("Failed to write to I2C device address %x at reg. %x\n", dev_i2c.addr, config[0]);
    }
}

static uint8_t READid(void){
    reg = (0xA0 | 0x12);
    uint8_t data;
    ret = i2c_write_read_dt(&dev_i2c, &reg, 1, &data, 1);
    if (ret != 0) {
        printk("Failed to read from I2C device address %x at Reg. %x\n", dev_i2c.addr, reg);
    }
	//printk("Dev ID: %d\n", data);
    return data;
}

static uint32_t READlux(void){
    for (uint8_t d = 0; d <= 2; d++) {
    		k_msleep(120);
  		}

		uint32_t x;
 		uint16_t y;

		uint8_t buffer[2];
		buffer[0] = (0xA0 | 0x14);
		//i2c_dev->write_then_read(buffer, 1, buffer, 2);
		ret = i2c_write_read_dt(&dev_i2c, buffer, 1, buffer, 2);
		if (ret != 0) {
            printk("Failed to read from I2C device address %x at Reg. %x\n", dev_i2c.addr, reg);
            return;
        }
		//y = (uint16_t(buffer[1]) << 8 | uint16_t(buffer[0]));
		y = (buffer[1] << 8 | buffer[0]);
		//uint8_t buffer[2];
		buffer[0] = (0xA0 | 0x16);
		//i2c_dev->write_then_read(buffer, 1, buffer, 2);
		ret = i2c_write_read_dt(&dev_i2c, buffer, 1, buffer, 2);
		if (ret != 0) {
            printk("Failed to read from I2C device address %x at Reg. %x\n", dev_i2c.addr, reg);
            return;
        }
		x = (buffer[1] << 8 | buffer[0]);

		x <<= 16;
 		x |= y;
	
		//printk("LUX: %d\n", x);
        return x;
}