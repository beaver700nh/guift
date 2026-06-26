#include <Arduino.h>
#include <MCUFRIEND_kbv.h>

MCUFRIEND_kbv tft;

void setup() {
	tft.begin(0x9486);
	tft.setRotation(1);
}

void loop() {
	uint16_t color = 0;

	for (float y = 0; y < 320; ++y) {
		for (float x = 0; x < 480; ++x) {
			float i = x / 480.0;
			float j = y / 320.0;

			float r = i * (1-j) * 32;
			float b = j * (1-i) * 32;
			float g = i*j * 64;

			tft.drawPixel(x, y, 0
				| (int) r << 11
				| (int) g << 5
				| (int) b << 0
			);
		}
	}
}
