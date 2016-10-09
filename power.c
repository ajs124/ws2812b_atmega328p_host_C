#include <unistd.h>
#include <stdio.h>
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
    while(1) {
        write_leds(fd, leds, 0);
        sleep(1);
    }
}

void off(int fd) {
    unsigned char t = 0xFE;
    if(write(fd, &t, 1) != 1) {
        fprintf(stderr, "(probably) failed to turn off the psu\n!");
    }
}
