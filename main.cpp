#include <Arduino.h>

#include "src/Common.hpp"
#include "src/Display.hpp"
#include "src/Touchscreen.hpp"

#include "src/ui/Text.hpp"
#include "src/ui/Box.hpp"
#include "src/ui/Button.hpp"

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
		Button {ButtonStyle {},

			Box {BoxStyle {}
				.setPosition({80, 50})
				.setFill(+guift::color::white)
				.setBorder(+guift::color::gray5)
				.setRoundness(12)
				// .setRoundness(-1) // TODO fix clamping being applied before size is set by button
				.setThickness(2)},
			Text {"I'm a button!", TextStyle {}
				.setFg(guift::color::black)
				.setUnderline(+guift::color::gray2)}}
	);

	tft.render(
		Button {ButtonStyle {}
			.useTextAlignment({Button::Align::pack, Button::Align::none}, {18, 24}),

			Box {BoxStyle {}
				.setSize({0, 50})
				.setPosition({80, 100})
				.setFill(+guift::color::yellow)
				.setBorder(+guift::color::red)
				.setThickness(1)},
			Text {"I'm a button!", TextStyle {}
				.setFg(guift::color::black)
				.setUnderline(+guift::color::gray2)}}
	);

	tft.render(
		Button {ButtonStyle {}
			.useTextOffset({15, 30}),

			Box {BoxStyle {}
				.setSize({220, 50})
				.setPosition({80, 170})
				.setRoundness(-1)
				.setThickness(5)},
			Text {"I'm another button!", TextStyle {}
				.setSize(1)
				.setFg(guift::color::magenta)
				.setStrike(+guift::color::pink)}}
	);

	tft.render(
		Button {ButtonStyle {},

			Box {BoxStyle {}
				.setPosition({80, 240})},
			Text {"Button #3\nsays hello!", TextStyle {}
				.setSize(2)
				.setFg(guift::color::cyan)}
		}
	);
}

void loop() {
	using namespace guift::ui;

	static unsigned long last = 0;
	static bool flag = false;
	static Box box {BoxStyle {}
		.setSize({40, 200})
		.setThickness(4)
		.setRoundness(100)};
	static auto color = box.getStyle().border;

	auto now = millis();
	if (now - last >= 2000) {
		last = now;
		if (flag) {
			box.getStyle().setBorder(color);
		} else {
			box.getStyle().setBorder(guift::color::transparent);
		}
		flag = !flag;
	}

	auto touch = ts.getTouch();
	if (touch.isValid()) {
		box.getStyle().setPosition(touch);
	}

	tft.render(box);
}
