#include <stdio.h>
#include <unistd.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "adxl345.h"
#include "reg.h"
#include "math.h"

#define NUM_SAMPLES     20
#define DIFF_REQUIRED   12.0
#define TIME_WINDOW     2.0

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

    float window_start_time = 0.0;
    float window_end_time = 0.0;
    float max_diff = 0;
    int quiesced = 0;

    while (1) {

        update_raw(&sample, 1);

        float maxdiff = 0.0;

        diff = fabs((sample.rawX) - averages[0]) +
               fabs((sample.rawY) - averages[1]) +
               fabs((sample.rawZ) - averages[2]);

        float new_window = sample.sample_time - window_end_time > TIME_WINDOW;

        if (new_window && !quiesced) {
            // Print data for the last window
            printf("%15.3f %10.2f %15.3f\n", window_start_time, max_diff, 
                                       window_end_time-window_start_time);
            quiesced = 1;
        }

        if (diff > DIFF_REQUIRED) {
            quiesced = 0;
            if (new_window) {
                // Out of the window -- starting a new window
                window_start_time = sample.sample_time;
                max_diff = diff;
            } else {
                // In the window
                if (diff > max_diff) {
                    max_diff = diff;
                }
            }
            window_end_time = sample.sample_time;
        }
        update_fifo(&sample, sample_fifo, averages);
    }
}
