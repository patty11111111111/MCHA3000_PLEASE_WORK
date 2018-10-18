#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "potentiometer.h"
#include "encoder.h"
#include "stm32f4xx_hal.h"

#define TIMERPERIOD 100
#include "log_data.h"

typedef enum {
    LOG_POT,
    LOG_ENC
} LOG_DEVICE_T;

static uint32_t _n_samples;
static LOG_DEVICE_T _device;
static float _time;
static osTimerId _log_dataID = NULL;

void log_data_cmd(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("%s: expecting 2 arguments, syntax is: %s [pot|enc] <samples>\n", argv[0], argv[0]);
        return;
    }

	if (_log_dataID == NULL)
	{
	osTimerDef(log_data_name, log_data_task);
	_log_dataID = osTimerCreate(osTimer(log_data_name), osTimerPeriodic, log_data_task);        
	}
	
		osTimerStart(_log_dataID, 10);
	
    int n = atoi(argv[2]);
    if (n < 0)
    {
        printf("%s: expecting positive number of samples\n", argv[0]);
        return;
    }
    _n_samples = n;
    _time = 0.0f;

    if (!strcmp(argv[1], "pot"))
    {
        _device = LOG_POT;
        pot_init();
        printf("Time [sec],Potentiometer [V]\n");

    }
    else if (!strcmp(argv[1], "enc"))
    {
        _device = LOG_ENC;
        encoder_init();
        printf("Time [sec],Encoder [-]\n");
    }
    else
    {
        printf("%s: unknown device \"%s\", syntax is: %s [pot|enc] <samples>\n", argv[0], argv[1], argv[0]);
        return;
    }

    // TODO: On first call, create the timer
    // Hint: statically initialise the timerid to NULL.
    //       If it's NULL, call osTimerCreate.

    // TODO: start the timer   
}

void log_data_task(void const *arg)
{
    switch (_device)
    {
    case LOG_POT:
        printf("%.2f,%g\n", _time, (float)pot_get_value()*3.3f/POT_FULL_SCALE);
        break;
    case LOG_ENC:
        printf("%.2f,%" PRId32 "\n", _time, encoder_get_count());
        break;
    default:
        printf("Invalid device\n");
		osTimerStop(_log_dataID);

        // TODO: stop the timer
        return;
    }
    _time += 0.01f;

    if (--_n_samples == 0)
    {
		osTimerStop(_log_dataID);
    }
}