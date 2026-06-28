#include <Arduino.h>
#include <MCUFRIEND_kbv.h>

#include "pins_arduino.h"
#include "src/Touchscreen.hpp"

MCUFRIEND_kbv tft;

guift::Touchscreen::Calibration calib {
	{130, 87},
	{898, 91},
	{129, 953},
	{906, 954},
	{541, 521},
};

guift::Touchscreen touchscreen {8, A2, A3, 9, 300, calib, &tft};

void setup() {
	Serial.begin(115200);
	tft.begin(0x9486);
	tft.fillScreen(TFT_BLACK);

	// auto c = guift::Touchscreen::runCalibration(8, A2, A3, 9, 300, &tft);
	// Serial.println(c.xMinYMin);
	// Serial.println(c.xMaxYMin);
	// Serial.println(c.xMinYMax);
	// Serial.println(c.xMaxYMax);
	// Serial.println(c.center);

	touchscreen.begin(guift::Touchscreen::QuadrilateralMode::FREE);
}

void loop() {
	static guift::Touchscreen::Touch touch;
	static uint8_t orientation = 0;
	static unsigned long last = 0;

	if (guift::Touchscreen::Touch::isValid(touch = touchscreen.getTouch())) {
		tft.drawPixel(touch.x, touch.y, static_cast<uint16_t>(touch.z * 15));
	}

	auto now = millis();
	if (now - last > 2000) {
		last = now;
		orientation = ++orientation & 3;
		tft.setRotation(orientation);
	}
}
