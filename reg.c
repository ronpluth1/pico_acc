#include <stdio.h>
#include "reg.h"

int init_acc(void) {
    printf("Initializing...");
    gpio_init(VCC_PIN);
    gpio_set_function(VCC_PIN, GPIO_FUNC_SIO);
    gpio_set_dir(VCC_PIN, GPIO_OUT);
    gpio_put(VCC_PIN, 1);

    i2c_init(i2c1, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    bi_decl(bi_2pins_with_func(SDA_PIN, SCL_PIN, GPIO_FUNC_I2C));
    printf("Done");
}

void write_acc_reg(unsigned char ra, unsigned char val) {
    unsigned char buf[2];
    buf[0] = ra;
    buf[1] = val;
    i2c_write_blocking (i2c1, ACC_I2C_ADDR, buf, 2, 0);
}

unsigned char read_acc_reg(unsigned char ra) {
    unsigned char val;
     
    i2c_write_blocking (i2c1, ACC_I2C_ADDR, &ra, 1, 1);
    i2c_read_blocking(i2c1, ACC_I2C_ADDR, &val, 1, 0);

    return (val);
}

void read_acc_reg_multiple(unsigned char ra, unsigned char *buf, int num_bytes) {
    i2c_write_blocking (i2c1, ACC_I2C_ADDR, &ra, 1, 1);
    i2c_read_blocking(i2c1, ACC_I2C_ADDR, buf, num_bytes, 0);
}
