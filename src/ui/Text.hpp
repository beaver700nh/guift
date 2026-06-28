#pragma once

#include "../Common.hpp"
#include "../Geometry.hpp"
#include "../Display.hpp"
#include "BaseElement.hpp"

namespace guift {

namespace ui {

template<typename Str>
class MutableText: public _BaseElement {
public:
	struct Style {
		geom::Point position;
		uint8_t size;
		color::OpaqueColor fg;
		color::Color bg;

		inline Style():
			position {-1, -1},
			size {2},
			fg {color::white},
			bg {color::transparent} {};

		inline Style &setPosition(geom::Point position) {
			this->position = position;
			return *this;
		}

		inline Style &setSize(uint8_t size) {
			this->size = size;
			return *this;
		}

		inline Style &setFg(color::OpaqueColor fg) {
			this->fg = fg;
			return *this;
		}

		inline Style &setBg(color::Color bg) {
			this->bg = bg;
			return *this;
		}
	};

	inline MutableText(Str text):
		MutableText {text, {}} {};
	inline MutableText(Str text, const Style &style):
		text {text},
		style {style} {};

	inline Str getText() const {
		return text;
	}

	inline MutableText &setText(Str text) {
		this->text = text;
		return *this;
	}

	inline Style &getStyle() {
		return style;
	}

	inline MutableText &setStyle(const Style &style) {
		this->style = style;
		return *this;
	}

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

	Str text;
	Style style;
	geom::BoxRect box = {-1, -1};
};

using Text = MutableText<const char *>;

}}
