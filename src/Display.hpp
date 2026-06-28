#pragma once

#include <MCUFRIEND_kbv.h>

namespace guift {

namespace ui {
class _BaseElement;
}

class Display: public MCUFRIEND_kbv {
public:
	void render(const ui::_BaseElement &element);
};

}
