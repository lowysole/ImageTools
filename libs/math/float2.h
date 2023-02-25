#pragma once

#include <iostream>
class float2 {
public:
	float x;
	float y;

public:
	// --- Constructors ---
	float2()
		: x {0.0f}
		, y {0.0f} {}

	float2(float _x, float _y)
		: x {_x}
		, y {_y} {}

	float2(float scalar)
		: x {scalar}
		, y {scalar} {};

	// --- Copy constructor ---
	float2(const float2& v) {
		x = v.x;
		y = v.y;
	}

	// --- Arithmetic Operators ---
	float2 operator+(const float2& v) const;
	float2 operator-(const float2& v) const;
	float2 operator*(const float scalar) const;
	float2 operator/(const float scalar) const;

	// --- Assign Operators ---
	float2& operator=(const float2& v);
	float2& operator+=(const float2& v);
	float2& operator-=(const float2& v);
	float2& operator*=(const float scalar);
	float2& operator/=(const float scalar);

	float* ptr() {
		return &x;
	}
	const float* ptr() const {
		return &x;
	}

	// --- Vector Operations ---

	// Return Length of vector;
	float Length() const;

	float LengthSq() const;

	// Normalize the vector;
	float2& Normalized();

	// Return a normalized vector from the original vector
	float2 Normalize() const;

	// Return Dot product from vector and passed vector
	float Dot(const float2& v) const;

	// Return Angle between vectors
	float AngleBetween(const float2& v) const;

	float2 xx() const;
	float2 xy() const;
	float2 yx() const;
	float2 yy() const;

	// Specifies a compile-time constant float2
	static const float2 zero;
	static const float2 one;
	static const float2 unitX;
	static const float2 unitY;

	friend std::ostream& operator<<(std::ostream& out, const float2& f);
};

// --- Arithmetic Operators ---

inline float2 float2::operator+(const float2& v) const {
	return float2(x + v.x, y + v.y);
}

inline float2 float2::operator-(const float2& v) const {
	return float2(x - v.x, y - v.y);
}

inline float2 float2::operator*(const float scalar) const {
	return float2(x * scalar, y * scalar);
}

inline float2 float2::operator/(const float scalar) const {
	float invScalar = 1 / scalar;
	return float2(x * invScalar, y * invScalar);
}

// --- Assign Operators ---

inline float2& float2::operator=(const float2& v) {
	x = v.x;
	y = v.y;

	return *this;
}

inline float2& float2::operator+=(const float2& v) {
	x += v.x;
	y += v.y;

	return *this;
}

inline float2& float2::operator-=(const float2& v) {
	x = x - v.x;
	y = y - v.y;

	return *this;
}

inline float2& float2::operator*=(const float scalar) {
	x = x * scalar;
	y = y * scalar;

	return *this;
}

inline float2& float2::operator/=(const float scalar) {
	float invScalar = 1.f / scalar;
	x = x * invScalar;
	y = y * invScalar;

	return *this;
}

// --- Vector Operations ---

inline float float2::Length() const {
	return sqrt(x * x + y * y);
}

inline float float2::LengthSq() const {
	return x * x + y * y;
}

inline float2& float2::Normalized() {
	float length = Length();
	x /= length;
	y /= length;

	return *this;
}

inline float2 float2::Normalize() const {
	float length = Length();
	return float2(x / length, y / length);
}

inline float float2::Dot(const float2& v) const {
	return (x * v.x + y * v.y);
}

inline float float2::AngleBetween(const float2& v) const {
	return acos(Dot(v) / sqrt(LengthSq() * v.LengthSq()));
}

inline float2 float2::xx() const {
	return float2(x, x);
}

inline float2 float2::xy() const {
	return float2(x, y);
}

inline float2 float2::yx() const {
	return float2(y, x);
}

inline float2 float2::yy() const {
	return float2(y, y);
}

const float2 float2::zero = float2(0, 0); 
const float2 float2::one = float2(1, 1);
const float2 float2::unitX = float2(1, 0);
const float2 float2::unitY = float2(0, 1);

inline std::ostream& operator<<(std::ostream& out, const float2& f) {
	out << "x: " << f.x << ", y: " << f.y;
	return out;
}
