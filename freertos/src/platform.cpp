#include "platform.hpp"

#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_hal.h"

namespace
{
constexpr uint16_t kWs2812Pin = GPIO_PIN_10;
GPIO_TypeDef *const kWs2812Port = GPIOA;

void systemClockConfig();

inline __attribute__((always_inline)) void waitUntil(uint32_t deadline)
{
    while (static_cast<int32_t>(DWT->CYCCNT - deadline) < 0)
    {
    }
}
}

namespace platform
{
void initialize()
{
    HAL_Init();
    systemClockConfig();

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {};
    gpio.Pin = kWs2812Pin;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(kWs2812Port, &gpio);
    HAL_GPIO_WritePin(kWs2812Port, kWs2812Pin, GPIO_PIN_RESET);

    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0U;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

__attribute__((optimize("O2")))
void writeWs2812(RgbColor color)
{
    const uint32_t bit_cycles = SystemCoreClock / 800000U;
    const uint32_t zero_high_cycles = (bit_cycles * 28U) / 100U;
    const uint32_t one_high_cycles = (bit_cycles * 56U) / 100U;
    const uint32_t color_word =
        (static_cast<uint32_t>(color.green) << 16U) |
        (static_cast<uint32_t>(color.red) << 8U) |
        color.blue;

    taskENTER_CRITICAL();
    for (uint32_t mask = 1UL << 23U; mask != 0U; mask >>= 1U)
    {
        const uint32_t high_cycles = (color_word & mask) != 0U
                                              ? one_high_cycles
                                              : zero_high_cycles;

        kWs2812Port->BSRR = kWs2812Pin;
        const uint32_t bit_start = DWT->CYCCNT;
        waitUntil(bit_start + high_cycles);
        kWs2812Port->BSRR = static_cast<uint32_t>(kWs2812Pin) << 16U;
        waitUntil(bit_start + bit_cycles);
    }
    taskEXIT_CRITICAL();

    HAL_Delay(1U);
}
}

namespace
{
void systemClockConfig()
{
    RCC_OscInitTypeDef oscillator = {};
    RCC_ClkInitTypeDef clocks = {};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    oscillator.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    oscillator.HSIState = RCC_HSI_ON;
    oscillator.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    oscillator.PLL.PLLState = RCC_PLL_ON;
    oscillator.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    oscillator.PLL.PLLM = 16;
    oscillator.PLL.PLLN = 200;
    oscillator.PLL.PLLP = RCC_PLLP_DIV2;
    oscillator.PLL.PLLQ = 7;
    oscillator.PLL.PLLR = 2;
    configASSERT(HAL_RCC_OscConfig(&oscillator) == HAL_OK);

    clocks.ClockType = RCC_CLOCKTYPE_SYSCLK |
                       RCC_CLOCKTYPE_HCLK |
                       RCC_CLOCKTYPE_PCLK1 |
                       RCC_CLOCKTYPE_PCLK2;
    clocks.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clocks.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clocks.APB1CLKDivider = RCC_HCLK_DIV2;
    clocks.APB2CLKDivider = RCC_HCLK_DIV1;
    configASSERT(HAL_RCC_ClockConfig(&clocks, FLASH_LATENCY_5) == HAL_OK);
}
}
