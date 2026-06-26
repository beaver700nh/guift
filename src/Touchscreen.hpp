#pragma once

#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

#include "Geometry.hpp"

namespace guift {

class Touchscreen: private TouchScreen {
public:
	struct Calibration {
		// Origin is at bottom left of orientation 0
		geom::Point xMinYMin, xMaxYMin, xMinYMax, xMaxYMax, center;
	};

	struct Touch: public TSPoint {
		Touch() = default;
		Touch(int16_t x, int16_t y, int16_t z): TSPoint {x, y, z} {}
		Touch(const TSPoint &point): TSPoint {point.x, point.y, point.z} {}

		static inline bool isValid(const TSPoint &point) {
			// Pressure might not be supported past a binary extent
			return point.z > 0;
		}
	};

	enum class QuadrilateralMode {
		// Rectangular is faster to calculate but suffers in accuracy, especially
		// for displays with very trapezoidal calibrations.
		FREE,
		RECTANGULAR,
	};

	Touchscreen(uint8_t xp, uint8_t xm, uint8_t yp, uint8_t ym, uint16_t rx, Calibration calib, MCUFRIEND_kbv *tft);

	void begin(QuadrilateralMode mode);

	bool isTouching();
	void waitForTouch();

	Touch getTouch();
	TSPoint getRawTouch();

private:
	uint8_t _xm, _yp;

	QuadrilateralMode mode;
	Calibration calib;
	MCUFRIEND_kbv *tft;

	geom::Size pixelSize;
	geom::CornerRect minRect;
};

}
