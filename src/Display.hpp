#pragma once

#include <MCUFRIEND_kbv.h>

namespace guift {

namespace ui {
class _BaseElement;
}

class Display: public MCUFRIEND_kbv {
public:
	Display();

	void render(const ui::_BaseElement &element);
	// Caller is responsible for memory management!
	void render(const ui::_BaseElement *element);
};

}
