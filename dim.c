#include <unistd.h>
#include "leds.h"
#include "dim.h"

void dim(int fd, int brightness) {
    struct led leds[NUM_LEDS];
    for(int i=0; i<NUM_LEDS; ++i) {
        leds[i].r = brightness*1.4;
        leds[i].g = brightness;
        leds[i].b = brightness*0.9;
    }
    write_leds(fd, leds, NUM_LEDS);
    while(1) {
        write_leds(fd, leds, 0);
        sleep(1);
    }
}
