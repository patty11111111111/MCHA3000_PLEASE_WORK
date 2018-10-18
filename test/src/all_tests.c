#include "unity_fixture.h"

static void RunAllTests(void)
{
    // Lab 2
    RUN_TEST_GROUP(LED_RGB);
    RUN_TEST_GROUP(Button);
    RUN_TEST_GROUP(Encoder);
    RUN_TEST_GROUP(Potentiometer);
    RUN_TEST_GROUP(Light);

    // Lab 3
    RUN_TEST_GROUP(CmdLineBuffer);
    RUN_TEST_GROUP(IOSpy);
    RUN_TEST_GROUP(CmdParse);
    RUN_TEST_GROUP(CmdProcess);
    RUN_TEST_GROUP(CmdEnc);
    RUN_TEST_GROUP(CmdPot);
    RUN_TEST_GROUP(CmdLight);
    RUN_TEST_GROUP(CmdDimmer);
    RUN_TEST_GROUP(CmdHeartbeat); 

    // Lab 4
    RUN_TEST_GROUP(CmdXY);
    RUN_TEST_GROUP(Ctrl);
    RUN_TEST_GROUP(CmdCtrl);

    // Lab 5
    RUN_TEST_GROUP(SinTable);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}