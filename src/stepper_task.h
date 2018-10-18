#ifndef STEPPER_TASK_H
#define STEPPER_TASK_H

#include <stdint.h>

void    stepper_task_init(void);
void    stepper_task_start(void);
void    stepper_task_stop(void);
uint8_t stepper_task_is_running(void);
void stepper_update(void const *arg);
void stepper_period(uint32_t period);
uint32_t stepper_period_get(void);
void stepper_dir(void);


#endif
