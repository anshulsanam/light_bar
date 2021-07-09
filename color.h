#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

#define HSV_CHANNEL_MAX          255
#define HSV_CHANNEL_MIN          0


struct RGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct HSV {
    uint8_t h;
    uint8_t s;
    uint8_t v;
};

class color {

    private:
        HSV *curr_color;

    public:
        color();
        color(uint8_t h, uint8_t s, uint8_t v);
        void hsv_to_rgb(struct RGB *rgb);
        ~color();
        

};

#endif