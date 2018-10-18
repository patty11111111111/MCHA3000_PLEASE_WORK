#include "stm32f4xx_hal.h"
#include "led_rgb.h"

TIM_HandleTypeDef   htim3;
TIM_OC_InitTypeDef  sConfigPWM;

static uint8_t _is_init = 0;

void led_rgb_init(void)
{
    if (!_is_init) // Guards against multiple initialisations
    {/* TODO:    Move this GPIO Initialisation to HAL_TIM_PWM_MspInit();
                and complete the TODO's
        ----------------------------------------------------------  */

    /* TODO:    Enable Timer 3 clock >> */
	//__HAL_RCC_TIM3_CLK_ENABLE();
    //__HAL_RCC_GPIOC_CLK_ENABLE();

    /* TODO:    Configure PC6, PC8, PC9 with
                - AF pushpull mode >>
                - TIM3 alternate function >>>
                - Pullup resistors enabled ???                           */
    /*GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Alternate = GPIO_AF2_TIM3;
	*/
	
   // HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    /*  ----------------------------------------------------------  */

    /* TODO:    Configure Timer3 to generate three PWM outputs on 
                PC6(TIM3_CH1), PC8(TIM3_CH3), PC9(TIM3_CH4) with
                - Prescaler of one >>
                - Counting up >>
                - Period of 65535 (maximum 16 bit value)   
                - Clock division of zero                            */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 1; /* set prescaler to 1 */
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP; /* set counter mode to up */
	htim3.Init.Period = 65535; /* set period to 65535 >> */
	htim3.Init.ClockDivision = 0; /* set clock division to 0 */
	
    /* TODO:    Configure the timers for a mode and polarity suitable
                for common anode LED and default to off >>            */
	sConfigPWM.OCMode = TIM_OCMODE_PWM1; /* timer output compare mode PWM1 */
	sConfigPWM.Pulse = 0; /* set pulse (duty cycle) to 0% initially */
	sConfigPWM.OCPolarity = TIM_OCPOLARITY_LOW; /* set polarity to high */
	sConfigPWM.OCFastMode = TIM_OCFAST_DISABLE; /* disable fast mode */
	
    /* TODO:    Initialise timer 3 in PWM mode */
    /* TODO:    Configure timer 3 channels 1, 3 and 4 in PWM mode */
    /* TODO:    Start timer 3 channels 1, 3 and 4 in PWM mode >> */
	HAL_TIM_PWM_Init(&htim3);
	
	HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigPWM, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigPWM, TIM_CHANNEL_3);
	HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigPWM, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
    // Set colours to off by default
        _is_init = 1;
    }

    led_rgb_set_red(0);
    led_rgb_set_green(0);
    led_rgb_set_blue(0);

}

uint16_t led_rgb_get_red(void) 
{
    return __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_1);
}
uint16_t led_rgb_get_green(void)
{
    return __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_3);
}
uint16_t led_rgb_get_blue(void)
{
    return __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_4);
}
void led_rgb_set_red(uint16_t R)
{
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, R);
}
void led_rgb_set_green(uint16_t G)
{
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, G);

}
void led_rgb_set_blue(uint16_t B)
{
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, B);
}

