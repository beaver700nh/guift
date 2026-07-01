#pragma once

#include "../Geometry.hpp"
#include "../Display.hpp"
#include "BaseElement.hpp"
#include "Box.hpp"
#include "Text.hpp"

namespace guift {

namespace ui {

struct ButtonStyle {
	enum class TextAlignment1d {
		none,
		pack,
		center,
		start, left = start, top = start,
		end,   right = end,  bottom = end,
	};

	using TextAlignment = geom::CartesianVec2d<TextAlignment1d>;

	TextAlignment alignment;
	geom::Point textOffset;

	// "Memoized" values can't subscribe to inputs like they usually would because
	// child elements' inputs are separate and inaccessible. Instead, manually
	// mark the button style as dirty to trigger recalculation.
	bool dirty;

	inline auto &markDirty(bool dirty = true) {
		this->dirty = dirty;
		return *this;
	}

	inline auto &useTextOffset(geom::Point textOffset) {
		this->alignment = TextAlignment::from(TextAlignment1d::none);
		this->textOffset = textOffset;
		return *this;
	}

	inline auto &useTextAlignment(TextAlignment alignment, geom::Point offset = {0, 0}) {
		this->alignment = alignment;
		this->textOffset = offset;
		dirty = true;
		return *this;
	}

	inline auto &usePackedText(geom::Size padding = {0, 0}) {
		this->alignment = TextAlignment::from(TextAlignment1d::pack);
		this->textOffset = geom::Point::from(geom_xy(padding));
		dirty = true;
		return *this;
	}

	inline ButtonStyle() {
		usePackedText({10, 6});
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

	inline void calculateText(Display *tft) {
		auto textBox = text.getBox(tft);
		auto &boxStyle = box.getStyle();

		using Align = ButtonStyle::TextAlignment1d;

#define calculateAlignment(axis) \
		switch (style.alignment.axis) { \
		default: \
			break; \
		}

		if (style.alignment.x == Align::pack)
			boxStyle.setSize(geom::Size::from(textBox.size.x + style.textOffset.x * 2, boxStyle.size.y));
		else
			calculateAlignment(x);

		if (style.alignment.y == Align::pack)
			boxStyle.setSize(geom::Size::from(boxStyle.size.x, textBox.size.y + style.textOffset.y * 2));
		else
			calculateAlignment(y);

#undef calculateAlignment
	}

	inline void renderTo(Display *tft) override {
		if (style.dirty) {
			calculateText(tft);
			style.markDirty(false);
		}

		box.renderTo(tft);

		tft->setCursor(box.getStyle().position + style.textOffset);
		text.renderTo(tft);
	}
};

}}
