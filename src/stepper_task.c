#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

#include "stepper_task.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <inttypes.h> // For PRIxx and SCNxx macros

static TIM_HandleTypeDef   _htim_step;
static TIM_OC_InitTypeDef  _sConfigPWM;
static osTimerId _stepperTimerID;

static uint8_t _is_running = 0;
static uint8_t step_flag = 0; 
static uint8_t dir_flag = 0; 
//static void stepper_update(void const *arg);
    
static uint8_t _is_init = 0;
static uint32_t TIMERPERIOD = 1000;

void stepper_task_init(void)
{
   // if (!_is_init)
    //{
        /* Configure Timer2 to generate single PWM output on GPIOA5             */
        /* NOTE: HAL_TIM_PWM_MspInit() is now located in stm32f4xx_hal_msp.c    */
        _htim_step.Instance = TIM3;
        _htim_step.Init.Prescaler = 10000;
        _htim_step.Init.CounterMode = TIM_COUNTERMODE_UP;
        _htim_step.Init.Period = TIMERPERIOD;
        _htim_step.Init.ClockDivision = 0;

        _sConfigPWM.OCMode = TIM_OCMODE_PWM1;
        _sConfigPWM.Pulse = 0;
        _sConfigPWM.OCPolarity = TIM_OCPOLARITY_HIGH;
        _sConfigPWM.OCFastMode = TIM_OCFAST_DISABLE;

        HAL_TIM_PWM_Init(&_htim_step);
        HAL_TIM_PWM_ConfigChannel(&_htim_step, &_sConfigPWM, TIM_CHANNEL_3);
		        HAL_TIM_PWM_ConfigChannel(&_htim_step, &_sConfigPWM, TIM_CHANNEL_1);


        /* Enable clock for Timer2 */
        HAL_TIM_PWM_Start(&_htim_step, TIM_CHANNEL_3);
		        HAL_TIM_PWM_Start(&_htim_step, TIM_CHANNEL_1);


        /*  TODO:   Define an osTimer for the heartbeat using osTimerDef().
                -   name        heartbeatTimer_name
                -   function    _heartbeat_update                               
            TIP: Read the CMSIS-RTOS v1 Timer Documentation: 
            https://www.keil.com/pack/doc/CMSIS/RTOS/html/group__CMSIS__RTOS__TimerMgmt.html */
	osTimerDef(stepperTimer_name, stepper_update);
	_stepperTimerID = osTimerCreate(osTimer(stepperTimer_name), osTimerPeriodic, NULL);        

        /*  TODO:   Create the timer for the osTimer 'heartbeatTimer_name'
                    using osTimerCreate().
                -   Store the id in _heartbeatTimerID
                -   the timer_def is found by calling osTimer(heartbeatTimer_name)
                -   Configure it to be periodic
                -   Give it a NULL input argument
            TIP:    _heartbeatTimerID is instantiated for you at the top of 
                    this file. Don't instantiate it twice               */

       // _is_init = 1;
   // }
   
   
	osTimerStart(_stepperTimerID, 10);

  // stepper_task_start();
}

void stepper_dir(void)
{
	//GPIO-write(GPIOC, 6, 1)
	
	//GPIO-write(GPIOB, 15, 1)
	
	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
	dir_flag = !dir_flag; 
	//PC6_4 
	//PB15_26 
	printf("DIR FLAG: %d\n", dir_flag); 
}

void stepper_task_start(void)
{
    if (!_is_running)
   {
	osTimerStart(_stepperTimerID, 10);
        /* TODO: Start the heartbeat osTimer running at 10ms*/ 
        _is_running = 1;
    }
}

void stepper_period(uint32_t period)
{
    TIMERPERIOD = period; 
	stepper_task_init();

}
uint32_t stepper_period_get(void)
{
	return TIMERPERIOD;
}

void stepper_task_stop(void)
{
   // if (_is_running)
   // {		osTimerStop(_heartbeatTimerID);

        /* TODO: Stop the heartbeat osTimer */
   //     _is_running = 0;
   // }
}

uint8_t stepper_task_is_running(void)
{
    return _is_running;
}

void stepper_update(void const *arg)
{
	
		__HAL_TIM_SET_COMPARE(&_htim_step, TIM_CHANNEL_3, TIMERPERIOD/2);
		
		if(dir_flag == 0)
		{
		 __HAL_TIM_SET_COMPARE(&_htim_step, TIM_CHANNEL_1, TIMERPERIOD);
		}
		
		if(dir_flag == 1)
		{
		 __HAL_TIM_SET_COMPARE(&_htim_step, TIM_CHANNEL_1, 0);

		}


	//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8, GPIO_PIN_SET);
	/*if (__HAL_TIM_GET_COMPARE(&htim_step, TIM_CHANNEL_3))
	{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8, GPIO_PIN_SET);
	}
	*/
	/*
	
	if (step_flag)
	{    __HAL_TIM_SET_COMPARE(&_htim_step, TIM_CHANNEL_3, TIMERPERIOD/2);

		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8, GPIO_PIN_RESET);
		step_flag = 0;}
		
	else {	);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8, GPIO_PIN_SET);
		step_flag  = 1; }*/
	
	//printf("We got here\n");
		//third argument is the total amount of "up time" so should 
}

