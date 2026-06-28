#include <Arduino.h>

#include "src/Common.hpp"
#include "src/Display.hpp"
#include "src/Touchscreen.hpp"

#include "src/ui/Box.hpp"

guift::Display tft;
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

	tft.fillScreen(guift::color::black);
}

void loop() {
	using namespace guift::ui;

	static bool flag = false;
	static Box box {BoxStyle {}
		.setFill(+guift::color::red)
		.setSize({80, 80})
		.setThickness(5)
		.setRoundness(15)};

	if (flag) {
		box.getStyle().setBorder(+guift::color::yellowGreen);
	} else {
		box.getStyle().setBorder(guift::color::transparent);
	}

	flag = !flag;
	tft.render(box);

	delay(200);
	while (!ts.isTouching());
}
