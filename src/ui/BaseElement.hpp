#pragma once

// #define UI_DEBUG

namespace guift {

class Display;

namespace ui {

class _BaseElement {
private:
	friend class guift::Display;
	virtual void renderTo(Display *tft) const = 0;
};

}}
