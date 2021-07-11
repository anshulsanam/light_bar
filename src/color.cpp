#include <math.h>

#include "color.h"

#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _b : _a; })



color::color() {
    curr_color = new HSV();
    curr_color->h = 0;
    curr_color->s = 0;
    curr_color->v = 0;
}

color::color(uint8_t h, uint8_t s, uint8_t v) {
    curr_color = new HSV();
    curr_color->h = h;
    curr_color->s = s;
    curr_color->v = v;
}

void color::hsv_to_rgb(struct RGB *rgb) {
    int32_t i;
    float f, p, q, t;
    float h, s, v;    

    h = 359.0 * ((float) curr_color->h / 255.0);

    h = MAX(0.0, MIN(360.0, h));
    s = MAX(0.0, MIN(100.0, curr_color->s));
    v = MAX(0.0, MIN(100.0, curr_color->v));

    s /= 100;
    v /= 100;

    if (s == 0) {
        rgb->r = rgb->g = rgb->b = round(v * 255);
        return;
    }

    h /= 60;
    i = floor(h);
    f = h - i;
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));

    switch (i) {
        case 0:
            rgb->r = round(255 * v);
            rgb->g = round(255 * t);
            rgb->b = round(255 * p);
            break;
        case 1:
            rgb->r = round(255 * q);
            rgb->g = round(255 * v);
            rgb->b = round(255 * p);
            break;
        case 2:
            rgb->r = round(255 * p);
            rgb->g = round(255 * v);
            rgb->b = round(255 * t);
            break;
        case 3:
            rgb->r = round(255 * p);
            rgb->g = round(255 * q);
            rgb->b = round(255 * v);
            break;
        case 4:
            rgb->r = round(255 * t);
            rgb->g = round(255 * p);
            rgb->b = round(255 * v);
            break;
        default:
            rgb->r = round(255 * v);
            rgb->g = round(255 * p);
            rgb->b = round(255 * q);
   }

}

color::~color() {
    delete curr_color;
}