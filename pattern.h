#ifndef PATTERN_H
#define PATTERN_H



class pattern {
    private:
        led_bar *bar;

    public:
        pattern(led_bar *bar);
        void scanning_rainbow();
        void rotating_rainbow();
        void scanning_effect(float fade, float speed, uint8_t hue, uint8_t sat, uint8_t val);
        void strobe_effect(float speed, uint8_t hue, uint8_t sat, uint8_t val);
        void sparkfire(int32_t cooling , int32_t sparking, int32_t speed_delay);
        void lakers_theme(int speed);
        void random_color(int count, int speed);
        void gaussian(double mean, double variance, uint8_t h, uint8_t s, uint8_t v);

};

#endif