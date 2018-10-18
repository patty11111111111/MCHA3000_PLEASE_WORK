#include "unity_fixture.h"

#include <stdint.h>
#include "button.h"
#include "stm32f4xx_hal.h"

TEST_GROUP(Button);

TEST_SETUP(Button)
{
    button_init();
}

TEST_TEAR_DOWN(Button)
{
}

TEST(Button, DefaultPressed)
{
    TEST_ASSERT_EQUAL_UINT8(0, button_get_pressed());
}

IGNORE_TEST(Button, InterruptPressed)
{
	button_isr();
    // TODO: Check the signal
	TEST_ASSERT_EQUAL_UINT8(1, button_get_pressed());
}

TEST(Button, PopPressed)
{
    button_set_pressed();
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(1, button_pop_pressed(), "Expected pop to return flag when it is set");
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, button_get_pressed(), "Expected pop to clear flag");
}


TEST_GROUP_RUNNER(Button)
{
    RUN_TEST_CASE(Button, DefaultPressed);
    RUN_TEST_CASE(Button, InterruptPressed);
    RUN_TEST_CASE(Button, PopPressed);
}