#include "unity_fixture.h"

#include <stdint.h>
#include "potentiometer.h"
#include "stm32f4xx_hal.h"

TEST_GROUP(Potentiometer);

TEST_SETUP(Potentiometer)
{
    // pot_init();
}

TEST_TEAR_DOWN(Potentiometer)
{
    UnityPointer_UndoAllSets();
    // pot_deinit();
}

TEST(Potentiometer, ADCconfig)
{
    UT_PTR_SET(HAL_ADC_Init, HAL_ADC_Init_spyHandle);
    pot_init();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(ADC_RESOLUTION_12B, ADC_Init_Spy.Init.Resolution, "Expected 12 bit ADC resolution");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(ADC_DATAALIGN_LEFT, ADC_Init_Spy.Init.DataAlign, "Expected to configure left aligned output");
}

TEST(Potentiometer, GetValueFromADC)
{
    uint16_t expected = 300;
    pot_init();
    mock_adc1.DR = 300;
    uint16_t actual = pot_get_value();
    TEST_ASSERT_EQUAL_UINT16(expected, actual);
}

TEST_GROUP_RUNNER(Potentiometer)
{
    RUN_TEST_CASE(Potentiometer, ADCconfig);
    RUN_TEST_CASE(Potentiometer, GetValueFromADC);
}