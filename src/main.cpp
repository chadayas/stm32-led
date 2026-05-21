extern "C" {
#include <stm32f4xx_hal_gpio.h>
}

// Nucleo F446RE - LD2 is on PA5
#define LED_PIN  GPIO_PIN_5
#define LED_PORT GPIOA

static void led_init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {};
    gpio.Pin   = LED_PIN;
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &gpio);
}

int main(void)
{
    HAL_Init();
    led_init();

    while (1) {
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        HAL_Delay(500);
    }

    return 0;
}
