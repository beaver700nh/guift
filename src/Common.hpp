#pragma once

#include <stdint.h>

#include <MCUFRIEND_kbv.h>

namespace guift {

// RGB565 format
namespace color {
	inline constexpr uint16_t from24Bit(uint32_t rgb) {
		return 0
			| ((rgb & 0x00F80000) >> 8)
			| ((rgb & 0x0000FC00) >> 5)
			| ((rgb & 0x000000F8) >> 3);
	}

	inline constexpr uint16_t from24Bit(uint8_t r, uint8_t g, uint8_t b) {
		return 0
			| ((r & 0xF8) << 8)
			| ((g & 0xFC) << 3)
			| (b >> 3);
	}

	enum OpaqueColor: uint16_t {
		black       = TFT_BLACK,
		darkGrey    = TFT_DARKGREY,
		darkGray    = darkGrey,
		lightGrey   = TFT_LIGHTGREY,
		lightGray   = lightGrey,
		white       = TFT_WHITE,

		red         = TFT_RED,
		green       = TFT_GREEN,
		blue        = TFT_BLUE,
		cyan        = TFT_CYAN,
		magenta     = TFT_MAGENTA,
		yellow      = TFT_YELLOW,

		maroon      = TFT_MAROON,
		orange      = TFT_ORANGE,
		greenYellow = TFT_GREENYELLOW,
		yellowGreen = greenYellow,
		olive       = TFT_OLIVE,
		darkGreen   = TFT_DARKGREEN,
		darkCyan    = TFT_DARKCYAN,
		navy        = TFT_NAVY,
		purple      = TFT_PURPLE,
		pink        = TFT_PINK,
	};

	enum Color: int32_t {
		transparent = -1,
	};

	inline Color operator+(OpaqueColor color) {
		return static_cast<Color>(color);
	}
}

}
