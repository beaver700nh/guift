#pragma once

// #define UI_DEBUG

namespace guift {

class Display;

namespace ui {

template<typename Style>
class _BaseElement {
public:
	_BaseElement(const Style &style) : style {style} {};

	virtual inline Style &getStyle() {
		return style;
	}

	virtual inline _BaseElement<Style> &setStyle(const Style &style) {
		this->style = style;
		return *this;
	}

protected:
	Style style;

private:
	friend class guift::Display;
	virtual void renderTo(Display *tft) const = 0;
};

}}
