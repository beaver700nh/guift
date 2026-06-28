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
		grey1       = 0x0861,        // 0000'1 000'011 0'0001
		gray1       = grey1,
		grey2       = 0x18E3,        // 0001'1 000'111 0'0011
		gray2       = grey2,
		grey3       = 0x2965,        // 0010'1 001'011 0'0101
		gray3       = grey3,
		grey4       = 0x4248,        // 0100'0 010'010 0'1000
		gray4       = grey4,
		grey5       = 0x5AEB,        // 0101'1 010'111 0'1011
		gray5       = grey5,
		grey6       = TFT_DARKGREY,  // 0111'1 011'111 0'1111
		gray6       = grey6,
		grey7       = TFT_LIGHTGREY, // 1100'0 110'000 1'1000
		gray7       = grey7,
		grey8       = 0xEF9D,        // 1110'1 111'100 1'1101
		gray8       = grey8,
		grey9       = 0xF7DE,        // 1111'0 111'110 1'1110
		gray9       = grey9,
		white       = TFT_WHITE,

		// backwards-compatibility
		darkGrey    = grey6,
		darkGray    = darkGrey,
		lightGrey   = grey7,
		lightGray   = lightGrey,

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
