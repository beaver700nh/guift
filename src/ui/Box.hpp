#pragma once

#include "../Common.hpp"
#include "../Geometry.hpp"
#include "../Display.hpp"
#include "BaseElement.hpp"

namespace guift {

namespace ui {

struct BoxStyle {
	geom::Point position {0, 0};
	geom::Size size {100, 100};
	color::Color fill = +color::gray1;
	color::Color border = +color::gray5;
	uint8_t thickness = 1;
	uint8_t roundness = 1;

	inline auto &setPosition(geom::Point position) {
		this->position = position;
		return *this;
	}

	inline auto &setSize(geom::Size size) {
		this->size = size;
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

	inline auto &setThickness(uint8_t thickness) {
		this->thickness = thickness;
		return *this;
	}

	inline auto &setRoundness(uint8_t roundness) {
		this->roundness = roundness;
		return *this;
	}
};

class Box: public _BaseElement<BoxStyle> {
public:
	inline Box(): Box {{}} {};

	inline Box(const BoxStyle &style): _BaseElement {style} {
		// must leave 1-2px that is not the border
		uint8_t maxBorder = (1 + min(style.size.x, style.size.y)) / 2 - 1;

		// thickness < 1 would just be no border
		this->style.roundness = constrain(style.roundness, 0, maxBorder);
		this->style.thickness = constrain(style.thickness, 1, maxBorder);
	};

private:
	inline void renderTo(Display *tft) const {
		tft->startWrite();

		auto roundOffset = style.position + geom::Point {style.roundness, style.roundness};
		auto roundShrunk = style.size - geom::Size {style.roundness, style.roundness} * 2;

		if (style.fill != color::transparent) {
			tft->writeFillRect(roundOffset.x, style.position.y, roundShrunk.x, style.size.y, style.fill);
			tft->fillCircleHelper(
				roundOffset.x,
				roundOffset.y, style.roundness, 0x2, roundShrunk.y - 1, style.fill
			);
			tft->fillCircleHelper(
				roundOffset.x + roundShrunk.x - 1,
				roundOffset.y, style.roundness, 0x1, roundShrunk.y - 1, style.fill
			);
		}

		auto thickOffset = style.position + geom::Point {style.thickness, style.thickness};
		auto thickShrunk = style.size - geom::Size {style.thickness, style.thickness} * 2;

		if (style.border != color::transparent && style.border != style.fill) {
			tft->writeFillRect(
				style.position.x,
				roundOffset.y,
				style.thickness, roundShrunk.y, style.border
			);
			tft->writeFillRect(
				thickOffset.x + thickShrunk.x,
				roundOffset.y,
				style.thickness, roundShrunk.y, style.border
			);
			tft->writeFillRect(
				roundOffset.x,
				style.position.y,
				roundShrunk.x, style.thickness, style.border
			);
			tft->writeFillRect(
				roundOffset.x,
				thickOffset.y + thickShrunk.y,
				roundShrunk.x, style.thickness, style.border
			);

			for (uint8_t radius = style.roundness; radius > style.roundness - style.thickness; --radius) {
				tft->drawCircleHelper(
					roundOffset.x,
					roundOffset.y,
					radius, 0x1, style.border
				);
				tft->drawCircleHelper(
					roundOffset.x + roundShrunk.x - 1,
					roundOffset.y,
					radius, 0x2, style.border
				);
				tft->drawCircleHelper(
					roundOffset.x,
					roundOffset.y + roundShrunk.y - 1,
					radius, 0x8, style.border
				);
				tft->drawCircleHelper(
					roundOffset.x + roundShrunk.x - 1,
					roundOffset.y + roundShrunk.y - 1,
					radius, 0x4, style.border
				);
			}
		}

		tft->endWrite();
	}
};

}}
