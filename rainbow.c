#include "colorspaces.h"
#include "leds.h"
#include <math.h>
#include <unistd.h>

#define FACTOR 100.0

void rainbow(int fd, size_t num, int dir) {
    struct led leds[NUM_LEDS];
    size_t N = ceil(NUM_LEDS/num);
    struct hsv hsv_tuples[N];
    struct rgb a;
    for(size_t i=0; i<N; ++i) {
        hsv_tuples[i].h = i/(double) N*360;
        hsv_tuples[i].s = 0.9;
        hsv_tuples[i].v = 0.7;
    }
    for(size_t i=0; i<NUM_LEDS; ++i) {
        a = hsv2rgb(hsv_tuples[i%N]);
        leds[i].r = a.r*FACTOR;
        leds[i].g = a.g*FACTOR;
        leds[i].b = a.b*FACTOR;
    }
    while(1) {
        rotate((dir == LEFT) ? 1 : -1, leds, NUM_LEDS);
        write_leds(fd, leds, NUM_LEDS);
        usleep(25000);
    }
}
