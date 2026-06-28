#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

#include "Common.hpp"
#include "Geometry.hpp"
#include "TouchScreen.h"
#include "Touchscreen.hpp"

namespace guift {

Touchscreen::Touchscreen(uint8_t xp, uint8_t xm, uint8_t yp, uint8_t ym, uint16_t rx, Calibration calib, MCUFRIEND_kbv *tft):
	TouchScreen {xp, yp, xm, ym, rx},
	_xm {xm}, _yp {yp},
	calib {calib},
	tft {tft} {}

void Touchscreen::begin(QuadrilateralMode mode) {
	auto w = tft->*_steal(_Backdoor_WIDTH {});
	auto h = tft->*_steal(_Backdoor_HEIGHT {});

	pixelSize = {
		static_cast<uint16_t>(w),
		static_cast<uint16_t>(h),
	};

	this->mode = mode;
	memoizeCalculationHelpers();
}

void Touchscreen::memoizeCalculationHelpers() {
	switch (mode) {
	case QuadrilateralMode::RECTANGULAR:
		memo.minRect = {
			{
				max(calib.xMinYMin.x, calib.xMinYMax.x),
				max(calib.xMinYMin.y, calib.xMaxYMin.y),
			},
			{
				min(calib.xMaxYMin.x, calib.xMaxYMax.x),
				min(calib.xMinYMax.y, calib.xMaxYMax.y),
			},
		};

		break;

	case QuadrilateralMode::FREE:
		memo.trueCenter = geom::Point::from(pixelSize.x / 2, pixelSize.y / 2);

		memo.slopes.xMinYMin = (float)
			(calib.center.x - calib.xMinYMin.x) /
			(calib.center.y - calib.xMinYMin.y);
		memo.slopes.xMaxYMin = (float)
			(calib.center.x - calib.xMaxYMin.x) /
			(calib.center.y - calib.xMaxYMin.y);
		memo.slopes.xMinYMax = (float)
			(calib.center.x - calib.xMinYMax.x) /
			(calib.center.y - calib.xMinYMax.y);
		memo.slopes.xMaxYMax = (float)
			(calib.center.x - calib.xMaxYMax.x) /
			(calib.center.y - calib.xMaxYMax.y);

		memo.edges.xMin = calib.xMinYMax - calib.xMinYMin;
		memo.edges.xMax = calib.xMaxYMax - calib.xMaxYMin;
		memo.edges.yMin = calib.xMaxYMin - calib.xMinYMin;
		memo.edges.yMax = calib.xMaxYMax - calib.xMinYMax;

		break;

	default:
		// All enum cases covered
		__builtin_unreachable();
	}
}

bool Touchscreen::isTouching() {
	return Touch::isValid(getRawTouch());
}

TSPoint Touchscreen::waitRawTouch() {
	TSPoint touch;
	while (!Touch::isValid(touch = getRawTouch()));
	return touch;
}

TSPoint Touchscreen::getRawTouch() {
	TSPoint p = getPoint();

	pinMode(_xm, OUTPUT);
	pinMode(_yp, OUTPUT);

	return p;
}

Touchscreen::Touch Touchscreen::waitTouch() {
	Touch touch;
	while (!Touch::isValid(touch = getTouch()));
	return touch;
}

Touchscreen::Touch Touchscreen::getTouch() {
	TSPoint p = getRawTouch();

	if (!Touch::isValid(p))
		return {0, 0, -1};

	switch (mode) {
	case QuadrilateralMode::RECTANGULAR:
		p.x = map(p.x, memo.minRect.a.x, memo.minRect.b.x, 0, pixelSize.x - 1); // here issue
		p.y = map(p.y, memo.minRect.a.y, memo.minRect.b.y, pixelSize.y - 1, 0);
		break;

	case QuadrilateralMode::FREE:
		p = calculateFreeTouch(p);
		break;

	default:
		// All enum cases covered
		__builtin_unreachable();
	}

	return correctForRotation(
		{	static_cast<int16_t>(constrain(p.x, 0, pixelSize.x - 1)),
			static_cast<int16_t>(constrain(p.y, 0, pixelSize.y - 1)),
			p.z }
	);
}

/*
 * A ray cast through the touch point follows this parametric line, where t = 1
 * denotes the touch point:
 *
 * (origin.x + t * (p.x - origin.x), origin.y + t * (p.y - origin.y))
 *
 * By the calculation below we can extend the ray further until it hits the
 * calibration quadrilateral; the resulting t-value tells how far it extended.
 *
 * We can then find the hit point's t-value along the quadrilateral's edge with
 * a distance ratio; these two t-values together allow us to reconstruct the
 * touch point in the desired output space.
 */

struct ReconstructionData {
	double rayT, edgePixels;
};

ReconstructionData calculateRaycast(
	geom::Point rayOrigin, geom::Point pointVector, geom::Point corner,
	geom::CartesianVec2d<double> edgeVector, uint16_t totalEdgePixels
) {
	ReconstructionData data;
	geom::Point cornerVector = corner - rayOrigin;

	data.rayT = 1.0
		* (edgeVector.x * cornerVector.y - edgeVector.y * cornerVector.x)
		/ (edgeVector.x * pointVector.y - edgeVector.y * pointVector.x);

	data.edgePixels = (pointVector.scaledBy(data.rayT) + rayOrigin)
		.distanceTo(corner) * totalEdgePixels / edgeVector.length();

	return data;
}

Touchscreen::Touch Touchscreen::calculateFreeTouch(const TSPoint &p) {
	auto pointVector = geom::Point {p.x, p.y} - calib.center;
	ReconstructionData data;
	geom::Point edgePoint;

	if (true
		&& p.x > calib.xMinYMin.x + memo.slopes.xMinYMin * (p.y - calib.xMinYMin.y)
		&& p.x < calib.xMaxYMin.x + memo.slopes.xMaxYMin * (p.y - calib.xMaxYMin.y)) {
		data = calculateRaycast(calib.center, pointVector, calib.xMinYMin, memo.edges.yMin, pixelSize.x);
		edgePoint = geom::Point::from(data.edgePixels, pixelSize.y - 1);
	}
	else if (true
		&& p.x > calib.xMinYMax.x + memo.slopes.xMinYMax * (p.y - calib.xMinYMax.y)
		&& p.x < calib.xMaxYMax.x + memo.slopes.xMaxYMax * (p.y - calib.xMaxYMax.y)) {
		data = calculateRaycast(calib.center, pointVector, calib.xMinYMax, memo.edges.yMax, pixelSize.x);
		edgePoint = geom::Point::from(data.edgePixels, 0);
	}
	else if (p.x < calib.center.x) {
		data = calculateRaycast(calib.center, pointVector, calib.xMinYMin, memo.edges.xMin, pixelSize.y);
		edgePoint = geom::Point::from(0, pixelSize.y - 1 - data.edgePixels);
	}
	else {
		data = calculateRaycast(calib.center, pointVector, calib.xMaxYMin, memo.edges.xMax, pixelSize.y);
		edgePoint = geom::Point::from(pixelSize.x - 1, pixelSize.y - 1 - data.edgePixels);
	}

	auto result = memo.trueCenter + (edgePoint - memo.trueCenter) / data.rayT;
	return {result.x, result.y, p.z};
}

Touchscreen::Touch Touchscreen::correctForRotation(const Touch &p) {
	switch (tft->getRotation()) {
	case 1: return
		{	static_cast<int16_t>(p.y),
			static_cast<int16_t>(pixelSize.x - p.x - 1),
			p.z };
	case 2: return
		{	static_cast<int16_t>(pixelSize.x - p.x - 1),
			static_cast<int16_t>(pixelSize.y - p.y - 1),
			p.z };
	case 3: return
		{	static_cast<int16_t>(pixelSize.y - p.y - 1),
			static_cast<int16_t>(p.x),
			p.z };
	default:
	case 0: return p;
	}
}

Touchscreen::Calibration Touchscreen::runCalibration(uint8_t xp, uint8_t xm, uint8_t yp, uint8_t ym, uint16_t rx, MCUFRIEND_kbv *tft) {
	Touchscreen ts {xp, xm, yp, ym, rx, {}, tft};
	Calibration calib;

	tft->setRotation(0);

	auto w = tft->width();
	auto h = tft->height();

	tft->fillCircle(0, h - 1, 5, color::green);
	TSPoint xMinYMin = ts.waitRawTouch();
	calib.xMinYMin = {xMinYMin.x, xMinYMin.y};
	tft->fillCircle(0, h - 1, 5, 0x0000);
	delay(1000);

	tft->fillCircle(w - 1, h - 1, 5, color::green);
	TSPoint xMaxYMin = ts.waitRawTouch();
	calib.xMaxYMin = {xMaxYMin.x, xMaxYMin.y};
	tft->fillCircle(w - 1, h - 1, 5, 0x0000);
	delay(1000);

	tft->fillCircle(0, 0, 5, color::green);
	TSPoint xMinYMax = ts.waitRawTouch();
	calib.xMinYMax = {xMinYMax.x, xMinYMax.y};
	tft->fillCircle(0, 0, 5, 0x0000);
	delay(1000);

	tft->fillCircle(w - 1, 0, 5, color::green);
	TSPoint xMaxYMax = ts.waitRawTouch();
	calib.xMaxYMax = {xMaxYMax.x, xMaxYMax.y};
	tft->fillCircle(w - 1, 0, 5, 0x0000);
	delay(1000);

	tft->fillCircle(w/2, h/2, 3, color::green);
	TSPoint center = ts.waitRawTouch();
	calib.center = {center.x, center.y};
	tft->fillCircle(w/2, h/2, 3, 0x0000);
	delay(1000);

	return calib;
}

}
