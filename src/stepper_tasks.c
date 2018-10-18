#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

#include "stepper_task.h"

#define TIMERPERIOD 2000

static TIM_HandleTypeDef   _htim2;
static TIM_OC_InitTypeDef  _sConfigPWM;
static osTimerId _stepperTimerID;

static uint8_t _is_running = 0;

static void _heartbeat_update(void const *arg);
    
static uint8_t _is_init = 0;

void stepper_task_init(void)
{
    if (!_is_init)
    {
        /* Configure Timer2 to generate single PWM output on GPIOA5             */
        /* NOTE: HAL_TIM_PWM_MspInit() is now located in stm32f4xx_hal_msp.c    */
        _htim2.Instance = TIM3;
        _htim2.Init.Prescaler = 1;
        _htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
        _htim2.Init.Period = TIMERPERIOD;
        _htim2.Init.ClockDivision = 0;

        _sConfigPWM.OCMode = TIM_OCMODE_PWM1;
        _sConfigPWM.Pulse = 0;
        _sConfigPWM.OCPolarity = TIM_OCPOLARITY_HIGH;
        _sConfigPWM.OCFastMode = TIM_OCFAST_DISABLE;

        HAL_TIM_PWM_Init(&_htim2);
        HAL_TIM_PWM_ConfigChannel(&_htim2, &_sConfigPWM, TIM_CHANNEL_1);

        /* Enable clock for Timer2 */
        HAL_TIM_PWM_Start(&_htim2, TIM_CHANNEL_1);

        /*  TODO:   Define an osTimer for the heartbeat using osTimerDef().
                -   name        heartbeatTimer_name
                -   function    _heartbeat_update                               
            TIP: Read the CMSIS-RTOS v1 Timer Documentation: 
            https://www.keil.com/pack/doc/CMSIS/RTOS/html/group__CMSIS__RTOS__TimerMgmt.html */
	osTimerDef(heartbeatTimer_name, _heartbeat_update);
	_heartbeatTimerID = osTimerCreate(osTimer(heartbeatTimer_name), osTimerPeriodic, NULL);        

        /*  TODO:   Create the timer for the osTimer 'heartbeatTimer_name'
                    using osTimerCreate().
                -   Store the id in _heartbeatTimerID
                -   the timer_def is found by calling osTimer(heartbeatTimer_name)
                -   Configure it to be periodic
                -   Give it a NULL input argument
            TIP:    _heartbeatTimerID is instantiated for you at the top of 
                    this file. Don't instantiate it twice               */

        _is_init = 1;
    }

    heartbeat_task_start();
}

void stepper_task_start(void)
{
    if (!_is_running)
    {
		osTimerStart(_heartbeatTimerID, 10);
        /* TODO: Start the heartbeat osTimer running at 10ms*/ 
        _is_running = 1;
    }
}

void stepper_task_stop(void)
{
    if (_is_running)
    {		osTimerStop(_heartbeatTimerID);

        /* TODO: Stop the heartbeat osTimer */
        _is_running = 0;
    }
}

uint8_t stepper_task_is_running(void)
{
    return _is_running;
}

void _heartbeat_update(void const *arg)
{
    static uint8_t  heartbeat_flag  = 0;
    static int      brightness      = 0;
    
    // Choose whether to increment/decrement
    if(brightness == TIMERPERIOD)
    {
        heartbeat_flag = 0;
    }
    else if(brightness == 0)
    {
        heartbeat_flag = 1;
    }

    // Increment/decrement the brightness of the onboard LED
    if(heartbeat_flag)
    {
        __HAL_TIM_SET_COMPARE(&_htim2, TIM_CHANNEL_1, brightness+=5);
    }
    else
    {
        __HAL_TIM_SET_COMPARE(&_htim2, TIM_CHANNEL_1, brightness-=5);
    }
}

