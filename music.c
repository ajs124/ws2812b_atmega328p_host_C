#include <stdlib.h>
#include <complex.h>
#include <fftw3.h>
#include <pulse/simple.h>
#include "config.h"
#include "leds.h"

#define N 1024

void generate_colors(struct led *colors, int num) {
    int i=0;
    for(int a=154; a>33; a-=20) {
	for(int b=154; b>33; b-=20) {
	    if(a != b) {
                for(int c=154; c>33; c-=20) {
	            if(i++ < num && b != c) {
	                colors[i].r = a;
	                colors[i].g = b;
	                colors[i].b = c;
	            }
	        }
	    }
	}
    }
}

void music(int fd) {
    struct led leds[NUM_LEDS], colors[NUM_LEDS];
    generate_colors(colors, NUM_LEDS);
    float buf[N];

    double *in = fftw_alloc_real(N);
    fftw_complex *out = fftw_alloc_complex(N);
    fftw_plan p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);

    struct pa_simple *s;
    struct pa_sample_spec ss;

    ss.format = PA_SAMPLE_FLOAT32LE;
    ss.channels = 1;
    ss.rate = 44100;

    s = pa_simple_new(NULL, "ws2812b", PA_STREAM_RECORD, NULL, "Visualization", &ss, NULL, NULL, NULL);
    if(s == NULL) {
        printf("failed to connect to pulseaudio\n");
        exit(1);
    }
    printf("pulseaudio latency: %lu\n", pa_simple_get_latency(s, NULL));

    while(1) {
        pa_simple_read(s, buf, N*sizeof(float), NULL);
        for(int i=0; i<N; ++i) {
            in[i] = (double) buf[i];
        }
        fftw_execute(p);
        printf("\n");
        for(int i=0; i<N/2; ++i) {
            double a = cabs(out[i])*10;
            printf("%lf  ", a);
            for(int j=0; j<a && j<NUM_LEDS; ++j) {
                leds[j] = colors[NUM_LEDS-i-1];
            }
        }
        write_leds(fd, leds, NUM_LEDS);
    }
    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);
    fftw_cleanup();
    pa_simple_free(s);
}
