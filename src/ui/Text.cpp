#include "../Common.hpp"
#include "../Display.hpp"
#include "Text.hpp"

namespace guift {

namespace ui {

void Text::renderTo(Display *tft) const {
	tft->setCursor(0, 0);
	tft->setTextColor(Color::white);
	tft->setTextSize(1);
	tft->println("Hello, world!");
}

}}
