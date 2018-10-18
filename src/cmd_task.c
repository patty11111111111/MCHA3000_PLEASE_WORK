#include <stdio.h>
#include "cmsis_os.h"
#include "cmd_line_buffer.h"

#include "cmd_task.h"

CLB_CREATE_STATIC(clb, 80);

static osThreadId _cmdTaskHandle = NULL;

static void CmdTask_function(void const * argument);
osThreadDef(cmdTask, CmdTask_function, osPriorityBelowNormal, 1, 0x100);

static uint8_t _is_init = 0;

void cmd_task_init(void)
{
    clb_init(&clb);

    if (!_is_init)
    {
        _cmdTaskHandle = osThreadCreate(osThread(cmdTask), NULL);
        
        _is_init = 1;
    }
}

void CmdTask_function(void const *argument)
{
    printf("Hello world!\n");

    clb_init(&clb);

    while(1)
    {
        clb_process(&clb);
    }
}