#pragma once

#include "../Common.hpp"
#include "../Geometry.hpp"
#include "../Display.hpp"
#include "BaseElement.hpp"

namespace guift {

namespace ui {

struct TextStyle {
	geom::Point position {-1, -1};
	uint8_t size = 2;
	color::OpaqueColor fg = color::white;
	color::Color bg = color::transparent;

	inline auto &setPosition(geom::Point position) {
		this->position = position;
		return *this;
	}

	inline auto &setSize(uint8_t size) {
		this->size = size;
		return *this;
	}

	inline auto &setFg(color::OpaqueColor fg) {
		this->fg = fg;
		return *this;
	}

	inline auto &setBg(color::Color bg) {
		this->bg = bg;
		return *this;
	}
};

template<typename Str>
class MutableText: public _BaseElement<TextStyle> {
public:
	inline MutableText(Str text):
		MutableText {text, {}} {};

	inline MutableText(Str text, const TextStyle &style):
		_BaseElement {style},
		text {text} {};

	inline Str getText() const {
		return text;
	}

	inline MutableText &setText(Str text) {
		this->text = text;
		return *this;
	}

private:
	inline void renderTo(Display *tft) const {
		auto isStatic = this->style.position.isPositive();
		geom::Point position;

		if (isStatic) {
			tft->setCursor(this->style.position.x, this->style.position.y);
			position = this->style.position;
		}
		else {
			position = geom::Point {tft->getCursorX(), tft->getCursorY()};
		}

		tft->setTextSize(this->style.size);
		tft->setTextColor(
			this->style.fg,
			this->style.bg == color::transparent ? this->style.fg : this->style.bg
		);

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
	geom::BoxRect box = {-1, -1};
};

using Text = MutableText<const char *>;

}}
