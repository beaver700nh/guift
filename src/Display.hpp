#pragma once

#include <MCUFRIEND_kbv.h>

#include "Adafruit_GFX.h"
#include "Geometry.hpp"

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
	inline void render(const ui::_BaseElement<Style> *element) {
		element->renderTo(this);
	}

	// TODO wrap more x,y pairs

	inline geom::Size getDimensions() {
		return {
			static_cast<uint16_t>(width()),
			static_cast<uint16_t>(height()),
		};
	}

	inline geom::Size getBaseDimensions() {
		return {
			static_cast<uint16_t>(this->*_steal(_Backdoor_WIDTH {})),
			static_cast<uint16_t>(this->*_steal(_Backdoor_HEIGHT {})),
		};
	}

	inline void setCursor(geom::Point position) {
		Adafruit_GFX::setCursor(geom_xy(position));
	}

	inline void setCursor(geom::Size position) {
		Adafruit_GFX::setCursor(geom_xy(position));
	}
};

}
