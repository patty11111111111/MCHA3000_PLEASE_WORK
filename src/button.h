#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

void    button_init(void);
void 	buttonTask_function(void const *argument);
void    button_clear_pressed(void);
uint8_t button_get_pressed(void);
void    button_set_pressed(void);
uint8_t button_pop_pressed(void);
void 	button_isr(void);

#endif