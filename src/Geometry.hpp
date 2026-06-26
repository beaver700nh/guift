#pragma once

#include <stdint.h>

namespace guift {
namespace geom {

template<typename T>
struct CartesianVec2d {
	T x, y;

	inline bool operator==(const CartesianVec2d<T> &other) const {
		return x == other.x && y == other.y;
	}

	inline bool operator!=(const CartesianVec2d<T> &other) const {
		return !(*this == other);
	}

	inline CartesianVec2d<T> operator+(const CartesianVec2d<T> &other) const {
		return {x + other.x, y + other.y};
	}

	inline CartesianVec2d<T> operator-(const CartesianVec2d<T> &other) const {
		return {x - other.x, y - other.y};
	}

	inline CartesianVec2d<T> operator*(const T &scalar) const {
		return {x * scalar, y * scalar};
	}

	inline CartesianVec2d<T> operator/(const T &scalar) const {
		return *this * (1.0 / scalar);
	}
};

using Point = CartesianVec2d<uint16_t>;
using Size = CartesianVec2d<uint16_t>;

struct CornerRect {
	Point a, b;

	inline bool contains(const Point &p) const {
		return p.x >= a.x && p.x < b.x && p.y >= a.y && p.y < b.y;
	}
};

struct BoxRect {
	Point origin;
	Size size;

	inline operator CornerRect() const {
		return {origin, origin + size};
	}
};

}
}
