#include <Arduino.h>

#include <MCUFRIEND_kbv.h>

#include "HardwareSerial.h"
#include "src/Touchscreen.hpp"
#include "src/Common.hpp"

MCUFRIEND_kbv tft;
guift::Touchscreen ts {
	8, A2, A3, 9, 300,
	{	{146, 89},
		{879, 91},
		{175, 949},
		{909, 947},
		{543, 522}},
	&tft};

void setup() {
	Serial.begin(115200);
	tft.begin(0x9486);
	ts.begin(guift::Touchscreen::QuadrilateralMode::FREE);
}

void loop() {
}
