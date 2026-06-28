#include <Arduino.h>

#include <MCUFRIEND_kbv.h>

#include "src/Touchscreen.hpp"

#define TS_INIT 8, A2, A3, 9, 300

MCUFRIEND_kbv tft;
guift::Touchscreen ts {TS_INIT, {}, &tft};

void setup() {
	tft.begin(0x9486);
}

void loop() {
	static char buf[256];

	tft.setRotation(1);
	tft.fillScreen(TFT_BLACK);
	tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
	tft.setCursor(48, 48);
	tft.setTextSize(2);
	tft.println("Calibration");
	tft.drawFastHLine(48, 48 + 9*2, 11*6*2, TFT_WHITE);
	tft.setCursor(0, 72);
	tft.setTextSize(1);
	tft.println("          Touch each dot briefly");
	tft.println("          when it appears.");

	auto calib = guift::Touchscreen::runCalibration(TS_INIT, &tft);

	snprintf(
		buf, sizeof(buf),

		"    Calibration calib {\n"
		"      {%d, %d},\n"
		"      {%d, %d},\n"
		"      {%d, %d},\n"
		"      {%d, %d},\n"
		"      {%d, %d},\n"
		"    };\n",

		calib.xMinYMin.x, calib.xMinYMin.y,
		calib.xMaxYMin.x, calib.xMaxYMin.y,
		calib.xMinYMax.x, calib.xMinYMax.y,
		calib.xMaxYMax.x, calib.xMaxYMax.y,
		calib.center.x, calib.center.y
	);

	tft.setRotation(0);
	tft.fillScreen(TFT_WHITE);
	tft.setTextColor(TFT_BLACK, TFT_WHITE);
	tft.setCursor(24, 24);
	tft.setTextSize(2);
	tft.println("Results");
	tft.drawFastHLine(24, 24 + 9*2, 7*6*2, TFT_BLACK);
	tft.setCursor(0, 60);
	tft.setTextSize(1);
	tft.print(buf);
	tft.setCursor(24, tft.height() - 24 - 8);
	tft.print("Tap the screen to restart.");

	while (!ts.isTouching());
}
