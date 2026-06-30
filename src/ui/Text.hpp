#pragma once

#include "../Common.hpp"
#include "../Geometry.hpp"
#include "../Display.hpp"
#include "BaseElement.hpp"

namespace guift {

namespace ui {

struct TextStyle {
	geom::Point position;
	uint8_t size;
	color::OpaqueColor fg;
	color::Color bg;
	color::Color underline;
	color::Color strike;

	struct {
		uint8_t decorThickness = 0;
	} memo;

	inline auto &setPosition(geom::Point position) {
		this->position = position;
		return *this;
	}

	inline auto &setSize(uint8_t size) {
		this->size = size;
		memo.decorThickness = (size + 1) / 2;
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

	inline auto &setUnderline(color::Color underline) {
		this->underline = underline;
		return *this;
	}

	inline auto &setStrike(color::Color strike) {
		this->strike = strike;
		return *this;
	}

	inline TextStyle() {
		setPosition({-1, -1});
		setSize(2);
		setFg(color::white);
		setBg(color::transparent);
		setUnderline(color::transparent);
		setStrike(color::transparent);
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
		tft->setTextColor(
			style.fg,
			style.bg == color::transparent ? style.fg : style.bg
		);

		tft->print(text);

		tft->getTextBounds(
			text, position.x, position.y,
			&box.origin.x, &box.origin.y,
			&box.size.x, &box.size.y
		);

		tft->startWrite();

#ifdef UI_DEBUG
		tft->writeFastHLine(
			box.origin.x, box.origin.y,
			box.size.x, color::yellowGreen);
		tft->writeFastHLine(
			box.origin.x, box.origin.y + box.size.y - 1,
			box.size.x, color::yellowGreen);
		tft->writeFastVLine(
			box.origin.x, box.origin.y + 1,
			box.size.y - 2, color::yellowGreen);
		tft->writeFastVLine(
			box.origin.x + box.size.x - 1, box.origin.y + 1,
			box.size.y - 2, color::yellowGreen);
#endif

		if (style.underline != color::transparent) {
			tft->writeFillRect(
				box.origin.x, box.origin.y + box.size.y - style.memo.decorThickness,
				box.size.x, style.memo.decorThickness, style.underline);
		}

		if (style.strike != color::transparent) {
			tft->writeFillRect(
				box.origin.x, box.origin.y + (box.size.y >> 1) - style.memo.decorThickness,
				box.size.x, style.memo.decorThickness, style.strike);
		}
	}

	Str text;
	geom::BoxRect box = {{-1, -1}};
};

using Text = MutableText<const char *>;

}}
