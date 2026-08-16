#include "FreeRTOS.h"
#include "task.h"

extern "C"
{
void vApplicationMallocFailedHook()
{
    configASSERT(pdFALSE);
}

void vApplicationStackOverflowHook(TaskHandle_t task, char *task_name)
{
    static_cast<void>(task);
    static_cast<void>(task_name);
    configASSERT(pdFALSE);
}

void _init()
{
}

void _fini()
{
}
}
