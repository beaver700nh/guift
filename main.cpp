#include <Arduino.h>
#include <MCUFRIEND_kbv.h>

#include "src/Touchscreen.hpp"

MCUFRIEND_kbv tft;

guift::Touchscreen::Calibration calib {
	{116, 64},
	{892, 71},
	{106, 963},
	{938, 964},
	{524, 530}
};

guift::Touchscreen touchscreen {8, A2, A3, 9, 300, calib, &tft};

void setup() {
	Serial.begin(115200);
	tft.begin(0x9486);
	tft.fillScreen(TFT_BLACK);

	touchscreen.begin(guift::Touchscreen::QuadrilateralMode::FREE);
}

void loop() {
	static guift::Touchscreen::Touch touch;

	if (guift::Touchscreen::Touch::isValid(touch = touchscreen.getTouch())) {
		tft.drawPixel(touch.x, touch.y, touch.z > 1 ? static_cast<uint16_t>(touch.z) : TFT_GREEN);
	}
}
