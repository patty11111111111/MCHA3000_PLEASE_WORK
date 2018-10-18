#include "unity_fixture.h"
#include "iospy.h"
#include <stdint.h>
#include "cmd_line_buffer.h"
#include "controller.h"

CLB_CREATE_STATIC(clb, 80);

TEST_GROUP(CmdCtrl);

TEST_SETUP(CmdCtrl)
{
    clb_init(&clb);
    ctrl_init();
}

TEST_TEAR_DOWN(CmdCtrl)
{
}

TEST(CmdCtrl, SetRefSilent)
{
    char out[80];

    iospy_hook();
    iospy_push_in_str("set ref 3.14\n");
    clb_process(&clb);
    size_t n = iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    TEST_ASSERT_EQUAL_STRING("", out);
    TEST_ASSERT_EQUAL_UINT_MESSAGE(0, n, "Expected no output");
}

TEST(CmdCtrl, SetMeasSilent)
{
    char out[80];

    iospy_hook();
    iospy_push_in_str("set meas 1.23 4.56\n");
    clb_process(&clb);
    size_t n = iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    TEST_ASSERT_EQUAL_STRING("", out);
    TEST_ASSERT_EQUAL_UINT_MESSAGE(0, n, "Expected no output");
}

TEST(CmdCtrl, CtrlReturnsValue)
{
    char out[1024];

    iospy_hook();
    iospy_push_in_str("ctrl\n");
    clb_process(&clb);
    iospy_pop_out_str(out, sizeof(out));
    iospy_unhook();

    // Expected reply format
    const char * fmt = "%g\n";

    float y_actual;
    char msg[1024];
    sprintf(msg, "Attempted to scan '%s' using format '%s'", out, fmt);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, sscanf(out, fmt, &y_actual), msg);
}

TEST_GROUP_RUNNER(CmdCtrl)
{
    RUN_TEST_CASE(CmdCtrl, SetRefSilent);
    RUN_TEST_CASE(CmdCtrl, SetMeasSilent);
    RUN_TEST_CASE(CmdCtrl, CtrlReturnsValue);
}