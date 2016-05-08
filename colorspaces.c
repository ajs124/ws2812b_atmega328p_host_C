#include "colorspaces.h"
#include <math.h>

#define MAX(a,b,c) (a>b && a>c)?a:(b>c?b:c)
#define MIN(a,b,c) (a<b && a<c)?a:(b<c?b:c)

// https://www.cs.rit.edu/~ncs/color/t_convert.html
// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
//	if s == 0, then h = -1 (undefined)

struct hsv rgb2hsv(struct rgb in) {
    struct hsv out;
    double min, max, delta;

    min = MIN(in.r, in.g, in.b);
    max = MAX(in.r, in.g, in.b);
    out.v = max;			// v

    delta = max - min;

    if(max != 0) {
        out.s = delta / max;	// s
    } else {
	// r = g = b = 0		// s = 0, v is undefined
	out.s = 0;
	out.h = -1;
	return out;
    }

    if(in.r == max) {
        out.h = (in.g - in.b) / delta;		// between yellow & magenta
    } else if(in.g == max) {
        out.h = 2 + (in.b - in.r) / delta;	// between cyan & yellow
    } else {
        out.h = 4 + (in.r - in.g) / delta;	// between magenta & cyan
    }
    out.h *= 60;				// degrees
    if(out.h < 0) {
	out.h += 360;
    }
    return out;
}

struct rgb hsv2rgb(struct hsv in) {
    struct rgb out;
    int i;
    double f, p, q, t;

    if(in.s == 0) {
        // achromatic (grey)
        out.r = out.g = out.b = in.v;
        return out;
    }

    in.h /= 60;			// sector 0 to 5
    i = floor(in.h);
    f = in.h - i;		// factorial part of h
    p = in.v * (1 - in.s);
    q = in.v * (1 - in.s*f);
    t = in.v * (1 - in.s*(1-f));

    switch(i) {
	    case 0:
		    out.r = in.v;
		    out.g = t;
		    out.b = p;
		    break;
	    case 1:
		    out.r = q;
		    out.g = in.v;
		    out.b = p;
		    break;
	    case 2:
		    out.r = p;
		    out.g = in.v;
		    out.b = t;
		    break;
	    case 3:
		    out.r = p;
		    out.g = q;
		    out.b = in.v;
		    break;
	    case 4:
		    out.r = t;
		    out.g = p;
		    out.b = in.v;
		    break;
	    default:		// case 5:
		    out.r = in.v;
		    out.g = p;
		    out.b = q;
		    break;
    }
    return out;
}
