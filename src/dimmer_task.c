#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "button.h"
#include "led_rgb.h"
#include "light.h"
#include "encoder.h"
#include "potentiometer.h"

#include "dimmer_task.h"

static osThreadId _dimmer_task_thread_id;

static void _dimmer_task_update(void const *argument);
osThreadDef(dimmerTask, _dimmer_task_update, osPriorityNormal, 1, 128);

static uint8_t _is_running = 0;

static uint8_t _is_init = 0;

void dimmer_task_init(void)
{
    if (!_is_init)
    {
		led_rgb_init();
        light_init();
        button_init();
        encoder_init(); 
        pot_init();
        _is_running = 0;

        _is_init = 1;   
    }
    
    dimmer_task_start();
}

void dimmer_task_start(void)
{
    if (!_is_running)
    {       
        _dimmer_task_thread_id = osThreadCreate(osThread(dimmerTask), NULL);
        _is_running = 1;
    }
}

void dimmer_task_stop(void)
{
    if (_is_running)
    {
        osThreadTerminate(_dimmer_task_thread_id);
        _is_running = 0;
    }
}

uint8_t dimmer_task_is_running(void)
{
    return _is_running;
}

void _dimmer_task_update(void const *argument)
{
    UNUSED(argument);
    while(1)
    {
        // Read potentiometer and set brightness
        light_set_brightness(pot_get_value());
        
        // Read encoder and set hue
        int32_t count = encoder_pop_count();
        light_incr_hue(count*15*LIGHT_HUE_DEGREE/4);
        // Note: Each quadrature transition is 3.75 deg
        
        // Read button and toggle colour
        if (button_pop_pressed())
        {
            // Toggle between 0% and 100% saturation
            light_set_saturation(LIGHT_MAX_SATURATION - light_get_saturation());
        }

        // Update LED brightness
        led_rgb_set_red(light_get_red());
        led_rgb_set_green(light_get_green());
        led_rgb_set_blue(light_get_blue());

        osDelay(100);
    }
}

