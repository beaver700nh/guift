export AVR_MCU_NAME := atmega2560
export AVR_MCU_FREQ := 16000000
export AVR_MCU_ARCH := avr6

PORT := /dev/ttyACM0
BOARD := arduino:avr:mega

all: build monitor

build: compile upload

compile:
	arduino-cli compile --fqbn $(BOARD) --verbose

upload:
	@$(call WaitPlugIn)
	arduino-cli upload --fqbn $(BOARD) --port $(PORT) --verbose

monitor:
	@$(call WaitPlugIn)
	arduino-cli monitor --fqbn $(BOARD) --port $(PORT) --config baudrate=115200

.PHONY: demo
demo:
	pushd demo/$(NAME) && arduino-cli compile --fqbn $(BOARD) && arduino-cli upload --fqbn $(BOARD) --port $(PORT) && popd

define WaitPlugIn
	while [ ! -e $(PORT) ]; do printf .; sleep 2; done
	echo
endef
