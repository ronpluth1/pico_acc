#ifndef REG_H
#define REG_H

#include <math.h>

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

#define VCC_PIN 10
#define SDA_PIN 14
#define SCL_PIN 15

#define ACC_I2C_ADDR 0x53

#define RA_DEVID            0x00
#define RA_THRESH_TAP       0x1d
#define RA_OFSX             0x1e
#define RA_OFSY             0x1f
#define RA_OFSZ             0x20
#define RA_DUR              0x21
#define RA_LATENT           0x22
#define RA_WINDOW           0x23
#define RA_THRESH_ACT       0x24
#define RA_THRESH_INACT     0x25
#define RA_TIME_INACT       0x26
#define RA_ACT_INACT_CTL    0x27
#define RA_THRESH_FF        0x28
#define RA_TIME_FF          0x29
#define RA_TAP_AXES         0x2a
#define RA_ACT_TAP_STATUS   0x2b
#define RA_BW_RATE          0x2c
#define RA_POWER_CTL        0x2d
#define RA_INT_ENABLE       0x2e
#define RA_INT_MAP          0x2f
#define RA_INT_SOURCE       0x30
#define RA_DATA_FORMAT      0x31
#define RA_DATAX0           0x32
#define RA_DATAX1           0x33
#define RA_DATAY0           0x34
#define RA_DATAY1           0x35
#define RA_DATAZ0           0x36
#define RA_DATAZ1           0x37
#define RA_FIFO_CTL         0x38
#define RA_FIFO_STATUS      0x39

int init_acc(void);
void write_acc_reg(unsigned char ra, unsigned char val);
unsigned char read_acc_reg(unsigned char ra);
void read_acc_reg_multiple(unsigned char ra, unsigned char *buf, int num_bytes);

#endif
