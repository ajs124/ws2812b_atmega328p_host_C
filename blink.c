#include <unistd.h>
#include "leds.h"
#include "blink.h"
#include <stdio.h>

void blink(int fd, size_t freq, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t sleeptime = 1000000.0/freq;
    struct led on[NUM_LEDS], off[NUM_LEDS];
    for(int i=0; i < NUM_LEDS; ++i) {
        on[i].r = r;
        on[i].g = g;
        on[i].b = b;
        off[i].r = 0;
        off[i].g = 0;
        off[i].b = 0;
    }
    while(1) {
        write_leds(fd, off, NUM_LEDS);
        usleep(sleeptime);
        write_leds(fd, on, NUM_LEDS);
        usleep(sleeptime);
    }
}
