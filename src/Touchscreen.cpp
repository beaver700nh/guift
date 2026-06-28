#include <Arduino.h>

#include "MCUFRIEND_kbv.h"
#include "Geometry.hpp"
#include "Touchscreen.hpp"

namespace guift {

Touchscreen::Touchscreen(uint8_t xp, uint8_t xm, uint8_t yp, uint8_t ym, uint16_t rx, Calibration calib, MCUFRIEND_kbv *tft):
	TouchScreen {xp, yp, xm, ym, rx},
	_xm {xm}, _yp {yp},
	calib {calib},
	tft {tft} {}

void Touchscreen::begin(QuadrilateralMode mode) {
	this->mode = mode;
	memoizeCalculationHelpers();

	auto w = tft->width();  if (w < 0) w = 0;
	auto h = tft->height(); if (h < 0) h = 0;

	pixelSize = {
		static_cast<uint16_t>(w),
		static_cast<uint16_t>(h),
	};
}

void Touchscreen::memoizeCalculationHelpers() {
	switch (mode) {
	case QuadrilateralMode::RECTANGULAR:
		memo.minRect = {
			{
				static_cast<uint16_t>(max(calib.xMinYMin.x, calib.xMinYMax.x)),
				static_cast<uint16_t>(max(calib.xMinYMin.y, calib.xMaxYMin.y)),
			},
			{
				static_cast<uint16_t>(min(calib.xMaxYMin.x, calib.xMaxYMax.x)),
				static_cast<uint16_t>(min(calib.xMinYMax.y, calib.xMaxYMax.y)),
			},
		};

		break;

	case QuadrilateralMode::FREE:
		memo.slopes.xMinYMin =
			((float) calib.center.x - calib.xMinYMin.x) /
			((float) calib.center.y - calib.xMinYMin.y);
		memo.slopes.xMaxYMin =
			((float) calib.center.x - calib.xMaxYMin.x) /
			((float) calib.center.y - calib.xMaxYMin.y);
		memo.slopes.xMinYMax =
			((float) calib.center.x - calib.xMinYMax.x) /
			((float) calib.center.y - calib.xMinYMax.y);
		memo.slopes.xMaxYMax =
			((float) calib.center.x - calib.xMaxYMax.x) /
			((float) calib.center.y - calib.xMaxYMax.y);

		memo.angles.xMin = (calib.xMinYMax - calib.xMinYMin).scaledBy(1.0 / calib.xMinYMin.distanceTo(calib.xMinYMax));
		memo.angles.xMax = (calib.xMaxYMax - calib.xMaxYMin).scaledBy(1.0 / calib.xMaxYMin.distanceTo(calib.xMaxYMax));
		memo.angles.yMin = (calib.xMaxYMin - calib.xMinYMin).scaledBy(1.0 / calib.xMinYMin.distanceTo(calib.xMaxYMin));
		memo.angles.yMax = (calib.xMaxYMax - calib.xMinYMax).scaledBy(1.0 / calib.xMinYMax.distanceTo(calib.xMaxYMax));
		Serial.println(String("angles: ") + memo.angles.xMin + ", " + memo.angles.xMax + ", " + memo.angles.yMin + ", " + memo.angles.yMax);

		break;

	default:
		// All enum cases covered
		__builtin_unreachable();
	}
}

TSPoint Touchscreen::getRawTouch() {
	TSPoint p = getPoint();

	pinMode(_xm, OUTPUT);
	pinMode(_yp, OUTPUT);

	return p;
}

Touchscreen::Touch Touchscreen::getTouch() {
	TSPoint p = getRawTouch();

	if (!Touch::isValid(p))
		return {0, 0, 0};

	switch (mode) {
	case QuadrilateralMode::RECTANGULAR:
		p.x = constrain(p.x, memo.minRect.a.x, memo.minRect.b.x);
		p.y = constrain(p.y, memo.minRect.a.y, memo.minRect.b.y);
		p.z = 1; // TEMP

		switch (tft->getRotation()) {
		case 0: return
			{	(int16_t) map(p.x, memo.minRect.a.x, memo.minRect.b.x, 0, pixelSize.x),
				(int16_t) map(p.y, memo.minRect.a.y, memo.minRect.b.y, pixelSize.y, 0),
				p.z };
		case 2: return
			{	(int16_t) map(p.x, memo.minRect.a.x, memo.minRect.b.x, pixelSize.x, 0),
				(int16_t) map(p.y, memo.minRect.a.y, memo.minRect.b.y, 0, pixelSize.y),
				p.z };
		case 3: return
			{	(int16_t) map(p.y, memo.minRect.a.y, memo.minRect.b.y, 0, pixelSize.x),
				(int16_t) map(p.x, memo.minRect.a.x, memo.minRect.b.x, 0, pixelSize.y),
				p.z };
		default:
		case 1: return
			{	(int16_t) map(p.y, memo.minRect.a.y, memo.minRect.b.y, pixelSize.x, 0),
				(int16_t) map(p.x, memo.minRect.a.x, memo.minRect.b.x, pixelSize.y, 0),
				p.z };
		}

	case QuadrilateralMode::FREE:
		if (p.y < calib.center.y) {
			uint16_t leftBound = calib.xMinYMin.x + memo.slopes.xMinYMin * ((float) p.y - calib.xMinYMin.y);

			if (p.x < leftBound) {
				p.z = 0x7800;
				/*
				 * x' = x cos - y sin
				 * y' = x sin + y cos
				 */
				Serial.println(String("before ") + geom::CartesianVec2d<int16_t>{p.x, p.y});
				p.x = p.x * memo.angles.xMin.y - p.y * memo.angles.xMin.x;
				p.y = p.x * memo.angles.yMin.x + p.y * memo.angles.yMin.y;
				Serial.println(String("after ") + geom::CartesianVec2d<int16_t>{p.x, p.y});
				return p;
			}

			uint16_t rightBound = calib.xMaxYMin.x + memo.slopes.xMaxYMin * ((float) p.y - calib.xMaxYMin.y);

			if (p.x >= rightBound) {
				p.z = TFT_BLUE;
				goto out;
			}

			p.z = 0x79E0;

			auto edgeVector = calib.xMaxYMin - calib.xMinYMin;
			auto diagVector = calib.xMinYMin - calib.center;
			auto pointVector = geom::Point {p.x, p.y} - calib.center;

			Serial.println(edgeVector);
			Serial.println(diagVector);
			Serial.println(pointVector);

			double inverseT = 1.0
				* (edgeVector.x * pointVector.y - edgeVector.y * pointVector.x)
				/ (edgeVector.x * diagVector.y - edgeVector.y * diagVector.x);

			auto hitPoint = pointVector.scaledBy(inverseT) + calib.center;
			double edgeT = hitPoint.distanceTo(calib.xMinYMin) / edgeVector.length();

			geom::Point center(pixelSize.x / 2, pixelSize.y / 2);
			auto outVector = geom::Point {static_cast<int16_t>(pixelSize.x * edgeT), 0} - center;
			auto result = center + outVector * inverseT;

			Serial.println(String("inverseT: ") + inverseT + ", edgeT: " + edgeT);

			p.x = result.x;
			p.y = result.y;
		}
		else if (true
			&& p.x > calib.xMinYMax.x + memo.slopes.xMinYMax * (p.y - calib.xMinYMax.y)
			&& p.x < calib.xMaxYMax.x + memo.slopes.xMaxYMax * (p.y - calib.xMaxYMax.y)
		) {
			p.z = TFT_CYAN;
		}
		else if (p.x < calib.center.x) {
			p.z = 0x7800;
		}
		else {
			uint16_t leftBound = calib.xMinYMax.x + memo.slopes.xMinYMax * ((float) p.y - calib.xMinYMax.y);

			if (p.x < leftBound) {
				p.z = 0x7800;
				goto out;
			}

			uint16_t rightBound = calib.xMaxYMax.x + memo.slopes.xMaxYMax * ((float) p.y - calib.xMaxYMax.y);

			if (p.x >= rightBound) {
				p.z = TFT_BLUE;
				goto out;
			}

			geom::Point edgePoint(pixelSize.x - 1, pixelSize.y - rayPoint.distanceTo(calib.xMaxYMin) * scaleFactor - 1);
			auto result = memo.free.trueCenter + (edgePoint - memo.free.trueCenter) / rayT;

			p.x = result.x;
			p.y = result.y;
		}

	out:
		p.x = p.y = 0;
		return p;

	default:
		// All enum cases covered
		__builtin_unreachable();
	}
}

bool Touchscreen::isTouching() {
	return Touch::isValid(getRawTouch());
}

void Touchscreen::waitForTouch() {
	while (!isTouching());
}

}
