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

	using namespace guift::ui;

	tft.fillScreen(guift::color::black);

	// tft.render(
	// 	Box {
	// 		// Box::Style {}
	// 		// 	.setPosition({10, 10})
	// 		// 	.setSize({100, 100})
	// 		// 	.setFill(guift::color::red)
	// 		// 	.setBorder(guift::color::green)
	// 		// 	.setRoundness(10)
	// 	}
	// );
}

void loop() {
}
