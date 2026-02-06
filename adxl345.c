#include "reg.h"
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "adxl345.h"

void update_raw(adxl345_sample *sample, int new_da) {
    unsigned char raw_buf[6];
    unsigned char da;
    uint64_t    time_in_us;

    if (new_da) {
        do {
            da = read_acc_reg(RA_INT_SOURCE) & 0x80;
        } while (!da);
    }

    time_in_us = time_us_64();
    sample->sample_time = ((float) time_in_us) / 1000000.0;

    read_acc_reg_multiple(RA_DATAX0, raw_buf, 6);
    sample->rawX = (raw_buf[1] << 8) | raw_buf[0];
    sample->rawY = (raw_buf[3] << 8) | raw_buf[2];
    sample->rawZ = (raw_buf[5] << 8) | raw_buf[4];
}
