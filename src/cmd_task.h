#ifndef CMD_TASK_H
#define CMD_TASK_H

#include "cmsis_os.h"

void cmd_task_init(void);
osThreadId cmd_task_get_thread_ID(void);

#endif