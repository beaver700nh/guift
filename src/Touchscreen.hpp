#pragma once

#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

#include "Geometry.hpp"

template<typename Tag, typename Tag::type member>
struct _AccessHack {
	friend typename Tag::type _steal(Tag) {
		return member;
	}
};

#define _AddBackdoor(Class, Type, Victim, property) \
	struct _Backdoor_##property { \
		typedef Type Class::*type; \
		friend type _steal(_Backdoor_##property); \
	}; \
	template struct _AccessHack<_Backdoor_##property, &Victim::property>;

_AddBackdoor(Adafruit_GFX, int16_t, MCUFRIEND_kbv, WIDTH)
_AddBackdoor(Adafruit_GFX, int16_t, MCUFRIEND_kbv, HEIGHT)

namespace guift {

class Touchscreen: private TouchScreen {
public:
	struct Calibration {
		// Origin is at bottom left of orientation 0
		geom::Point xMinYMin, xMaxYMin, xMinYMax, xMaxYMax, center;
	};

	struct Touch: public TSPoint {
		inline Touch() = default;
		inline Touch(int16_t x, int16_t y, int16_t z): TSPoint {x, y, z} {}
		inline Touch(const TSPoint &point): TSPoint {point.x, point.y, point.z} {}

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

	Touch getTouch();
	Touch waitTouch();
	TSPoint getRawTouch();
	TSPoint waitRawTouch();

	Touch correctForRotation(const Touch &p);

	static Calibration runCalibration(uint8_t xp, uint8_t xm, uint8_t yp, uint8_t ym, uint16_t rx, MCUFRIEND_kbv *tft);

private:
	uint8_t _xm, _yp;

	QuadrilateralMode mode;
	Calibration calib;
	MCUFRIEND_kbv *tft;

	geom::Size pixelSize;

	union {
		geom::CornerRect minRect;

		struct {
			geom::Point trueCenter;
			struct {
				float xMinYMin, xMaxYMin, xMinYMax, xMaxYMax;
			} slopes; // of the diagonals
			struct {
				geom::CartesianVec2d<double> xMin, xMax, yMin, yMax;
			} edges;
		} ;
	} memo;

	void memoizeCalculationHelpers();

	Touch calculateFreeTouch(const TSPoint &p);
};

}
