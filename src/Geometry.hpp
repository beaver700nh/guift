#pragma once

#include <WString.h>

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

	inline double distanceTo(const CartesianVec2d<T> &other) const {
		return distance(*this, other);
	}

	static inline double distance(const CartesianVec2d<T> &p, const CartesianVec2d<T> &q) {
		auto a = (double) p.x - q.x;
		auto b = (double) p.y - q.y;
		return sqrt(a * a + b * b);
	}

	// Assumes calculation will not overflow
	inline CartesianVec2d<T> operator+(const CartesianVec2d<T> &other) const {
		return {static_cast<T>(x + other.x), static_cast<T>(y + other.y)};
	}

	// Assumes calculation will not overflow
	inline CartesianVec2d<T> operator-(const CartesianVec2d<T> &other) const {
		return {static_cast<T>(x - other.x), static_cast<T>(y - other.y)};
	}

	inline CartesianVec2d<T> operator*(const T &scalar) const {
		return {x * scalar, y * scalar};
	}

	inline CartesianVec2d<T> operator/(const T &scalar) const {
		return *this * (1.0 / scalar);
	}

	inline CartesianVec2d<double> scaledBy(double scalar) const {
		return {x * scalar, y * scalar};
	}

	inline operator String() const {
		return String("(") + x + ", " + y + ")";
	}
};

using Point = CartesianVec2d<uint16_t>;
using Size = CartesianVec2d<uint16_t>;

struct CornerRect {
	Point a, b;

	inline bool contains(const Point &p) const {
		return p.x >= a.x && p.x < b.x && p.y >= a.y && p.y < b.y;
	}

	inline operator String() const {
		return String("[") + a + ", " + b + "]";
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
