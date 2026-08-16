BUILD_DIR ?= build
FIRMWARE := $(BUILD_DIR)/firmware.elf
OPENOCD ?= openocd
OPENOCD_CONFIG ?= board/st_nucleo_f4.cfg

.PHONY: all configure build flash clean

all: build

configure:
	cmake -S . -B $(BUILD_DIR) -G Ninja \
		-DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake

build:
	cmake --build $(BUILD_DIR)

flash: build
	$(OPENOCD) \
		-f $(OPENOCD_CONFIG) \
		-c "program $(FIRMWARE) verify reset exit"

clean:
	cmake --build $(BUILD_DIR) --target clean
