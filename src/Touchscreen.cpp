#include <Arduino.h>

#include "Touchscreen.hpp"
#include "MCUFRIEND_kbv.h"

namespace guift {

Touchscreen::Touchscreen(uint8_t xp, uint8_t xm, uint8_t yp, uint8_t ym, uint16_t rx, Calibration calib, MCUFRIEND_kbv *tft):
	TouchScreen {xp, yp, xm, ym, rx},
	_xm {xm}, _yp {yp},
	calib {calib},
	tft {tft},
	minRect {
		{
			max(calib.xMinYMin.x, calib.xMinYMax.x),
			max(calib.xMinYMin.y, calib.xMaxYMin.y),
		},
		{
			min(calib.xMaxYMin.x, calib.xMaxYMax.x),
			min(calib.xMinYMax.y, calib.xMaxYMax.y),
		},
	} {}

void Touchscreen::begin(QuadrilateralMode mode) {
	if (minRect.contains(calib.center))
		this->mode = mode;
	else
		// Rectangular mode doesn't use the center point; fall back to it if the
		// calibration is really messed up.
 		this->mode = QuadrilateralMode::RECTANGULAR;

	auto w = tft->width();  if (w < 0) w = 0;
	auto h = tft->height(); if (h < 0) h = 0;

	pixelSize = {
		static_cast<uint16_t>(w),
		static_cast<uint16_t>(h),
	};
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

	if (mode == QuadrilateralMode::RECTANGULAR) {
		p.x = constrain(p.x, minRect.a.x, minRect.b.x);
		p.y = constrain(p.y, minRect.a.y, minRect.b.y);
		p.z = 1; // TEMP

		switch (tft->getRotation()) {
		case 0: return
			{	(int16_t) map(p.x, minRect.a.x, minRect.b.x, 0, pixelSize.x),
				(int16_t) map(p.y, minRect.a.y, minRect.b.y, pixelSize.y, 0),
				p.z };
		case 2: return
			{	(int16_t) map(p.x, minRect.a.x, minRect.b.x, pixelSize.x, 0),
				(int16_t) map(p.y, minRect.a.y, minRect.b.y, 0, pixelSize.y),
				p.z };
		case 3: return
			{	(int16_t) map(p.y, minRect.a.y, minRect.b.y, 0, pixelSize.x),
				(int16_t) map(p.x, minRect.a.x, minRect.b.x, 0, pixelSize.y),
				p.z };
		default:
		case 1: return
			{	(int16_t) map(p.y, minRect.a.y, minRect.b.y, pixelSize.x, 0),
				(int16_t) map(p.x, minRect.a.x, minRect.b.x, pixelSize.y, 0),
				p.z };
		}
	}
	else if (mode == QuadrilateralMode::FREE) {
		char buf[128];
		if (p.y < calib.center.y) {
			double leftBound = calib.xMinYMin.x + (double) (p.y - calib.xMinYMin.y)
				* (double) (calib.center.x - calib.xMinYMin.x)
				/ (double) (calib.center.y - calib.xMinYMin.y);
			Serial.print(leftBound);
			Serial.print(" ");

			if (p.x < leftBound) {
				p.z = 0x7800;
				goto out;
			}

			double rightBound = calib.xMaxYMin.x + (double) (p.y - calib.xMaxYMin.y)
				* (double) (calib.center.x - calib.xMaxYMin.x)
				/ (double) (calib.center.y - calib.xMaxYMin.y);
			Serial.print(rightBound);
			Serial.print(" ");

			if (p.x >= rightBound) {
				p.z = TFT_BLUE;
				goto out;
			}
			Serial.print("neither");

			p.z = 0x79E0;
			goto out;
		}
		else {
			double leftBound = calib.xMinYMax.x + (double) (p.y - calib.xMinYMax.y)
				* (double) (calib.center.x - calib.xMinYMax.x)
				/ (double) (calib.center.y - calib.xMinYMax.y);

			if (p.x < leftBound) {
				p.z = 0x7800;
				goto out;
			}

			double rightBound = calib.xMaxYMax.x + (double) (p.y - calib.xMaxYMax.y)
				* (double) (calib.center.x - calib.xMaxYMax.x)
				/ (double) (calib.center.y - calib.xMaxYMax.y);

			if (p.x >= rightBound) {
				p.z = TFT_BLUE;
				goto out;
			}

			p.z = TFT_CYAN;
			goto out;
		}

out:
		Serial.println();
		p.x = constrain(p.x, minRect.a.x, minRect.b.x);
		p.y = constrain(p.y, minRect.a.y, minRect.b.y);

		switch (tft->getRotation()) {
		case 0: return
			{	(int16_t) map(p.x, minRect.a.x, minRect.b.x, 0, pixelSize.x),
				(int16_t) map(p.y, minRect.a.y, minRect.b.y, pixelSize.y, 0),
				p.z };
		case 2: return
			{	(int16_t) map(p.x, minRect.a.x, minRect.b.x, pixelSize.x, 0),
				(int16_t) map(p.y, minRect.a.y, minRect.b.y, 0, pixelSize.y),
				p.z };
		case 3: return
			{	(int16_t) map(p.y, minRect.a.y, minRect.b.y, 0, pixelSize.x),
				(int16_t) map(p.x, minRect.a.x, minRect.b.x, 0, pixelSize.y),
				p.z };
		default:
		case 1: return
			{	(int16_t) map(p.y, minRect.a.y, minRect.b.y, pixelSize.x, 0),
				(int16_t) map(p.x, minRect.a.x, minRect.b.x, pixelSize.y, 0),
				p.z };
		}
		return p;
	}

	// All enum cases covered
	__builtin_unreachable();
}

bool Touchscreen::isTouching() {
	return Touch::isValid(getRawTouch());
}

void Touchscreen::waitForTouch() {
	while (!isTouching());
}

}
