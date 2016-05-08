#include <unistd.h>
#include "leds.h"
#include "power.h"

void on(int fd, int num) {
    struct led leds[NUM_LEDS];
    for(int i=0; i < num && i < NUM_LEDS; ++i) {
        leds[i].r = 0x42;
        leds[i].g = 0x42;
        leds[i].b = 0x42;
    }
    for(int i=num; i < NUM_LEDS; ++i) {
        leds[i].r = 0;
        leds[i].g = 0;
        leds[i].b = 0;
    }
    write_leds(fd, leds, NUM_LEDS);
}

void off(int fd) {
    unsigned char t = 0xFE;
    write(fd, &t, 1);
}
