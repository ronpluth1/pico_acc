#ifndef ADXL345_H
#define ADXL345_H

#include <stdint.h>

#define DR_3200 0xF
#define DR_1600 0xE
#define DR_800  0xD
#define DR_400  0xC
#define DR_200  0xB
#define DR_100  0xA
#define DR_50   0x9
#define DR_25   0x8
#define DR_12p5 0x7
#define DR_6p25 0x6
#define DR_3p13 0x5
#define DR_1p56 0x4
#define DR_0p78 0x3
#define DR_0p39 0x2
#define DR_0p20 0x1
#define DR_0p10 0x0

typedef struct {
    int16_t   rawX;
    int16_t   rawY;
    int16_t   rawZ;
    int sec;
    int ms;
} adxl345_sample;

void update_raw(adxl345_sample *sample, int new_da);

#endif
