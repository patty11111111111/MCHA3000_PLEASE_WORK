#ifndef LIGHT_H
#define LIGHT_H

#include <stdint.h>

enum {
    LIGHT_MIN_BRIGHTNESS        = 0,
    LIGHT_MAX_BRIGHTNESS        = UINT16_MAX,
    LIGHT_DEFAULT_BRIGHTNESS    = UINT16_MAX/2,
    LIGHT_MIN_SATURATION        = 0,
    LIGHT_MAX_SATURATION        = UINT16_MAX,
    LIGHT_DEFAULT_SATURATION    = UINT16_MAX,
    LIGHT_HUE_DEGREE            = 128,  // Value of 1 degree of hue. Should be divisible by 4 and 360*LIGHT_HUE_DEGREE < UINT16_MAX
    LIGHT_DEFAULT_HUE           = 0*LIGHT_HUE_DEGREE,
};

void        light_init(void);

void        light_set_brightness(uint16_t);
uint16_t    light_get_brightness(void);

void        light_set_saturation(uint16_t);
uint16_t    light_get_saturation(void);

void        light_set_hue(uint16_t);
void        light_incr_hue(int16_t);
uint16_t    light_get_hue(void);

uint16_t    light_get_red(void);
uint16_t    light_get_green(void);
uint16_t    light_get_blue(void);

#endif