#pragma once

#include "../Common.hpp"
#include "../Geometry.hpp"
#include "../Display.hpp"
#include "BaseElement.hpp"

namespace guift {

namespace ui {

#if 0
class Box: public _BaseElement {
public:
	struct Style {
		geom::Point position;
		geom::Size size;
		color::Color fill;
		color::Color border;
		uint8_t roundness;

		inline Style():
			position {0, 0},
			size {100, 100},
			fill {+color::gray1},
			border {+color::gray5},
			roundness {0} {};
	};

	inline Box():
		Box {{}} {};
	inline Box(const Style &style):
		style {style} {};

private:
	inline void renderTo(Display *tft) const {
		auto isStatic = style.position.isPositive();
		geom::Point position;

		if (isStatic) {
			tft->setCursor(style.position.x, style.position.y);
			position = style.position;
		}
		else {
			position = geom::Point {tft->getCursorX(), tft->getCursorY()};
		}

		tft->setTextSize(style.size);
		tft->setTextColor(style.fg, style.bg == color::transparent ? style.fg : style.bg);

		tft->print(text);

		tft->getTextBounds(
			text, position.x, position.y,
			&box.origin.x, &box.origin.y,
			&box.size.x, &box.size.y
		);

#ifdef UI_DEBUG
		tft->drawRect(box.origin.x, box.origin.y, box.size.x, box.size.y, color::yellowGreen);
#endif
	}
	Style style;
};
#endif

}}
