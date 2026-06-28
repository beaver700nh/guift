#include "ui/BaseElement.hpp"
#include "Display.hpp"

namespace guift {

Display::Display(): MCUFRIEND_kbv {} {
	// Prevent interfering with text bounding box calculations
	setTextWrap(false);
}

void Display::render(const ui::_BaseElement &element) {
	element.renderTo(this);
}

void Display::render(const ui::_BaseElement *element) {
	element->renderTo(this);
}

}
