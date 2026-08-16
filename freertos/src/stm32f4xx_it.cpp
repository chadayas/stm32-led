#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_hal.h"

extern "C"
{
void xPortSysTickHandler();

void NMI_Handler()
{
}

void HardFault_Handler()
{
    for (;;)
    {
    }
}

void MemManage_Handler()
{
    for (;;)
    {
    }
}

void BusFault_Handler()
{
    for (;;)
    {
    }
}

void UsageFault_Handler()
{
    for (;;)
    {
    }
}

void DebugMon_Handler()
{
}

void SysTick_Handler()
{
    HAL_IncTick();

    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        xPortSysTickHandler();
    }
}
}
