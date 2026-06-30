#include <Arduino.h>

#include "src/Common.hpp"
#include "src/Display.hpp"

#include "src/ui/Text.hpp"

guift::Display tft;

void setup() {
	tft.begin(0x9486);

	using namespace guift::ui;

	tft.setRotation(1);
	tft.fillScreen(guift::color::black);

	tft.render(
		Text {"Hello, World!", TextStyle {}
			.setPosition({10, 10})
			.setSize(1)
			.setUnderline(+guift::color::red)
			.setStrike(+guift::color::blue)}
	);
	tft.render(
		Text {"Hello, World!", TextStyle {}
			.setPosition({10, 20})
			.setSize(2)
			.setUnderline(+guift::color::red)
			.setStrike(+guift::color::blue)}
	);
	tft.render(
		Text {"Hello, World!", TextStyle {}
			.setPosition({10, 40})
			.setSize(3)
			.setUnderline(+guift::color::red)
			.setStrike(+guift::color::blue)}
	);
	tft.render(
		Text {"Hello, World!", TextStyle {}
			.setPosition({10, 70})
			.setSize(4)
			.setUnderline(+guift::color::red)
			.setStrike(+guift::color::blue)}
	);
	tft.render(
		Text {"Hello, World!", TextStyle {}
			.setPosition({10, 110})
			.setSize(5)
			.setUnderline(+guift::color::red)
			.setStrike(+guift::color::blue)}
	);
	tft.render(
		Text {"Hello, World!", TextStyle {}
			.setPosition({10, 160})
			.setSize(6)
			.setUnderline(+guift::color::red)
			.setStrike(+guift::color::blue)}
	);
	tft.render(
		Text {"Hello, World!", TextStyle {}
			.setPosition({10, 230})
			.setSize(7)
			.setUnderline(+guift::color::red)
			.setStrike(+guift::color::blue)}
	);
}

void loop() {}
