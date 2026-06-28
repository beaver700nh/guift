#pragma once

#include <MCUFRIEND_kbv.h>

namespace guift {

namespace ui {
	template<typename Style>
	class _BaseElement;
}

class Display: public MCUFRIEND_kbv {
public:
	inline Display(): MCUFRIEND_kbv {} {
		// Prevent interfering with text bounding box calculations
		setTextWrap(false);
	}

	template<typename Style>
	inline void render(const ui::_BaseElement<Style> &element) {
		element.renderTo(this);
	}

	// Caller is responsible for memory management!
	template<typename Style>
	void render(const ui::_BaseElement<Style> *element) {
		element->renderTo(this);
	}
};

}
