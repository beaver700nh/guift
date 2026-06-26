export AVR_MCU_NAME := atmega2560
export AVR_MCU_FREQ := 16000000
export AVR_MCU_ARCH := avr6

PORT := /dev/ttyACM0

build: compile upload

compile:
	arduino-cli compile --fqbn arduino:avr:mega .

upload:
	@$(call WaitPlugIn)
	arduino-cli upload -p $(PORT) --fqbn arduino:avr:mega .

define WaitPlugIn
	while [ ! -e $(PORT) ]; do printf .; sleep 2; done
	echo
endef
