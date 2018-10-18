#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "potentiometer.h"
#include "stm32f4xx_hal_adc.h"

ADC_HandleTypeDef hadc1;
ADC_ChannelConfTypeDef sConfigADC;

static uint8_t _is_init = 0;

void pot_init(void)
{
    if (!_is_init)
    {
    /* LAB2TODO: Configure ADC1 instance for:
       LAB2         - div2 prescaler,
       LAB2         - 12-bit resolution, 
       LAB2         - left data aligned, 
       LAB2         - continuous conversion mode,
       LAB2         - one conversion.
       LAB2 Note:   Config parameters of the ADC_InitTypeDef not mentioned in
       LAB2         list above are not required in this lab             */
    /* LAB2TODO: Configure the ADC to:
       LAB2         - use channel 0,
       LAB2         - sequence rank of 1,
       LAB2         - 480 cycle sample time,
       LAB2         - offset of 0.                                      */
    /* LAB2TODO: Initialise the ADC                     */  
    /* LAB2TODO: Initialise the ADC channel config      */
    /* LAB2TODO: Start the ADC                          */
	/* TODO: Configure ADC1 instance for:
                - div2 prescaler,
                - 12-bit resolution, 
                - left data aligned, 
                - continuous conversion mode,
                - one conversion.
        Note:   Config parameters of the ADC_InitTypeDef not mentioned in
                list above are not required in this lab             */

	hadc1.Instance 					= ADC1;
	hadc1.Init.ClockPrescaler 		= ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc1.Init.Resolution 			= ADC_RESOLUTION_12B;
	hadc1.Init.DataAlign			= ADC_DATAALIGN_LEFT;
	hadc1.Init.ContinuousConvMode	= ENABLE;
	hadc1.Init.NbrOfConversion		= 1;
	
	
				
    /* TODO: Configure the ADC to:
                - use channel 0,
                - sequence rank of 1,
                - 480 cycle sample time,
                - offset of 0.       				*/
	sConfigADC.Channel 		= ADC_CHANNEL_0;
	sConfigADC.Rank 		= 1;
	sConfigADC.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	sConfigADC.Offset 		= 0;
				
    /* TODO: Initialise the ADC                     */  
	HAL_ADC_Init(&hadc1);
	HAL_ADC_ConfigChannel(&hadc1,&sConfigADC);
	HAL_ADC_Start(&hadc1);
        _is_init = 1;
    }
}

void pot_deinit(void)
{
    HAL_ADC_DeInit(&hadc1);
    _is_init = 0;
}

uint16_t pot_get_value(void)
{
    uint16_t result = 0;
    /* TODO: Poll the ADC to complete a conversion  */
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1,1);
    /* TODO: Get and return the 12-bit result       */
    result = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);
    return result;
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    /* TODO: Enable ADC1 clock */
	__HAL_RCC_ADC1_CLK_ENABLE();
    /* TODO: Enable GPIOA clock */
	__HAL_RCC_GPIOA_CLK_ENABLE();  //enable clock
    /* TODO: Configure PA0 in analog mode, no pullup */

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_0; //set pull up mode tp pin 5
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG; //activate analog mode
	GPIO_InitStruct.Pull = GPIO_NOPULL; //no pull up activation 
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
}