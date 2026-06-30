#pragma once

#include "../Geometry.hpp"
#include "../Display.hpp"
#include "BaseElement.hpp"
#include "Box.hpp"
#include "Text.hpp"

namespace guift {

namespace ui {

struct ButtonStyle {
	bool packed;

	enum class TextAlignment1d {
		none,
		start,
		center,
		end,
		left = start,
		right = end,
		top = start,
		bottom = end,
	};

	using TextAlignment = geom::CartesianVec2d<TextAlignment1d>;

	union {
		struct {
			TextAlignment alignment;
			geom::Point textOffset;
		};

		geom::Size padding;
	};

	// "Memoized" values can't be subscribed like they usually are due to the
	// child elements' having separate style objects. Instead, manually mark the
	// button as dirty to trigger recalculation.
	bool dirty;

	inline auto &markDirty(bool dirty = true) {
		this->dirty = dirty;
		return *this;
	}

	inline auto &setTextOffset(geom::Point textOffset) {
		this->alignment = TextAlignment::from(TextAlignment1d::none);
		this->textOffset = textOffset;
		return *this;
	}

	inline auto &useTextOffset(geom::Point textOffset) {
		packed = false;
		return setTextOffset(textOffset);
	}

	inline auto &useTextAlignment(TextAlignment alignment, uint16_t offsetY) {
		packed = false;
		this->alignment = alignment;
		this->textOffset.y = offsetY;
		dirty = true;
		return *this;
	}

	inline auto &usePackedText(geom::Size padding = {0, 0}) {
		packed = true;
		this->padding = padding;
		dirty = true;
		return *this;
	}

	inline ButtonStyle() {
		setTextOffset({0, 0});
	}
};

class Button: public _BaseElement<ButtonStyle> {
public:
	inline Button() = delete;

	template<typename BoxT, typename TextT>
	inline Button(const ButtonStyle &style, BoxT &&box, TextT &&text):
		_BaseElement {style},
		box {box},
		text {text}
	{
		text.getStyle()
			.setPosition({-1, -1}); // enables setCursor() positioning
	};

private:
	Box &box;
	Text &text;

	inline void calculateTextOffset(Display *tft) {
		if (style.packed) {
			// Render off-screen to get bounding box
			tft->setCursor(tft->getDimensions());
			text.renderTo(tft);

			box.getStyle()
				.setSize(text.box.size + style.padding * 2);

			return;
		}

		// TODO refactor to allow packing both axes separately
		switch (style.alignment.x) {
		case ButtonStyle::TextAlignment1d::start:
			style.textOffset.x = 0;
			break;
		case ButtonStyle::TextAlignment1d::center:
			style.textOffset.x = (box.getStyle().size.x - text.box.size.x) / 2;
			break;
		case ButtonStyle::TextAlignment1d::end:
			style.textOffset.x = box.getStyle().size.x - text.box.size.x;
			break;
		default:
			// Ignore
			break;
		}
	}

	inline void renderTo(Display *tft) override {
		if (style.dirty) {
			calculateTextOffset(tft);
			style.markDirty(false);
		}

		box.renderTo(tft);

		if (style.packed) {
			tft->setCursor(box.getStyle().position + style.padding);
		}
		else {
			tft->setCursor(box.getStyle().position + style.textOffset);
		}

		text.renderTo(tft);
	}
};

}}
