#ifndef LED_BAR_H
#define LED_BAR_H



class led_bar {
    private:
        int32_t led_len;
        int32_t width;
        int32_t height;
        spi led_bar_connection;
        
        struct led {
            uint8_t brightness;
            uint8_t blue;
            uint8_t green;
            uint8_t red;
        };

        led *led_bar_data;


    public:
        // Number of LEDs on the widths and number of LEDs on heights
        led_bar(int width, int height);
        void clear();
        void write_to_LEDs();
        void set_LED_RGB(uint8_t idx, uint8_t brightness, uint8_t red, uint8_t green, uint8_t blue);
        void set_LED_HSV(uint8_t idx, uint8_t brightness, uint8_t h, uint8_t s, uint8_t v);
        void shift_all_once(int dir);
        int32_t get_LED_len();
        int32_t get_width();
        int32_t get_height();
        // void scanning_bar();

        ~led_bar();


};

#endif