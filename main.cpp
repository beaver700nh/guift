#include <Arduino.h>

#include <MCUFRIEND_kbv.h>

#include "src/Touchscreen.hpp"

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
	tft.begin(0x9486);
}

void loop() {
	tft.fillScreen(TFT_MAGENTA);
}
