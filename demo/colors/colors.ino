#include <Arduino.h>
#include <stdlib.h>
#include <string.h>

#include <MCUFRIEND_kbv.h>

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
	uint16_t m1 = tft.height() / 11;
	BAR( 0, m1, guift::color::black)
	BAR( 1, m1, guift::color::gray1)
	BAR( 2, m1, guift::color::gray2)
	BAR( 3, m1, guift::color::gray3)
	BAR( 4, m1, guift::color::gray4)
	BAR( 5, m1, guift::color::gray5)
	BAR( 6, m1, guift::color::gray6)
	BAR( 7, m1, guift::color::gray7)
	BAR( 8, m1, guift::color::gray8)
	BAR( 9, m1, guift::color::gray9)
	BAR(10, m1, guift::color::white)
	while (!ts.isTouching());

	uint16_t m2 = tft.height() / 6;
	BAR(0, m2, guift::color::red)
	BAR(1, m2, guift::color::green)
	BAR(2, m2, guift::color::blue)
	BAR(3, m2, guift::color::cyan)
	BAR(4, m2, guift::color::magenta)
	BAR(5, m2, guift::color::yellow)
	while (!ts.isTouching());

	uint16_t m3 = tft.height() / 9;
	BAR(0, m3, guift::color::maroon)
	BAR(1, m3, guift::color::orange)
	BAR(2, m3, guift::color::greenYellow)
	BAR(3, m3, guift::color::olive)
	BAR(4, m3, guift::color::darkGreen)
	BAR(5, m3, guift::color::darkCyan)
	BAR(6, m3, guift::color::navy)
	BAR(7, m3, guift::color::purple)
	BAR(8, m3, guift::color::pink)
	while (!ts.isTouching());

	uint16_t m4 = tft.height() / 13;
	BAR( 0, m4, guift::color::maroon)
	BAR( 1, m4, guift::color::red)
	BAR( 2, m4, guift::color::orange)
	BAR( 3, m4, guift::color::yellow)
	BAR( 4, m4, guift::color::greenYellow)
	BAR( 5, m4, guift::color::green)
	BAR( 6, m4, guift::color::cyan)
	BAR( 7, m4, guift::color::darkCyan)
	BAR( 8, m4, guift::color::blue)
	BAR( 9, m4, guift::color::navy)
	BAR(10, m4, guift::color::purple)
	BAR(11, m4, guift::color::magenta)
	BAR(12, m4, guift::color::pink)
	while (!ts.isTouching());

	tft.drawRect(0, 0, tft.width(), 50, guift::color::red);
	tft.fillRect(1, 1, tft.width() - 2, 48, guift::color::white);
	tft.drawLine(1, 1, tft.width() - 2, 48, guift::color::red);
	tft.fillRect(0, 50, tft.width(), tft.height() - 50, guift::color::black);

	double color = 0.0;
	for (guift::Touchscreen::Touch touch; (touch = ts.getTouch()).y >= 50 || !touch.isValid();) {
		if (touch.isValid()) {
			tft.drawPixel(touch.x, touch.y,
				guift::color::from24Bit(
					0xFF * max(0.0, cos(color)),
					0xFF * max(0.0, cos(color - PI/1.5)),
					0xFF * max(0.0, cos(color - PI/0.75))
				));
			color += 0.001;
		}
	}

	tft.fillScreen(guift::color::black);
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

		tft.fillScreen(guift::color::from24Bit(strtol(buf, nullptr, 16)));
	}

exit:
	;
}
