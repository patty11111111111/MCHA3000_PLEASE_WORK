#include <stdint.h>
#include "light.h"

static uint16_t _brightness, _saturation, _hue;
static uint16_t _red, _green, _blue;

static void _light_hsv2rgb(void);

void light_init(void)
{
    _brightness = LIGHT_DEFAULT_BRIGHTNESS;
    _saturation = LIGHT_DEFAULT_SATURATION;
    _hue        = LIGHT_DEFAULT_HUE;
    _light_hsv2rgb();
}

void light_set_brightness(uint16_t brightness)
{
    if      (brightness >= LIGHT_MAX_BRIGHTNESS)    _brightness = LIGHT_MAX_BRIGHTNESS;
    else if (brightness <= LIGHT_MIN_BRIGHTNESS)    _brightness = LIGHT_MIN_BRIGHTNESS;
    else                                            _brightness = brightness;

    _light_hsv2rgb();
}

uint16_t light_get_brightness(void)
{
    return _brightness;
}

void light_set_saturation(uint16_t saturation)
{
    if      (saturation >= LIGHT_MAX_SATURATION)    _saturation = LIGHT_MAX_SATURATION;
    else if (saturation <= LIGHT_MIN_SATURATION)    _saturation = LIGHT_MIN_SATURATION;
    else                                            _saturation = saturation;

    _light_hsv2rgb();
}

uint16_t light_get_saturation(void)
{
    return _saturation;
}

void light_set_hue(uint16_t hue)
{
    _hue = hue % (360*LIGHT_HUE_DEGREE);
    _light_hsv2rgb();
}

void light_incr_hue(int16_t deltahue)
{
    enum {N = 360*LIGHT_HUE_DEGREE};
	if((deltahue < 0) && (_hue + deltahue) < 0)
	{
		_hue = (360*LIGHT_HUE_DEGREE + (_hue + deltahue) )% N;
	}
	else
	{
	_hue = (_hue + deltahue) % N;
	}
    _light_hsv2rgb();

}


uint16_t light_get_hue(void)
{
    return _hue;
}

uint16_t light_get_red(void)
{
    return _red;
}

uint16_t light_get_green(void)
{
    return _green;
}

uint16_t light_get_blue(void)
{
    return _blue;
}

void _light_hsv2rgb(void)
{
    enum {N = 60*LIGHT_HUE_DEGREE}; // Size of each sextant

    // uint16_t min = _brightness - (_brightness*_saturation >> 16); // Loss of precision
    uint16_t min = ((_brightness << 16) - _brightness*_saturation) >> 16;
    uint16_t max = _brightness;
    uint8_t  sextant = _hue/N; // Sextant index
    uint16_t t = sextant & 1 ? (sextant + 1)*N - _hue : _hue - sextant*N; // interpolation factor (0 to N)
    uint16_t x = ((N - t)*min + t*max)/N; // LERP between min and max

    switch (sextant)
    {
    case 0:     // 0-60 deg (red to yellow)
        _red    = max;
        _green  = x;
        _blue   = min;
        break;
    case 1:     // 60-120 deg (yellow to green)
        _red    = x;
        _green  = max;
        _blue   = min;
        break;
    case 2:     // 120-180 deg (green to cyan)
        _red    = min;
        _green  = max;
        _blue   = x;
        break;
    case 3:     // 180-240 deg (cyan to blue)
        _red    = min;
        _green  = x;
        _blue   = max;
        break;
    case 4:     // 240-300 deg (blue to magenta)
        _red    = x;
        _green  = min;
        _blue   = max;
        break;
    case 5:     // 300-360 deg (magenta to red)
        _red    = max;
        _green  = min;
        _blue   = x;
        break;
    default:    // Something went horribly wrong
        _red    = 0;
        _green  = 0;
        _blue   = 0;
        break;
    }
}
