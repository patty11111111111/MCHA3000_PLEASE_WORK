#ifndef DIMMER_H
#define DIMMER_H
#include "cmsis_os.h"

void        dimmer_task_init(void);
void        dimmer_task_start(void);
void        dimmer_task_stop(void);
uint8_t     dimmer_task_is_running(void);

#endif