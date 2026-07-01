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

class Button;

// Single-line text only!
template<typename Str>
class GenericText: public _BaseElement<TextStyle> {
public:
	inline GenericText(Str text):
		GenericText {text, {}} {};

	inline GenericText(Str text, const TextStyle &style):
		_BaseElement {style},
		text {text} {};

	inline Str getText() const {
		return text;
	}

	inline GenericText &setText(Str text) {
		this->text = text;
		return *this;
	}

	inline geom::Point getPosition(Display *tft) const {
		return style.position.isPositive()
			? style.position
			: tft->getCursor();
	}

private:
	inline void configureText(Display *tft) {
		auto position = getPosition(tft);

		if (position == style.position)
			tft->setCursor(position);

		tft->setTextSize(style.size);
		tft->setTextColor(
			style.fg,
			(style.bg, style.fg)
		);

		tft->getTextBounds(
			text,
			geom_xy(position),
			geom_xy(&box.origin),
			geom_xy(&box.size)
		);
	}

	inline void renderTo(Display *tft) override {
		configureText(tft);
		tft->print(text);

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

		tft->endWrite();
	}

	Str text;
	geom::BoxRect box = {{-1, -1}};

	friend class Button;

public:
	// Pass nullptr to skip recalculation
	inline geom::BoxRect getBox(Display *tft) {
		if (tft != nullptr) {
			auto saved = style.position;
			style.position = {0, tft->height()}; // render off-screen
			configureText(tft);
			style.position = saved;
		}

		return box;
	}
};

using Text = GenericText<const char *>;

}}
