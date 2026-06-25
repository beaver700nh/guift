export AVR_MCU_NAME := atmega2560
export AVR_MCU_FREQ := 16000000
export AVR_MCU_ARCH := avr6

PORT := /dev/ttyACM0

build: compile upload

compile:
	(mkdir -p build/artifacts && cd build; cmake .. && make -j$$(echo 1))

upload:
	@$(call WaitPlugIn)
	~/.arduino*/packages/arduino/tools/avrdude/*/bin/avrdude -c wiring -p $(AVR_MCU_NAME) -P $(PORT) -b 115200 -D -U flash:w:build/artifacts/guift.elf.hex:i

define WaitPlugIn
	while [ ! -e $(PORT) ]; do printf .; sleep 2; done
	echo
endef

clean:
	rm -r build
