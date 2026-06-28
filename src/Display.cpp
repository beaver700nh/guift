#include "ui/BaseElement.hpp"
#include "Display.hpp"

namespace guift {

void Display::render(const ui::_BaseElement &element) {
	element.renderTo(this);
}

}
