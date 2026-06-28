/*
 * DEMO: Paint program that keeps working even when the display orientation is changed.
 */

#include <Arduino.h>

#include <MCUFRIEND_kbv.h>

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

void writeOrientation(MCUFRIEND_kbv *tft, uint8_t orientation) {
	tft->setRotation(0);
	tft->setCursor(0, 0);
	tft->fillRect(0, 0, 10, 16, TFT_BLACK);
	tft->write('0' + orientation);
}

void setup() {
	tft.begin(0x9486);
	tft.fillScreen(TFT_BLACK);
	tft.setTextSize(2);
	writeOrientation(&tft, 0);
}

void loop() {
	static guift::Touchscreen::Touch touch;
	static uint8_t orientation = 0;
	static unsigned long last = millis();

	auto now = millis();
	if (now - last > 2000) {
		last = now;
		orientation = ++orientation & 3;
		writeOrientation(&tft, orientation);
		tft.setRotation(orientation);
		touchscreen.begin(guift::Touchscreen::QuadrilateralMode::FREE);
	}

	if (guift::Touchscreen::Touch::isValid(touch = touchscreen.getTouch())) {
		tft.drawPixel(touch.x, touch.y, static_cast<uint16_t>(touch.z * 20));
	}
}
