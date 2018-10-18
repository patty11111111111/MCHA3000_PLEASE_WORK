#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

#include "button.h"
 
#define BUTTON_SIGNAL 0x002

static uint8_t _was_pressed = 0;    // Sticky flag
static uint8_t _is_init = 0;
osThreadId buttonTask_handle;

void button_init(void)
{
    _was_pressed = 0;

    if (!_is_init)  // Guards against multiple initialisations
    {
/* TODO:    Configure and initialise PC4 with
                - External interrupts on the rising edge
                - Pulldown resistors enabled                */
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_4;
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    /* TODO:    Enable GPIOC clock                          */
	__HAL_RCC_GPIOC_CLK_ENABLE();
    /* TODO:    Initialise GPIOC                            */
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);	
    /* TODO:    Set the priorities of EXTI4 to  
                - Preempt: 0x0F,    
                - Sub: 0x0F                                 */
	HAL_NVIC_SetPriority(EXTI4_IRQn,0x0F, 0x0F);
    /* TODO:    Enable the EXTI4 interrupt                  */
	HAL_NVIC_EnableIRQ (EXTI4_IRQn);
        _is_init = 1;
		
		    /* LAB2TODO:    Configure and initialise PC4 with
       LAB2         - External interrupts on the rising edge
       LAB2         - Pulldown resistors enabled                */
    /* LAB2TODO:    Enable GPIOC clock                          */
    /* LAB2TODO:    Initialise GPIOC                            */
    /* LAB2TODO:    Set the priorities of EXTI4 to  
       LAB2         - Preempt: 0x0F,    
       LAB2         - Sub: 0x0F                                 */
    /* LAB2TODO:    Enable the EXTI4 interrupt                  */
osThreadDef(buttonTask_name, buttonTask_function, osPriorityIdle, 1, 128);
    /*  TODO: Instantiate a thread for the button using osThreadDef().
            name - buttonTask_name
            thread - buttonTask_function
            priority - osPriorityIdle
            instances - 1
            stackz - 128       
        TIP:    Look at the tip box in the lab doc, the RTOS documentation, 
                and the implementation of dimmerTask)                       */
buttonTask_handle = osThreadCreate(osThread(buttonTask_name), NULL);
    /*  TODO: Create a thread for the osThread 'buttonTask_name' using osThreadCreate().
              - Store the id in buttonTask_handle
              - The thread_def is given by calling osThread(buttonTask_name)
              - Give it a NULL input argument
        TIP:    buttonTask_handle is instantiated for you at the top of this
                file. Don't instantiate it twice!)                          */
        
    }
}


void buttonTask_function(void const *argument)
{
    typedef enum {
        BUTTON_LOW,
        BUTTON_HIGH,
    } BUTTON_STATE_T;

    BUTTON_STATE_T state = BUTTON_LOW;  // Default state to LOW when thread is initailised

    UNUSED(argument);
    while(1)
    {
        switch (state)
        {
            case BUTTON_LOW:
			osSignalWait(BUTTON_SIGNAL,osWaitForever); 
			osDelay(30);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4) == GPIO_PIN_SET)
				{
					_was_pressed = 1;
				state = BUTTON_HIGH;
				
				}
			osSignalWait(BUTTON_SIGNAL, 0); 
            /* TODO: Implement state machine here */
            break;
            case BUTTON_HIGH:
            /* TODO: Implement state machine here */
			osSignalWait(BUTTON_SIGNAL,osWaitForever); 
			osDelay(50); //falling edge debounce
			
			if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4) == GPIO_PIN_RESET)
			{
				state = BUTTON_LOW;
			}
		
			osSignalWait(BUTTON_SIGNAL,0); 
			
            break;
        default:
            // Something went horribly wrong
            break;
        }
    }
}

void button_clear_pressed(void)
{
    _was_pressed = 0;
}

uint8_t button_get_pressed(void)
{
    return _was_pressed;
}

void button_set_pressed(void)
{
    _was_pressed = 1;
}

uint8_t button_pop_pressed(void)
{
    // Atomically read _was_pressed
    uint8_t was_pressed = _was_pressed;
    _was_pressed = 0;
    return was_pressed;
}

void button_isr(void)
{
	
    /* TODO: Set the osSignal 'BUTTON_SIGNAL' instead of the _was_pressed flag */
	osSignalSet(buttonTask_handle, BUTTON_SIGNAL);

}

void EXTI4_IRQHandler(void)
{
     /* TODO: Call the button isr */
	button_isr();
    /* TODO: Call the HAL GPIO EXTI IRQ Handler and specify the GPIO pin */
	HAL_GPIO_EXTI_IRQHandler (GPIO_PIN_4);
}