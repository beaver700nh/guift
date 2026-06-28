#include <Arduino.h>
#include <stdlib.h>
#include <string.h>

#include <MCUFRIEND_kbv.h>

#include "HardwareSerial.h"
#include "src/Touchscreen.hpp"
#include "src/Common.hpp"

#define BAR(number, thickness, color) tft.fillRect(0, number * thickness, tft.width(), thickness, color);

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
	uint16_t m = tft.height() / 10;
	BAR(0, m, guift::Color::black)
	BAR(1, m, guift::Color::darkGrey)
	BAR(2, m, guift::Color::lightGrey)
	BAR(3, m, guift::Color::white)
	BAR(4, m, guift::Color::red)
	BAR(5, m, guift::Color::green)
	BAR(6, m, guift::Color::blue)
	BAR(7, m, guift::Color::cyan)
	BAR(8, m, guift::Color::magenta)
	BAR(9, m, guift::Color::yellow)
	while (!ts.isTouching());

	uint16_t n = tft.height() / 9;
	BAR(0, n, guift::Color::maroon)
	BAR(1, n, guift::Color::orange)
	BAR(2, n, guift::Color::greenYellow)
	BAR(3, n, guift::Color::olive)
	BAR(4, n, guift::Color::darkGreen)
	BAR(5, n, guift::Color::darkCyan)
	BAR(6, n, guift::Color::navy)
	BAR(7, n, guift::Color::purple)
	BAR(8, n, guift::Color::pink)
	while (!ts.isTouching());

	uint16_t p = tft.height() / 13;
	BAR( 0, p, guift::Color::maroon)
	BAR( 1, p, guift::Color::red)
	BAR( 2, p, guift::Color::orange)
	BAR( 3, p, guift::Color::yellow)
	BAR( 4, p, guift::Color::greenYellow)
	BAR( 5, p, guift::Color::green)
	BAR( 6, p, guift::Color::cyan)
	BAR( 7, p, guift::Color::darkCyan)
	BAR( 8, p, guift::Color::blue)
	BAR( 9, p, guift::Color::navy)
	BAR(10, p, guift::Color::purple)
	BAR(11, p, guift::Color::magenta)
	BAR(12, p, guift::Color::pink)
	while (!ts.isTouching());

	tft.drawRect(0, 0, tft.width(), 50, guift::Color::red);
	tft.fillRect(1, 1, tft.width() - 2, 48, guift::Color::white);
	tft.drawLine(1, 1, tft.width() - 2, 48, guift::Color::red);
	tft.fillRect(0, 50, tft.width(), tft.height() - 50, guift::Color::black);

	double color = 0.0;
	for (guift::Touchscreen::Touch touch; (touch = ts.getTouch()).y >= 50 || !touch.isValid();) {
		if (touch.isValid()) {
			tft.drawPixel(touch.x, touch.y,
				guift::Color::from24Bit(
					0xFF * max(0.0, cos(color)),
					0xFF * max(0.0, cos(color - PI/1.5)),
					0xFF * max(0.0, cos(color - PI/0.75))
				));
			color += 0.001;
		}
	}

	tft.fillScreen(guift::Color::black);
	tft.setCursor(0, 0);
	tft.println("Type hex color code in Serial.\nSubmit nothing to exit.\nTap screen to exit.");

	char buf[16];
	char *ptr;

	while (true) {
		ptr = buf;
		do {
			while (Serial.available() <= 0) {
				if (ts.isTouching())
					goto exit;
			}
		}
		while ((*ptr++ = Serial.read()) != '\n');
		*(ptr - 1) = '\0';

		while (Serial.available() > 0)
			Serial.read();

		if (strlen(buf) == 0)
			break;

		tft.fillScreen(guift::Color::from24Bit(strtol(buf, nullptr, 16)));
	}

exit:
	;
}
