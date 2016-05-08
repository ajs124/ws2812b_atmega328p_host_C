#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sched.h>
#include "serial.h"
#include "rainbow.h"
#include "music.h"
#include "fib.h"
#include "dim.h"
#include "power.h"
#include "blink.h"
#include "config.h"
#include "leds.h"

int fd_hack;

void sig_handler(int signum) __attribute__((noreturn));
void sig_handler(__attribute__((unused)) int signum) {
    fprintf(stderr,"Received SIG(INT|TERM)\n");
    fflush(stderr);
	close(fd_hack);
	exit(1);
}

// based on https://stackoverflow.com/questions/7775991/how-to-get-hexdump-of-a-structure-data
void hexdump(void *addr, int len);
void hexdump(void *addr, int len) {
    #define LINEWIDTH 32
    int i;
    char buff[LINEWIDTH+1];
    char *pc = (char*) addr;

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        if ((i % LINEWIDTH) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0) {
                printf("  %s\n", buff);
            }

            // Output the offset.
            printf("  %04x  ", i);
        }

        // Now the hex code for the specific character.
        printf(" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % LINEWIDTH] = '.';
        else
            buff[i % LINEWIDTH] = pc[i];
        buff[(i % LINEWIDTH) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % LINEWIDTH) != 0) {
        fputs("   ", stdout);
        i++;
    }

    // And print the final ASCII bit.
    printf("  %s\n", buff);
}

void showUsage(void) __attribute__((noreturn));
void showUsage(void) {
    fprintf(stderr, "valid agruments: on [num], off, blink freq [r, g, b], music, rainbow num [dir], fib [dir], dim [brightness]\n");
    exit(1);
}

inline uint8_t parse_rgb(char* a) {
    return (uint8_t) (atoi(a) & (0xFE));
}

int main(int argc, char* argv[]) {
    if(argc<2) {
        showUsage();
    }
    int fd = serial_open();
    if(fd < 0) {
        fprintf(stderr, "failed to open serial port\n");
        return 1;
    }
    if(nice(-20) == -1) {
        fprintf(stderr, "failed to nice\n");
    }
    struct sched_param param;
    param.sched_priority = 1;
    sched_setscheduler(getpid(), SCHED_RR, &param);
    fd_hack = fd;
    signal(SIGTERM, sig_handler);
    signal(SIGINT, sig_handler);
    if(strncmp(argv[1], "music", strlen("music")) == 0) {
        music(fd);
    } else if(strncmp(argv[1], "rainbow", strlen("rainbow")) == 0) {
        rainbow(fd, atoi(argv[2]), argc>3 ? atoi(argv[3]) : LEFT);
    } else if(strncmp(argv[1], "fib", strlen("fib")) == 0) {
        fib(fd, argc>2 ? atoi(argv[2]) : LEFT);
    } else if(strncmp(argv[1], "on", strlen("on")) == 0) {
        on(fd, argc>2 ? atoi(argv[2]) : NUM_LEDS);
    } else if(strncmp(argv[1], "off", strlen("off")) == 0) {
        off(fd);
    } else if(strncmp(argv[1], "dim", strlen("dim")) == 0) {
        dim(fd, argc>2 ? atoi(argv[2]) : 64);
    } else if(strncmp(argv[1], "blink", strlen("blink")) == 0) {
        if(argc == 3)
            blink(fd, atoi(argv[2]), 80, 80, 80);
        else if(argc == 6)
            blink(fd, atoi(argv[2]), parse_rgb(argv[3]), parse_rgb(argv[4]), parse_rgb(argv[5]));
        else {
            fprintf(stderr, "wrong number of arguments for blink. freq or freq r g b are valid\n");
            return 1;
        }
    } else {
        showUsage();
    }
	close(fd);
    return 0;
}
