#pragma once

#include <MCUFRIEND_kbv.h>

template<typename Tag, typename Tag::type member>
struct _AccessHack {
	friend typename Tag::type _steal(Tag) {
		return member;
	}
};

#define _AddBackdoor(Class, Type, Victim, property) \
	struct _Backdoor_##property { \
		typedef Type Class::*type; \
		friend type _steal(_Backdoor_##property); \
	}; \
	template struct _AccessHack<_Backdoor_##property, &Victim::property>;

_AddBackdoor(Adafruit_GFX, int16_t, MCUFRIEND_kbv, WIDTH)
_AddBackdoor(Adafruit_GFX, int16_t, MCUFRIEND_kbv, HEIGHT)

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
