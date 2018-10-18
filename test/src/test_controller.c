#include "unity_fixture.h"

#include <stdint.h>
#include "controller.h"

TEST_GROUP(Ctrl);

TEST_SETUP(Ctrl)
{
    ctrl_init();
}

TEST_TEAR_DOWN(Ctrl)
{
}

TEST(Ctrl, ZeroInitialState)
{
    TEST_ASSERT_EACH_EQUAL_FLOAT(0.0f, ctrl_get_state(), CTRL_N_STATE);
}

TEST(Ctrl, SetState)
{
    float state_expected[CTRL_N_STATE];
    for (size_t i = 0; i < CTRL_N_STATE; ++i)
        state_expected[i] = 1.2f*i;

    ctrl_set_state(state_expected);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(state_expected, ctrl_get_state(), CTRL_N_STATE);
}

TEST(Ctrl, OneStepAx)
{
    float x0[CTRL_N_STATE] = {0.1, 0.1, 0.1, 0.1};
    float u[CTRL_N_INPUT] = {0, 0, 0};

    ctrl_set_state(x0);
    ctrl_run(u);
    float * x_actual = ctrl_get_state();

    float x_expected[CTRL_N_STATE] = {9.5230, -1.3797, 0.13583, 0.09344};
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(x_expected, x_actual, CTRL_N_STATE);
}

TEST(Ctrl, OneStepBu)
{
    float x0[CTRL_N_STATE] = {0, 0, 0, 0};
    float u[CTRL_N_INPUT] = {1, -2, 0};

    ctrl_set_state(x0);
    ctrl_run(u);
    float * x_actual = ctrl_get_state();

    float x_expected[CTRL_N_STATE] = {-31.50838, 5.60102, -0.5844, 0.19711};
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(x_expected, x_actual, CTRL_N_STATE);
}

TEST(Ctrl, OutputCx)
{
    float x0[CTRL_N_STATE] = {0.1, 0.1, 0.1, 0.1};
    float u[CTRL_N_INPUT] = {0, 0, 0};

    ctrl_set_state(x0);

    float y_expected[CTRL_N_OUTPUT] = {1.18558};
    float * y_actual = ctrl_run(u);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(y_expected, y_actual, CTRL_N_OUTPUT);
}

TEST(Ctrl, OutputDu)
{
    float x0[CTRL_N_STATE] = {0, 0, 0, 0};
    float u[CTRL_N_INPUT] = {1, -2, 0};

    ctrl_set_state(x0);

    float y_expected[CTRL_N_OUTPUT] = {-3.10417};
    float * y_actual = ctrl_run(u);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(y_expected, y_actual, CTRL_N_OUTPUT);
}

TEST(Ctrl, RunSteps)
{
    float x0[CTRL_N_STATE] = {0.1, 0.1, 0.1, 0.1};

    float ref[]     = {0.100000, 0.100000, 0.10000000, 0.1000000, 0.1000000};
    float phi[]     = {0,	0.100000,	0.122990,	0.176708,	0.216027};
    float theta[]   = {0,	0.100000,	0.095468,	0.08988,	0.08859};

    enum {N_DATA = sizeof(ref)/sizeof(ref[0])};

    float y_expected[][CTRL_N_OUTPUT] = {{0.8752}, {-0.20885}, {0.120678}, {0.102451}, {0.066235}};

    float u[CTRL_N_INPUT];

    ctrl_set_state(x0);

    for (size_t i = 0; i < N_DATA; ++i)
    {
        u[0] = ref[i];
        u[1] = phi[i];
        u[2] = theta[i];
        float * y_actual = ctrl_run(u);
        TEST_ASSERT_EQUAL_FLOAT_ARRAY(&y_expected[i][0], y_actual, CTRL_N_OUTPUT);
    }
}

TEST_GROUP_RUNNER(Ctrl)
{
    RUN_TEST_CASE(Ctrl, ZeroInitialState);
    RUN_TEST_CASE(Ctrl, SetState);
    RUN_TEST_CASE(Ctrl, OneStepAx);
    RUN_TEST_CASE(Ctrl, OneStepBu);
    RUN_TEST_CASE(Ctrl, OutputCx);
    RUN_TEST_CASE(Ctrl, OutputDu);
    RUN_TEST_CASE(Ctrl, RunSteps);
}