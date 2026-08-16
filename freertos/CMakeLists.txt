cmake_minimum_required(VERSION 3.20)

project(sys LANGUAGES C CXX ASM)

set(FREERTOS_KERNEL_PATH
    "${CMAKE_CURRENT_LIST_DIR}/third_party/FreeRTOS-Kernel"
    CACHE PATH "Path to the FreeRTOS-Kernel checkout"
)
set(STM32CUBE_F4_PATH
    "${CMAKE_CURRENT_LIST_DIR}/third_party/STM32CubeF4"
    CACHE PATH "Path to the STM32CubeF4 checkout"
)

set(STM32_TEMPLATE_PATH
    "${STM32CUBE_F4_PATH}/Projects/STM32446E-Nucleo/Templates"
)
set(STM32_HAL_PATH
    "${STM32CUBE_F4_PATH}/Drivers/STM32F4xx_HAL_Driver"
)
set(STM32_CMSIS_DEVICE_PATH
    "${STM32CUBE_F4_PATH}/Drivers/CMSIS/Device/ST/STM32F4xx"
)

foreach(required_file
    "${FREERTOS_KERNEL_PATH}/CMakeLists.txt"
    "${STM32_TEMPLATE_PATH}/STM32CubeIDE/STM32F446RETX_FLASH.ld"
    "${STM32_TEMPLATE_PATH}/STM32CubeIDE/Example/Startup/startup_stm32f446retx.s"
)
    if(NOT EXISTS "${required_file}")
        message(FATAL_ERROR "Required dependency was not found: ${required_file}")
    endif()
endforeach()

add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/include"
)

set(FREERTOS_PORT "GCC_ARM_CM4F" CACHE STRING "FreeRTOS port" FORCE)
set(FREERTOS_HEAP "4" CACHE STRING "FreeRTOS heap implementation" FORCE)
add_subdirectory("${FREERTOS_KERNEL_PATH}" FreeRTOS-Kernel)

add_library(stm32f4_hal STATIC
    "${STM32_HAL_PATH}/Src/stm32f4xx_hal.c"
    "${STM32_HAL_PATH}/Src/stm32f4xx_hal_cortex.c"
    "${STM32_HAL_PATH}/Src/stm32f4xx_hal_flash.c"
    "${STM32_HAL_PATH}/Src/stm32f4xx_hal_flash_ex.c"
    "${STM32_HAL_PATH}/Src/stm32f4xx_hal_gpio.c"
    "${STM32_HAL_PATH}/Src/stm32f4xx_hal_pwr.c"
    "${STM32_HAL_PATH}/Src/stm32f4xx_hal_pwr_ex.c"
    "${STM32_HAL_PATH}/Src/stm32f4xx_hal_rcc.c"
    "${STM32_HAL_PATH}/Src/stm32f4xx_hal_rcc_ex.c"
    "${STM32_TEMPLATE_PATH}/Src/stm32f4xx_hal_msp.c"
    "${STM32_TEMPLATE_PATH}/Src/system_stm32f4xx.c"
)
target_compile_definitions(stm32f4_hal PUBLIC STM32F446xx USE_HAL_DRIVER)
target_include_directories(stm32f4_hal PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/include"
    "${STM32_TEMPLATE_PATH}/Inc"
    "${STM32_HAL_PATH}/Inc"
    "${STM32_HAL_PATH}/Inc/Legacy"
    "${STM32CUBE_F4_PATH}/Drivers/CMSIS/Core/Include"
    "${STM32_CMSIS_DEVICE_PATH}/Include"
)

add_executable(firmware
    src/main.cpp
    src/platform.cpp
    src/runtime.cpp
    src/stm32f4xx_it.cpp
    "${STM32_TEMPLATE_PATH}/STM32CubeIDE/Example/Startup/startup_stm32f446retx.s"
)
target_link_libraries(firmware PRIVATE
    stm32f4_hal
    freertos_kernel
)
set_target_properties(firmware PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    LINKER_LANGUAGE C
)
target_compile_options(firmware PRIVATE -Wall -Wextra)

set(LINKER_SCRIPT
    "${STM32_TEMPLATE_PATH}/STM32CubeIDE/STM32F446RETX_FLASH.ld"
)
target_link_options(firmware PRIVATE
    "-T${LINKER_SCRIPT}"
    "-nostartfiles"
    "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/firmware.map"
    "-Wl,--gc-sections"
    "-Wl,--print-memory-usage"
)
set_target_properties(firmware PROPERTIES SUFFIX ".elf")

add_custom_command(TARGET firmware POST_BUILD
    COMMAND "${CMAKE_OBJCOPY}" -O ihex
            "$<TARGET_FILE:firmware>" "${CMAKE_CURRENT_BINARY_DIR}/firmware.hex"
    COMMAND "${CMAKE_OBJCOPY}" -O binary
            "$<TARGET_FILE:firmware>" "${CMAKE_CURRENT_BINARY_DIR}/firmware.bin"
    COMMAND "${CMAKE_SIZE}" "$<TARGET_FILE:firmware>"
    VERBATIM
)
