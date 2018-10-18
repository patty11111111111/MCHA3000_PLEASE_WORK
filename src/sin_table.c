#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

#define TIMERPERIOD 500
#include "sin_table.h"

static uint32_t _n_samples;
//static uint32_t first_time = 1;
static float _time, _amplitude, _frequency;
//static TIMERPERIOD = 1; 
static float s = 0.0; 
static osTimerId _sin_tableID = NULL;

void sin_table_cmd(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("%s: expecting 3 arguments, syntax is: %s <samples> <amplitude> <frequency>\n", argv[0], argv[0]);
        return;
    }

    _n_samples = atoi(argv[1]);
    _amplitude = atof(argv[2]);
    _frequency = atof(argv[3]);
    _time      = 0.0f;

    printf("Time [sec],Value\n");
    
	if (_sin_tableID == NULL)
	{
		
	osTimerDef(sin_table_name, sin_table_task);
	_sin_tableID = osTimerCreate(osTimer(sin_table_name), osTimerPeriodic, sin_table_task);        
	}
		osTimerStart(_sin_tableID, 10);

	// TODO: On first call, create the timer
    // Hint: statically initialise the timerid to NULL.
    //       If it's NULL, call osTimerCreate.
	
    // TODO: start the timer
}

void sin_table_task(void const *arg)
{
    // TODO: calculate and print one row of data
    // Hint: Use sinf, not sin
    
	s = _amplitude*sinf(_time*2*3.141592*_frequency);
	
	printf("%g,%g\n", _time, s);
	_time += 0.01f;
    if (--_n_samples == 0)
    {
		osTimerStop(_sin_tableID);
    }	
	
}