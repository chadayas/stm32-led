# Project status

## Completed

- Pinned FreeRTOS-Kernel and STM32CubeF4 as repository-local Git submodules under `third_party/`.
- Selected the STM32CubeF4 `Projects/STM32446E-Nucleo/Templates` project as the hardware baseline for the NUCLEO-F446RE.
- Added a CMake/Ninja C and C++ build targeting the STM32F446RE Cortex-M4F.
- Configured Arm GNU C/C++ cross-compilers for Cortex-M4F hard-float builds.
- Added a one-shot FreeRTOS task that sends dim blue to the first WS2812 pixel through PA10.
- Added a typed C++ platform interface for the 100 MHz clock, PA10 GPIO setup, and cycle-timed WS2812 output.
- Converted every project-owned source file to C++ while preserving C linkage for symbols consumed by FreeRTOS, HAL, newlib, and startup assembly.
- Integrated the STM32 HAL, CMSIS device support, STM32 startup code, STM32 linker script, FreeRTOS `GCC_ARM_CM4F` port, and `heap_4`.
- Integrated HAL and FreeRTOS SysTick handling.
- Generated and flashed verified ELF, BIN, HEX, and linker-map outputs successfully.

## Project files

- `CMakeLists.txt`: mixed C/C++ STM32 HAL and FreeRTOS firmware target, linker configuration, and artifact generation.
- `cmake/arm-none-eabi.cmake`: Cortex-M4F hard-float C/C++ cross-compilation settings and local toolchain paths.
- `include/FreeRTOSConfig.h`: STM32F446RE FreeRTOS configuration with C/C++ linkage support.
- `include/platform.hpp`: typed C++ RGB color and platform API.
- `src/main.cpp`: scheduler startup and one-shot blue WS2812 pixel task.
- `src/platform.cpp`: 100 MHz clock setup, PA10 output setup, and cycle-timed WS2812 transmission.
- `src/runtime.cpp`: C-linked FreeRTOS failure hooks and C runtime stubs.
- `src/stm32f4xx_it.cpp`: C-linked fault handlers and combined HAL/FreeRTOS SysTick handling.

## Build

```bash
cd freertos
cmake --fresh -S . -B build \
  -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Last successful build:

```text
RAM:  18,216 / 131,072 bytes (13.90%)
ROM:  12,448 / 524,288 bytes (2.37%)
```

Generated artifacts:

```text
build/firmware.elf
build/firmware.bin
build/firmware.hex
build/firmware.map
```

The ELF was verified as ARM ELF32, EABI5, hard-float. The linker map contains the C++ symbols `platform::initialize()` and `platform::writeWs2812(platform::RgbColor)`, plus the C-linked reset, interrupt, FreeRTOS hook, and runtime symbols.

## Hardware verification

Flash and verify through the attached ST-Link:

```bash
make flash
```

OpenOCD detected the Cortex-M4 and 512 KiB flash, programmed the C++ firmware, verified it, and reset the target. Runtime register inspection confirmed PA10 in output mode, DIN low after the WS2812 latch interval, and the DWT cycle counter enabled:

```text
GPIOA_MODER=0xa8100000
GPIOA_ODR=0x0
DWT_CTRL=0x40000001
```

This environment is WSL2, so the ST-Link USB device may need to be reattached to WSL after a host or USB reset.

