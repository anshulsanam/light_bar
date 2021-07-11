#include <iostream>
#include <algorithm>
#include <vector>

#include <chrono>
#include <thread>
#include <unistd.h>


#include "led_bar.h"
#include "color.h"
#include "pattern.h"

using namespace std;


int main() {    

    led_bar bar(66, 6);
    pattern led_bar_patterns(&bar);
    // led_bar_patterns.scanning_rainbow();
    // led_bar_patterns.rotating_rainbow();
    // led_bar_patterns.scanning_effect(2, 1, 2, 74, 100);
    // led_bar_patterns.strobe_effect(50, 0, 0, 100);
    // led_bar_patterns.lakers_theme(0);
    // led_bar_patterns.sparkfire(30, 160, 20);
    // led_bar_patterns.random_color(bar.get_LED_len(), 5);
    // led_bar_patterns.gaussian(0, 1, 29, 83, 100);
  

    return 0;
} 