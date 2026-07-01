#pragma once

#include "../Common.hpp"
#include "../Geometry.hpp"
#include "../Display.hpp"
#include "BaseElement.hpp"

namespace guift {

namespace ui {

struct BoxStyle {
	geom::Point position;
	geom::Size size;
	color::Color fill;
	color::Color border;
	uint16_t thickness;
	uint16_t roundness;

	struct {
		uint8_t maxBorder;
		geom::Point thickPosition;
		geom::Size thickSize;
		geom::Point roundPosition;
		geom::Point roundPosition2; // opposite corner
		geom::Size roundSize;
	} _memo;

	inline auto &setPosition(geom::Point position) {
		this->position = position;
		_updateThickPosition();
		_updateRoundPosition();
		_updateRoundPosition2();

		return *this;
	}

	inline auto &setThickness(uint16_t thickness) {
		thickness = constrain(thickness, 1, _memo.maxBorder);

		this->thickness = thickness;
		_updateThickPosition();
		_updateThickSize();

		return *this;
	}

	inline auto &setRoundness(uint16_t roundness) {
		roundness = constrain(roundness, 0, _memo.maxBorder);

		this->roundness = roundness;
		_updateRoundPosition();
		_updateRoundSize();
		_updateRoundPosition2();

		return *this;
	}

	inline auto &setSize(geom::Size size) {
		// must leave 1-2px that is not the border
		_memo.maxBorder = (1 + min(size.x, size.y)) / 2 - 1;

		this->size = size;
		setThickness(thickness);
		setRoundness(roundness);

		return *this;
	}

	inline auto &setFill(color::Color fill) {
		this->fill = fill;
		return *this;
	}

	inline auto &setBorder(color::Color border) {
		this->border = border;
		return *this;
	}

	inline BoxStyle() {
		setPosition({0, 0});
		setSize({100, 100});
		setFill(+color::gray1);
		setBorder(+color::gray5);
		setThickness(2);
		setRoundness(0);
	}

private:
	inline void _updateThickPosition() {
		_memo.thickPosition = position + geom::Point::from(thickness);
	}
	inline void _updateThickSize() {
		_memo.thickSize = size - geom::Size::from(thickness) * 2;
	}
	inline void _updateRoundPosition() {
		_memo.roundPosition = position + geom::Point::from(roundness);
	}
	inline void _updateRoundSize() {
		_memo.roundSize = size - geom::Size::from(roundness) * 2;
	}
	inline void _updateRoundPosition2() {
		_memo.roundPosition2 = _memo.roundPosition + _memo.roundSize;
	}
};

class Button;

class Box: public _BaseElement<BoxStyle> {
public:
	inline Box():
		Box {{}} {};

	inline Box(const BoxStyle &style):
		_BaseElement {style} {};

private:
	inline void renderTo(Display *tft) override {
		tft->startWrite();

		if (style.fill != color::transparent) {
			tft->writeFillRect(
				style._memo.roundPosition.x, style.position.y,
				style._memo.roundSize.x, style.size.y, style.fill);
			tft->fillCircleHelper(
				style._memo.roundPosition.x,
				style._memo.roundPosition.y, style.roundness, 0x2, style._memo.roundSize.y - 1, style.fill);
			tft->fillCircleHelper(
				style._memo.roundPosition2.x - 1,
				style._memo.roundPosition.y, style.roundness, 0x1, style._memo.roundSize.y - 1, style.fill);
		}

		if (style.border != color::transparent && style.border != style.fill) {
			for (uint8_t i = 0; i < style.thickness; ++i) {
				tft->drawCircleHelper(
					style._memo.roundPosition.x,
					style._memo.roundPosition.y,
					style.roundness - i, 0x1, style.border);
				tft->writeFastHLine(
					style._memo.roundPosition.x,
					style.position.y + i,
					style._memo.roundSize.x, style.border);
				tft->drawCircleHelper(
					style._memo.roundPosition2.x - 1,
					style._memo.roundPosition.y,
					style.roundness - i, 0x2, style.border);
				tft->writeFastVLine(
					style.position.x + style.size.x - 1 - i,
					style._memo.roundPosition.y,
					style._memo.roundSize.y, style.border);
				tft->drawCircleHelper(
					style._memo.roundPosition2.x - 1,
					style._memo.roundPosition2.y - 1,
					style.roundness - i, 0x4, style.border);
				tft->writeFastHLine(
					style._memo.roundPosition.x,
					style.position.y + style.size.y - 1 - i,
					style._memo.roundSize.x, style.border);
				tft->drawCircleHelper(
					style._memo.roundPosition.x,
					style._memo.roundPosition2.y - 1,
					style.roundness - i, 0x8, style.border);
				tft->writeFastVLine(
					style.position.x + i,
					style._memo.roundPosition.y,
					style._memo.roundSize.y, style.border);
			}
		}

		tft->endWrite();
	}

	friend class Button;
};

}}
