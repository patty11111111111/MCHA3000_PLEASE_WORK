#ifndef LED_RGB_H
#define LED_RGB_H

#include <stdint.h>

void        led_rgb_init(void);
uint16_t    led_rgb_get_red(void);
uint16_t    led_rgb_get_green(void);
uint16_t    led_rgb_get_blue(void);
void        led_rgb_set_red(uint16_t R);
void        led_rgb_set_green(uint16_t G);
void        led_rgb_set_blue(uint16_t B);

#endif