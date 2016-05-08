CC=gcc
WARNINGS   = 	-Waddress                           \
                -Wchar-subscripts                   \
                -Wclobbered                         \
                -Wcomment                           \
                -Wempty-body                        \
                -Wformat                            \
                -Wignored-qualifiers                \
                -Wimplicit-function-declaration     \
                -Wimplicit-int                      \
                -Winit-self                         \
                -Winline                            \
                -Wmain                              \
                -Wmissing-braces                    \
                -Wmissing-field-initializers        \
                -Wmissing-format-attribute          \
                -Wmissing-parameter-type            \
                -Wnested-externs                    \
                -Wnonnull                           \
                -Wold-style-declaration             \
                -Wold-style-definition              \
                -Woverride-init                     \
                -Wpacked                            \
                -Wparentheses                       \
                -Wpointer-sign                      \
                -Wreturn-type                       \
                -Wsequence-point                    \
                -Wsign-compare                      \
                -Wstrict-aliasing=2                 \
                -Wstrict-overflow=1                 \
                -Wstrict-prototypes                 \
                -Wswitch                            \
                -Wtrigraphs                         \
                -Wtype-limits                       \
                -Wundef                             \
                -Wuninitialized                     \
                -Wunknown-pragmas                   \
                -Wunreachable-code                  \
                -Wunsafe-loop-optimizations         \
                -Wunused-function                   \
                -Wunused-label                      \
                -Wunused-value                      \
                -Wunused-variable                   \
                -Wvolatile-register-var             \
		-Wall			            \
		-Wextra
CFLAGS = -I. -std=gnu99 -O2 -fprefetch-loop-arrays -fPIC $(WARNINGS) # -Werror -g -mtune=native -march=native
OBJECTS = ws2812b_atmega328p_host.o serial.o power.o fib.o music.o rainbow.o dim.o blink.o leds.o colorspaces.o
DEPS = blink.h colorspaces.h config.h dim.h fib.h leds.h music.h power.h rainbow.h serial.h
LIBS = -lm -lfftw3 -lpulse-simple

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ws2812b_atmega328p_host: $(OBJECTS) 
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

all: ws2812b_atmega328p_host

clean:
	rm -f *.o ws2812b_atmega328p_host	
