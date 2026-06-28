#pragma once

#include "BaseElement.hpp"

namespace guift {

namespace ui {

class Text: public _BaseElement {
private:
	void renderTo(Display *tft) const override;
};

}}
