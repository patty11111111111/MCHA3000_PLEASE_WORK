#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <stdint.h>

enum {
    POT_FULL_SCALE = (1<<12),
};

void     pot_init(void);
void     pot_deinit(void);
uint16_t pot_get_value(void);

#endif