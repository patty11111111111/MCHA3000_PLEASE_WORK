#include "unity_fixture.h"
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "led_rgb.h"

TEST_GROUP(LED_RGB);

TEST_SETUP(LED_RGB)
{
}

TEST_TEAR_DOWN(LED_RGB)
{
    UnityPointer_UndoAllSets();
}

TEST(LED_RGB, PinConfig)
{
    // led_deinit();
    UT_PTR_SET(HAL_TIM_PWM_Init, HAL_TIM_PWM_Init_spyHandle);
    UT_PTR_SET(HAL_TIM_PWM_ConfigChannel, HAL_TIM_PWM_ConfigChannel_spyInitTypeDef);
    UT_PTR_SET(HAL_GPIO_Init, HAL_GPIO_Init_spyInitTypeDef);
    led_rgb_init();
    TEST_ASSERT_MESSAGE(GPIO_PIN_6 == (GPIO_Init_Spy.Pin & GPIO_PIN_6), "Expected GPIO to initialise Pin 6");
    TEST_ASSERT_MESSAGE(GPIO_PIN_8 == (GPIO_Init_Spy.Pin & GPIO_PIN_8), "Expected GPIO to initialise Pin 8");
    TEST_ASSERT_MESSAGE(GPIO_PIN_9 == (GPIO_Init_Spy.Pin & GPIO_PIN_9), "Expected GPIO to initialise Pin 9");
    TEST_ASSERT_MESSAGE(GPIO_MODE_AF_PP == GPIO_Init_Spy.Mode, "Expected GPIO mode to be 'Alternate Function Push Pull Output'");
    TEST_ASSERT_MESSAGE(GPIO_PULLUP == GPIO_Init_Spy.Pull, "Expected LED pullup resistors to be enabled");
    TEST_ASSERT_MESSAGE(GPIO_AF2_TIM3 == GPIO_Init_Spy.Alternate, "Expected GPIO alternate function for Timer 3");
}

TEST(LED_RGB, TimerConfig)
{
    UT_PTR_SET(HAL_TIM_PWM_Init, HAL_TIM_PWM_Init_spyHandle);
    UT_PTR_SET(HAL_TIM_PWM_ConfigChannel, HAL_TIM_PWM_ConfigChannel_spyInitTypeDef);
    led_rgb_init();
    TEST_ASSERT_MESSAGE(TIM3 == timerSpy.Instance,"Expected timer 3 instance");
    TEST_ASSERT_MESSAGE(1 == timerSpy.Init.Prescaler,"Expected prescaler of 1");
    TEST_ASSERT_MESSAGE(TIM_COUNTERMODE_UP == timerSpy.Init.CounterMode,"Expected timer to count up");
    TEST_ASSERT_MESSAGE(65535 == timerSpy.Init.Period,"Expected timer period of 65535");
    TEST_ASSERT_MESSAGE(0 == timerSpy.Init.ClockDivision, "Expected timer clock divider of 0");

    if(timerOCInitSpy.OCMode == TIM_OCMODE_PWM2)
    {
        TEST_ASSERT_MESSAGE(TIM_OCPOLARITY_HIGH == timerOCInitSpy.OCPolarity,"Expected output compare polarity to be high for PWM mode 2");
    }
    else if(timerOCInitSpy.OCMode == TIM_OCMODE_PWM1)
    {
        TEST_ASSERT_MESSAGE(TIM_OCPOLARITY_LOW == timerOCInitSpy.OCPolarity,"Expected output compare polarity to be high for PWM mode 1");
    }
    else
    {
        TEST_FAIL_MESSAGE("Expected output compare mode to be set to PWM mode 1 or 2");
    }
    TEST_ASSERT_EQUAL_UINT32(TIM_OCFAST_DISABLE, timerOCInitSpy.OCFastMode);
}

TEST(LED_RGB, DefaultBrightness)
{
    led_rgb_init();

    TEST_ASSERT_EQUAL_UINT16(0, led_rgb_get_red());
}

TEST(LED_RGB, SetBrightness)
{
    led_rgb_init();

    led_rgb_set_red(1234);
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(1234, led_rgb_get_red(),"Expected red to set/get compare register value timer channel 1");
    led_rgb_set_green(4321);
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(4321, led_rgb_get_green(),"Expected green to set/get compare register value timer channel 3");
    led_rgb_set_blue(1324);
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(1324, led_rgb_get_blue(),"Expected blue to set/get compare register value timer channel 4");
}

TEST_GROUP_RUNNER(LED_RGB)
{
    RUN_TEST_CASE(LED_RGB, PinConfig);
    RUN_TEST_CASE(LED_RGB, TimerConfig);
    RUN_TEST_CASE(LED_RGB, DefaultBrightness);
    RUN_TEST_CASE(LED_RGB, SetBrightness);
}