#include "FreeRTOS.h"
#include "task.h"
#include "platform.hpp"

namespace
{
void matrixTask(void *argument)
{
    static_cast<void>(argument);

    constexpr platform::RgbColor blue{0U, 0U, 32U};
    platform::writeWs2812(blue);
    vTaskDelete(nullptr);
}
}

int main()
{
    platform::initialize();

    configASSERT(
        xTaskCreate(
            matrixTask,
            "matrix",
            configMINIMAL_STACK_SIZE,
            nullptr,
            tskIDLE_PRIORITY + 1U,
            nullptr
        ) == pdPASS
    );

    vTaskStartScheduler();
    configASSERT(pdFALSE);
}
