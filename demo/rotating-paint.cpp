/*
 * DEMO: Paint program that keeps working even when the display orientation is changed.
 */

#include <Arduino.h>
#include <MCUFRIEND_kbv.h>

#include "src/Touchscreen.hpp"

MCUFRIEND_kbv tft;
uint8_t rotation = 0;
long last = 0;

guift::Touchscreen::Calibration calib {
	{116, 64},
	{892, 71},
	{106, 963},
	{938, 964},
	{524, 530}
};

guift::Touchscreen touchscreen {8, A2, A3, 9, 300, calib, &tft};

void setup() {
	tft.begin(0x9486);
	tft.fillScreen(TFT_BLACK);
}

void loop() {
	static guift::Touchscreen::Touch touch;

	if (guift::Touchscreen::Touch::isValid(touch = touchscreen.getTouch())) {
		tft.drawPixel(touch.x, touch.y, TFT_RED);
	}

	long now = millis();
	if (now - last > 3000) {
		last = now;

		tft.setRotation(rotation = (rotation + 1) & 3);
		touchscreen.begin(guift::Touchscreen::QuadrilateralMode::RECTANGULAR);

		tft.setCursor(0, 0);
		tft.fillRect(0, 0, 5, 8, 0);
		tft.write('0' + rotation);
	}
}
