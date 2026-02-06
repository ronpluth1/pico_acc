#include <stdio.h>
#include <unistd.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "adxl345.h"
#include "reg.h"
#include "math.h"

#define NUM_SAMPLES 20

void update_fifo(adxl345_sample *sample, int16_t *sample_fifo, float *averages) {
    int sample_num;

    averages[0] = 0.0; averages[1] = 0.0; averages[2] = 0.0;
    for (sample_num = NUM_SAMPLES-1; sample_num > 0; sample_num--) {
        sample_fifo[sample_num*3+0] = sample_fifo[(sample_num-1)*3+0];
        sample_fifo[sample_num*3+1] = sample_fifo[(sample_num-1)*3+1];
        sample_fifo[sample_num*3+2] = sample_fifo[(sample_num-1)*3+2];
    };
    sample_fifo[0] = sample->rawX;
    sample_fifo[1] = sample->rawY;
    sample_fifo[2] = sample->rawZ;
    for (sample_num = 0; sample_num < NUM_SAMPLES; sample_num++) {
        averages[0] += sample_fifo[sample_num*3+0];
        averages[1] += sample_fifo[sample_num*3+1];
        averages[2] += sample_fifo[sample_num*3+2];
    };
    averages[0] /= (float) NUM_SAMPLES;
    averages[1] /= (float) NUM_SAMPLES;
    averages[2] /= (float) NUM_SAMPLES;
}

int main() {

    float averages[3];
    int16_t sample_fifo[NUM_SAMPLES*3];
    float mag;
    float diff;
    int i;

    adxl345_sample sample;

    bi_decl(bi_program_description("This is a test binary."));

    stdio_init_all();
    while (!stdio_usb_connected()); 

    init_acc();

    printf("Device ID is %x\n", read_acc_reg(RA_DEVID));

    /* Change the data rate */
    write_acc_reg(RA_BW_RATE, DR_12p5);
    printf("Data rate register is %x\n", read_acc_reg(RA_BW_RATE));

    /* Set the accelerometer to full-resolution mode at 2g */
    write_acc_reg(RA_DATA_FORMAT, 0x08);
    printf("Data format register is %x\n", read_acc_reg(RA_DATA_FORMAT));

    /* Set the offsets with the values we measured manually */
    write_acc_reg(RA_OFSX, 2);
    write_acc_reg(RA_OFSY, -1);
    write_acc_reg(RA_OFSZ, 3);

    /* Turn on accelerator and start measurement */
    write_acc_reg(RA_POWER_CTL, 0x00);
    printf("Power control register is %x\n", read_acc_reg(RA_POWER_CTL));
    write_acc_reg(RA_POWER_CTL, 0x08);
    printf("Power control register is %x\n", read_acc_reg(RA_POWER_CTL));

    update_raw(&sample, 1);

    for (i = 0; i < NUM_SAMPLES; i++) {
        update_raw(&sample, 1);
        update_fifo(&sample, sample_fifo, averages);
    }

    while (1) {
        update_raw(&sample, 1);

#define DIFF_REQUIRED 8.0

        diff = fabs((sample.rawX) - averages[0]) +
               fabs((sample.rawY) - averages[1]) +
               fabs((sample.rawZ) - averages[2]);

        if (diff > DIFF_REQUIRED) {
            mag = sqrt( pow(sample.rawX - averages[0], 2) +
                        pow(sample.rawY - averages[1], 2) +
                        pow(sample.rawZ - averages[2], 2)) * 0.0039;

            printf("%d.%03d,%d,%d,%d,%f,%4.2f,%4.2f,%4.2f(%4.2f)\n", 
                sample.sec, 
                sample.ms, 
                sample.rawX, 
                sample.rawY, 
                sample.rawZ, 
                mag,
                averages[0],
                averages[1],
                averages[2],
                diff
                );
        }
        update_fifo(&sample, sample_fifo, averages);
    }
    char   wb;

    while (1) {
        puts("Hello World\n");
        
        printf("Doing the write/read now...\n");
        wb = read_acc_reg (RA_DEVID);
        printf ("Device ID is %b\n", wb);

        sleep_ms(1000);
    }
}
