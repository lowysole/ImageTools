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