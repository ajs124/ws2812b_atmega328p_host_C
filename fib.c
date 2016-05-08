#include "config.h"
#include "colorspaces.h"
#include "leds.h"
#include <unistd.h>

static unsigned long long fib_num(int n) {
    unsigned long long a = 0, b = 1;
    for (int i = 31; i >= 0; i--) {
	unsigned long long d = a * (b * 2 - a);
	unsigned long long e = a * a + b * b;
	a = d;
	b = e;
	if ((((unsigned int)n >> i) & 1) != 0) {
	    unsigned long long c = a + b;
	    a = b;
	    b = c;
	}
    }
    return a;
}

#define N 14
#define factor 100.0

void fib(int fd, int direction) {
    struct led leds[NUM_LEDS];
    struct hsv hsv_tuples[N];
    struct rgb a;
    for(int i=0; i<N; ++i) {
        hsv_tuples[i].h = i/(double) N*360.0;
        hsv_tuples[i].s = 0.9;
        hsv_tuples[i].v = 0.7;
    }
    for(int x=0; x<N; ++x) {
    	for(int i=fib_num(x); i<fib_num(x+1) && i<NUM_LEDS; ++i) {
    	    a = hsv2rgb(hsv_tuples[x]);
    	    leds[i].r = (uint8_t) (a.r*factor);
    	    leds[i].g = (uint8_t) (a.g*factor);
    	    leds[i].b = (uint8_t) (a.b*factor);
        }
    }
    while(1) {
    	rotate((direction == LEFT) ? 1 : -1, leds, NUM_LEDS);
    	write_leds(fd, leds, NUM_LEDS);
    	usleep(25000);
    }
}
