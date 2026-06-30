#pragma once

#include <WString.h>

namespace guift {
namespace geom {

template<typename T>
struct CartesianVec2d {
	T x, y;

	template<typename U, typename V>
	static inline CartesianVec2d<T> from(U x, V y) {
		return {
			static_cast<T>(x),
			static_cast<T>(y),
		};
	}

	inline bool operator ==(const CartesianVec2d<T> &other) const {
		return x == other.x && y == other.y;
	}

	inline bool operator !=(const CartesianVec2d<T> &other) const {
		return !(*this == other);
	}

	inline bool isPositive() const {
		return x >= 0 && y >= 0;
	}

	inline double length() const {
		return sqrt(0.0 + x * x + y * y);
	}

	inline double distanceTo(const CartesianVec2d<T> &other) const {
		return distance(*this, other);
	}

	static inline double distance(const CartesianVec2d<T> &p, const CartesianVec2d<T> &q) {
		return (p - q).length();
	}

	// Assumes calculation will not overflow
	template<typename U>
	inline CartesianVec2d<T> operator +(const CartesianVec2d<U> &other) const {
		return {static_cast<T>(x + other.x), static_cast<T>(y + other.y)};
	}

	// Assumes calculation will not overflow
	template<typename U>
	inline CartesianVec2d<T> operator -(const CartesianVec2d<U> &other) const {
		return {static_cast<T>(x - other.x), static_cast<T>(y - other.y)};
	}

	inline CartesianVec2d<T> operator *(double scalar) const {
		return {x * scalar, y * scalar};
	}

	inline CartesianVec2d<T> operator /(double scalar) const {
		return *this * (1.0 / scalar);
	}

	inline CartesianVec2d<double> scaledBy(double scalar) const {
		return {x * scalar, y * scalar};
	}

	inline operator CartesianVec2d<double>() const {
		return {x, y};
	}

	inline operator String() const {
		return String("(") + x + ", " + y + ")";
	}
};

using Point = CartesianVec2d<int16_t>;
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

	// Assumes the rectangle is sufficiently small to not overflow
	inline operator CornerRect() const {
		return {origin, origin + size};
	}
};

}}
