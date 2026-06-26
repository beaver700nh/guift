#include <Arduino.h>

#include "Touchscreen.hpp"

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
	this->mode = mode;

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

	if (mode == QuadrilateralMode::RECTANGULAR) {
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
	}
	else if (mode == QuadrilateralMode::FREE) {
		return p; // TODO
	}

	__builtin_unreachable();
}

bool Touchscreen::isTouching() {
	return Touch::isValid(getRawTouch());
}

void Touchscreen::waitForTouch() {
	while (!isTouching());
}

}
