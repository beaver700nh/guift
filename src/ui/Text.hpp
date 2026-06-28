#pragma once

#include "../Common.hpp"
#include "../Geometry.hpp"
#include "../Display.hpp"
#include "BaseElement.hpp"

namespace guift {

namespace ui {

template<typename String>
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

	inline MutableText(String text):
		MutableText {text, {}} {};
	inline MutableText(String text, const Style &style):
		text {text},
		style {style} {};

	static inline Style &of(String text) {
		return MutableText {text}.getStyle();
	}

	inline String getText() const {
		return text;
	}

	inline MutableText &setText(String text) {
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
		if (style.position.x >= 0 && style.position.y >= 0) {
			tft->setCursor(style.position.x, style.position.y);
		}

		tft->setTextSize(style.size);
		tft->setTextColor(style.fg, style.bg == color::transparent ? style.fg : style.bg);

		tft->print(text);
	}

	String text;
	Style style;
	// geom::BoxRect boundingBox;
};

using Text = MutableText<const char *>;

}}
