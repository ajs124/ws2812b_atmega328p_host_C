#ifndef LEDS_H
#define LEDS_H

#include <stddef.h>
#include <stdint.h>
#include "config.h"

#define LEFT 0
#define RIGHT 1

struct led {
    uint8_t g;
    uint8_t r;
    uint8_t b;
}; // __attribute__((__packed__))

void write_leds(int fd, struct led* leds, size_t num);
void rotate(long offset, struct led* leds, size_t num);

#endif /* LEDS_H */
