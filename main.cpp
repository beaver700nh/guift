#include <Arduino.h>

#include "src/Common.hpp"
#include "src/Display.hpp"
#include "src/Touchscreen.hpp"

#include "src/ui/Text.hpp"

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

	using namespace guift::ui;

	tft.fillScreen(guift::color::black);

	tft.render(
		Text {"[THIS IS A TEST!!]\nhi", Text::Style {}
			.setFg(guift::color::red)
			.setBg(+guift::color::lightGray)}
	);
	tft.render(
		Text {"test 2"}
	);
	tft.render(
		Text {"test 3", Text::Style {}
			.setPosition({100, 100})
			.setSize(3)}
	);
	tft.render(
		Text {"test 4", Text::Style {}
			.setSize(1)
			.setFg(guift::color::green)
			.setBg(+guift::color::darkGray)}
	);
}

void loop() {
}
