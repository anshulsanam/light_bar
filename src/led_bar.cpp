#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <iostream>

#include "led_bar.h"

using namespace std;

#define SPI_FRAME_SIZE_BYTES 4

static uint8_t start[4] = {0x0, 0x0, 0x0, 0x0};
static uint8_t finish[4] = {0xFF, 0xFF, 0xFF, 0xFF};

uint8_t gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

led_bar::led_bar(int width, int height) {
    this->width = width;
    this->height = height;
    led_len = 2 * width + 2 * height;
    

    led_bar_data = new led[led_len];
    for (int i = 0; i < led_len; i++) {
        led_bar_data[i].brightness = 0;
        led_bar_data[i].red = 0;
        led_bar_data[i].blue = 0;
        led_bar_data[i].green = 0;
    }

    led_bar_connection.spi_open();
    clear();
}

int32_t led_bar::get_LED_len() {
    return led_len;
}

int32_t led_bar::get_width() {
    return width;
}

int32_t led_bar::get_height() {
    return height;
}

void led_bar::clear() {
    for (int i = 0; i < led_len; i++)
        set_LED_RGB(i, 0, 0, 0, 0);

    write_to_LEDs();
}

void led_bar::write_to_LEDs() {
    uint8_t led[4] = {0x0, 0x0, 0x0, 0x0};

    led_bar_connection.transmit(start, SPI_FRAME_SIZE_BYTES);

    led_bar_connection.transmit(reinterpret_cast<uint8_t *>(led_bar_data), SPI_FRAME_SIZE_BYTES * led_len);

    for (int i = 0; i < (led_len / 2 + 7) / 8; ++i)
        led_bar_connection.transmit(finish, 1);
};

void led_bar::set_LED_RGB(int32_t idx, uint8_t brightness, uint8_t red, uint8_t green, uint8_t blue) {
    uint8_t blue_g8 = gamma8[blue];
    uint8_t green_g8 = gamma8[green];
    uint8_t red_g8 = gamma8[red];

    led color = {
        .brightness = brightness,
        .blue = blue_g8,
        .green = green_g8,
        .red = red_g8,
    };
    color.brightness |= 0xE0;

    if (idx > led_len) 
        return;

    led_bar_data[idx] = color;
    
}

void led_bar::set_LED_HSV(int32_t idx, uint8_t brightness, uint8_t h, uint8_t s, uint8_t v) {
    color color_hsv(h, s, v);
    RGB color_RGB;
    
    color_hsv.hsv_to_rgb(&color_RGB);

    uint8_t blue = gamma8[color_RGB.b];
    uint8_t green = gamma8[color_RGB.g];
    uint8_t red = gamma8[color_RGB.r];

    led color = {
        .brightness = brightness,
        .blue = blue,
        .green = green,
        .red = red,
    };
    color.brightness |= 0xE0;

    if (idx > led_len) 
        return;

    led_bar_data[idx] = color;
    
}

// uint8_t led_bar::get_LED_hue(int32_t idx) {
//     return led_bar_data
// }
// uint8_t led_bar::get_LED_sat(int32_t idx) {

// }
// uint8_t led_bar::get_LED_val(int32_t idx) {

// }

void led_bar::shift_all_once(int dir) {
    uint8_t i = 0;
	led buff;
	if(dir) {
		buff = led_bar_data[led_len - 1];
		for(i = (led_len - 1); i > 0; i--){
			led_bar_data[i] = led_bar_data[i - 1];
		}
		led_bar_data[0] = buff;
	}
    else {
		buff = led_bar_data[0];
		for(i = 0; i < (led_len - 1); i++){
			led_bar_data[i] = led_bar_data[i + 1];	
		}
		led_bar_data[led_len - 1] = buff;
	}
}

void led_bar::backup() {
    backup_bar_data = new led[get_LED_len()];

    for (int i = 0; i < get_LED_len(); i++)
        backup_bar_data[i] = led_bar_data[i];

}


void led_bar::restore() {
    for (int i = 0; i < get_LED_len(); i++)
        led_bar_data[i] = backup_bar_data[i];

    delete[] backup_bar_data;
}

led_bar::~led_bar() {
    delete[] led_bar_data;
}
