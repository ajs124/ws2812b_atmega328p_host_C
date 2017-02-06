#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "leds.h"

void rotate(long offset, struct led* leds, size_t num) {
    if(offset % num == 0) return;
    size_t struct_size = sizeof(struct led);
    struct led* temp = malloc((size_t) labs(offset)*struct_size);
    if(offset < 0) {
        offset *= -1;
        offset = offset % num;
        temp = memcpy(temp, leds, (size_t) offset*struct_size);
        for(size_t i=0; i < num-offset; ++i) {
            leds[i] = leds[i+offset];
        }
        memcpy(leds+num-offset, temp, (size_t) offset*struct_size);
    } else { // offset > 0
        offset = offset % num;
        temp = memcpy(temp, leds+num-offset, (size_t) offset*struct_size); 
        for(long i=num-offset; i >= 0; --i) {
            leds[i+offset] = leds[i];
        }
        memcpy(leds, temp, (size_t) offset*struct_size);
    }
}

void write_leds(int fd, struct led* leds, size_t num) {
    int num_bytes = num*sizeof(struct led);
    if(write(fd, leds, num_bytes) != num_bytes) {
        fprintf(stderr, "Some stuff was not written to the serial port…\n");
    }
    uint8_t t = 0xFF;
    if(write(fd, &t, 1) != 1) {
        fprintf(stderr, "Failed to write finish frame \"opcode\"\n");
    }
    fsync(fd);
}
