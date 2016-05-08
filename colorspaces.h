#ifndef COLORSPACES_H
#define COLORSPACES_H

struct rgb {
    double r;       // percent
    double g;       // percent
    double b;       // percent
};

struct hsv {
    double h;       // angle in degrees
    double s;       // percent
    double v;       // percent
};

extern struct hsv rgb2hsv(struct rgb in);
extern struct rgb hsv2rgb(struct hsv in);

#endif /* COLORSPACES_H */
