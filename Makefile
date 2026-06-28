export AVR_MCU_NAME := atmega2560
export AVR_MCU_FREQ := 16000000
export AVR_MCU_ARCH := avr6

PORT := $$(echo $$({ ls /dev/ttyACM* 2>/dev/null || echo '/dev/ttyACM0'; } | head -n1))
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
.ONESHELL: demo
demo:
	pushd demo/$(NAME)
	arduino-cli compile --fqbn $(BOARD)
	if [ $$? -ne 0 ];	then printf "\x1b[1;31mCouldn't run demo '$(NAME)' (build failed).\x1b[0m\n";	exit 1;	fi
	$(call WaitPlugIn)
	arduino-cli upload --fqbn $(BOARD) --port $(PORT)
	popd

define WaitPlugIn
	while [ ! -e $(PORT) ]; do printf .; sleep 2; done
	echo
endef
