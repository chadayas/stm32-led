set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(ARM_TOOLCHAIN_ROOT "$ENV{HOME}/.local/arm-none-eabi")
set(ARM_TOOLCHAIN_BIN "${ARM_TOOLCHAIN_ROOT}/usr/bin")

find_program(CMAKE_C_COMPILER arm-none-eabi-gcc
    HINTS "${ARM_TOOLCHAIN_BIN}"
    REQUIRED
    NO_CACHE
)
find_program(CMAKE_CXX_COMPILER arm-none-eabi-g++
    HINTS "${ARM_TOOLCHAIN_BIN}"
    REQUIRED
    NO_CACHE
)
find_program(CMAKE_ASM_COMPILER arm-none-eabi-gcc
    HINTS "${ARM_TOOLCHAIN_BIN}"
    REQUIRED
    NO_CACHE
)
find_program(CMAKE_OBJCOPY arm-none-eabi-objcopy
    HINTS "${ARM_TOOLCHAIN_BIN}"
    REQUIRED
    NO_CACHE
)
find_program(CMAKE_SIZE arm-none-eabi-size
    HINTS "${ARM_TOOLCHAIN_BIN}"
    REQUIRED
    NO_CACHE
)

set(MCU_FLAGS
    "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard"
)
set(NEWLIB_ROOT "${ARM_TOOLCHAIN_ROOT}/usr/lib/arm-none-eabi/newlib")
set(NEWLIB_MULTILIB "${NEWLIB_ROOT}/thumb/v7e-m+fp/hard")

set(CMAKE_C_FLAGS_INIT
    "${MCU_FLAGS} -isystem ${ARM_TOOLCHAIN_ROOT}/usr/include/newlib -ffunction-sections -fdata-sections"
)
set(CMAKE_CXX_FLAGS_INIT
    "${MCU_FLAGS} -isystem ${ARM_TOOLCHAIN_ROOT}/usr/include/newlib -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics"
)
set(CMAKE_ASM_FLAGS_INIT "${MCU_FLAGS} -x assembler-with-cpp")
set(CMAKE_EXE_LINKER_FLAGS_INIT
    "${MCU_FLAGS} -L${NEWLIB_MULTILIB} -L${NEWLIB_ROOT} --specs=${NEWLIB_ROOT}/nosys.specs"
)
