#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <math.h>
#include <vector>
#include <stdlib.h>    
#include <time.h>    
#include <random>

#include "led_bar.h"
#include "color.h"
#include "pattern.h"


using namespace std::chrono;
using namespace std;



pattern::pattern(led_bar *bar) {
    this->bar = bar;
}

void pattern::scanning_rainbow() {
    int32_t led_len = bar->get_LED_len();
    int32_t led_width = bar->get_width();
    int32_t led_height = bar->get_height();

    int32_t i = 0;

    while (true) {
        for (int k = 0; k < led_width; k++) {
            bar->set_LED_HSV(k, 31, (i + static_cast<int>((k / static_cast<float>(led_width)) * 255)) % 255, 100, 100);
            bar->set_LED_HSV(led_len - led_height - k - 1, 31, (i + static_cast<int>((k / static_cast<float>(led_width)) * 255)) % 255, 100, 100);
        }
        bar->write_to_LEDs();
        i++;
        this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

void pattern::rotating_rainbow() {
    int32_t led_len = bar->get_LED_len();
    int32_t i = 0;

    while (true) {
        for (int k = 0; k < led_len; k++) {
            bar->set_LED_HSV(k, 31, (i + static_cast<int>((k / static_cast<float>(led_len)) * 255)) % 255, 100, 100);
        }
        bar->write_to_LEDs();
        i++;
        this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

/*
    Bigger fade means smaller bar
*/
void pattern::scanning_effect(float fade = 5, float speed = 1, uint8_t hue = 0, uint8_t sat = 100, uint8_t val = 100) {
    int leader = 0;
    int direction = 1;
    int32_t led_len = bar->get_LED_len();
    int32_t pixel_count = bar->get_width();
    int32_t led_height = bar->get_height();
    int h = hue; 
    int s = sat;
    uint8_t pixels[pixel_count];

    while (true) {
        leader += direction * speed;
        if (leader >= pixel_count) {
            direction = -direction;
            leader = pixel_count - 1;
            this_thread::sleep_for(std::chrono::milliseconds(20));
            bar->clear();
        }

        if (leader < 0) {
            direction = -direction;
            leader = 0;
            this_thread::sleep_for(std::chrono::milliseconds(20));
            bar->clear();
        }

        pixels[(int) floor(leader)] = val;

        for (int i = 0; i < pixel_count; i++) {
            pixels[i] -= fade;
            pixels[i] = max(0, (int)pixels[i]);
            bar->set_LED_HSV(i, 31, h, s, pixels[i]);
            bar->set_LED_HSV(led_len - led_height - i, 31, h, s, pixels[i]);
        }        
        bar->write_to_LEDs();
        this_thread::sleep_for(std::chrono::milliseconds(15));
    }
}

static double map_one_range_to_another(double input, double begin_A, double begin_B, double end_A, double end_B, int decimal_precision ) {
    double deltaA = begin_B - begin_A;
    double deltaB = end_B - end_A;
    double scale  = deltaB / deltaA;
    double negA   = -1 * begin_A;
    double offset = (negA * scale) + end_A;
    double finalNumber = (input * scale) + offset;
    int calcScale = (int) pow(10, decimal_precision);
    return (double) round(finalNumber * calcScale) / calcScale;
}

/*
 *
 *
 *  speed - Between 0 and 100
*/ 
void pattern::strobe_effect(float speed, uint8_t h, uint8_t s, uint8_t v) {
    int32_t led_count = bar->get_LED_len();
    float delay;

    if (speed < 0) delay = 100;
    if (speed > 100) delay = 10;
    delay = map_one_range_to_another(speed, 0, 100, 100, 10, 3);

    while (true) {

        for (int i = 0; i < led_count; i++) 
            bar->set_LED_HSV(i, 31, h, s, v);
        
        bar->write_to_LEDs();
        this_thread::sleep_for(std::chrono::milliseconds((int) delay));
            
        bar->clear();
        this_thread::sleep_for(std::chrono::milliseconds((int) delay));
    }
}

void set_pixel_heat_color(led_bar *bar, int pixel, uint8_t temperature) {
    // Scale 'heat' down from 0-255 to 0-191
    uint8_t t192 = round((temperature / 255.0) * 191);

    // calculate ramp up from
    uint8_t heatramp = t192 & 0x3F; // 0..63
    heatramp <<= 2; // scale up to 0..252

    // figure out which third of the spectrum we're in:
    if( t192 > 0x80) {                     // hottest
        bar->set_LED_RGB(pixel, 31, 255, 255, heatramp);
    } 
    else if( t192 > 0x40 ) {             // middle
        bar->set_LED_RGB(pixel, 31, 255, heatramp, 0);
    } 
    else {                               // coolest
        bar->set_LED_RGB(pixel, 31, heatramp, 0, 0);
    }
}


/*
The first one (Cooling) indicates how fast a flame cools down. More cooling means shorter flames, and the recommended values are between 20 and 100. 50 seems the nicest.

The Second parameter (Sparking), indicates the chance (out of 255) that a spark will ignite. A higher value makes the fire more active. Suggested values lay between 50 and 200, with my personal preference being 120.

The last parameter (SpeedDelay) allows you to slow down the fire activity … a higher value makes the flame appear slower.
You’ll have to play a little with that, but personally I like a value between 0 and 20.

*/
void pattern::sparkfire(int32_t cooling = 55, int32_t sparking = 120, int32_t speed_delay = 15) {
    int32_t led_len = bar->get_LED_len();
    int32_t led_width = bar->get_width();
    int32_t led_height = bar->get_height();

    int32_t cooldown;

    vector<uint8_t> heat(led_width);

    srand(time(0));
    while (true) {

        for (int i = 0; i < led_width; i++) {
            cooldown = rand() % (((cooling * 10) / led_width) + 2);

            if (cooldown > heat[i]) {
                heat[i] = 0;
            } 
            else {
                heat[i] = heat[i] - cooldown;
            }
        }

        for(int k= led_width - 1; k >= 2; k--) {
            heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
        }

        if( (rand() % 255) < sparking ) {
            int y = rand() % 7;
            heat[y] = heat[y] + (160 + rand() % 95);
            //heat[y] = random(160,255);
        }
        
        for( int j = 0; j < led_width; j++) {
            set_pixel_heat_color(bar, j, heat[j]);
            set_pixel_heat_color(bar, led_len - led_height - j - 1, heat[j]);
        }                                        
        bar->write_to_LEDs();
        this_thread::sleep_for(std::chrono::milliseconds(speed_delay));
    }

}

void pattern::lakers_theme(int speed) {
    int32_t led_len = bar->get_LED_len();
    int32_t led_width = bar->get_width();
    int32_t led_height = bar->get_height();
    int32_t delay;


    delay = map_one_range_to_another(speed, 0, 65, 100, 10, 3);
    if (speed <= 0) delay = -1;
    if (speed > 100) delay = 10;

    for (int k = 0; k < led_width + led_height; k++) {
        bar->set_LED_RGB(k, 31, 253, 185, 39);
        bar->set_LED_RGB(led_len - k - 1, 31, 85, 37, 100);
    }
    bar->write_to_LEDs();
    
    if (delay > 0)
        while (true) {
            bar->shift_all_once(1);
            this_thread::sleep_for(std::chrono::milliseconds(delay));
            bar->write_to_LEDs();
        }
}

void pattern::random_color(int count, int speed) {
    int32_t led_len = bar->get_LED_len();

    default_random_engine generator;
    uniform_int_distribution<int> distribution(0, led_len);
    uniform_int_distribution<int> color_distribution(0, led_len);
    
    int32_t delay;


    delay = map_one_range_to_another(speed, 0, 3000, 100, 10, 3);
    if (speed <= 0) delay = 3000;
    if (speed > 100) delay = 10;

    while (true) {
        for (int i = 0; i < 2 * count; i++) {
            bar->set_LED_HSV(distribution(generator), 31, rand() % 255, rand() % 100, 100);
            bar->write_to_LEDs();
            this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
        // bar->backup();

        this_thread::sleep_for(std::chrono::milliseconds(10000));

        // for (int rainbow_idx = 0; rainbow_idx < 8; rainbow_idx++) {
        //     // Flash color
        //     for (int i = 0; i < led_len; i++) {
        //         bar->set_LED_HSV(i, 31, rainbow_hue[rainbow_idx], 100, 100); 
        //     }
        //     bar->write_to_LEDs();
        //     this_thread::sleep_for(std::chrono::milliseconds(250));   
        // }
     
        // bar->restore();
        for (int i = 0; i < 2 * count; i++) {
            bar->set_LED_HSV(distribution(generator), 31, 0, 0, 0);
            bar->write_to_LEDs();
            this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
    }
}

double map(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double clamp(double input, double min, double max) {
    if (input <= min) return min;
    else if (input >= max) return max;
    else return input;
}

void pattern::gaussian(double mean, double variance, uint8_t h, uint8_t s, uint8_t v) {
    int32_t led_len = bar->get_LED_len();
    int32_t led_width = bar->get_width();
    int32_t led_height = bar->get_height();
    float zero_val;

    for (int i = -10; i < 11; i++) {
        double curr = (1 / (variance * sqrt(2 * M_PI))) * exp(-0.5 * pow((i - mean) / variance, 2.0));
        if (curr > 0.01)
            zero_val = i;
    }

    cout<< zero_val<< endl;
    for (int i = 0; i < led_width; i++) {
        double pdf_gaussian = (1 / (variance * sqrt(2 * M_PI))) * exp(-0.5 * pow((map(i, 0, led_width, -zero_val - 0.5, zero_val + 0.5) - mean) / variance, 2.0));
        double max_gaussian = (1 / (variance * sqrt(2 * M_PI))) * exp(-0.5 * pow(0, 2.0));

        float val = clamp(map(pdf_gaussian, 0.01, max_gaussian, 0, 100), 0, 100);
        bar->set_LED_HSV(i, 31, h, s, val);
        bar->set_LED_HSV(led_len - led_height - i - 1, 31, h, s, val);
    }              
    bar->write_to_LEDs();

}


