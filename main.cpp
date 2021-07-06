#include <iostream>
#include <algorithm>
#include <vector>

#include <chrono>
#include <thread>
#include <unistd.h>


#include "spi.h"
#include "led_bar.h"
#include "color.h"

using namespace std::chrono;
using namespace std;


int main() {
    int fd;
    uint8_t start[4] = {0x0, 0x0, 0x0, 0x0};
    uint8_t test_payload[4] = {0xE0, 0x00, 0x00, 0x00};
    uint8_t finish[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    

    led_bar bar(66, 6);


    // int32_t led_len = bar.get_LED_len();
    // int32_t i = 0;

    // while (true) {
    //     for (int k = 0; k < 66; k++) {
    //         bar.set_LED_HSV(k, 31, (i + static_cast<int>((k / static_cast<float>(led_len)) * 255)) % 255, 255, 255);
    //         bar.set_LED_HSV(143 - 6 - k, 31, (i + static_cast<int>((k / static_cast<float>(led_len)) * 255)) % 255, 255, 255);
    //     }

    //     bar.write_to_LEDs();
    //     i++;
    //     this_thread::sleep_for(std::chrono::milliseconds(5));
    // }

    // for (int k = 0; k < led_len; k++) {
    //     bar.set_LED_HSV(k, 31, static_cast<int>((k / static_cast<float>(led_len)) * 255), 255, 255);
    // }
    // bar.write_to_LEDs();

  

    return 0;
} 