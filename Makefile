export AVR_MCU_NAME := atmega2560
export AVR_MCU_FREQ := 16000000
export AVR_MCU_ARCH := avr6

PORT := $$(echo $$({ ls /dev/ttyACM* 2>/dev/null || echo '/dev/ttyACM0'; } | head -n1))
BOARD := arduino:avr:mega
DIR := $${DEMO_NAME:-.}

all: build monitor

build: compile upload

compile:
	arduino-cli compile --fqbn $(BOARD) --build-property build.extra_flags="-std=gnu++17 -Wall -Wextra" --verbose $(DIR)

upload:
	@$(call WaitPlugIn)
	arduino-cli upload --fqbn $(BOARD) --port $(PORT) --verbose $(DIR)

monitor:
	@$(call WaitPlugIn)
	arduino-cli monitor --fqbn $(BOARD) --port $(PORT) --config baudrate=115200

define WaitPlugIn
	while [ ! -e $(PORT) ]; do printf .; sleep 2; done
	echo
endef
