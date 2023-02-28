#pragma once

#include <iostream>

class float3 {
private:
	float x;
	float y;
	float z;

public:
	// --- Constructors ---
	float3(float _x, float _y, float _z)
		: x {_x}
		, y {_y}
		, z {_z} {};

	float3(float scalar)
		: x {scalar}
		, y {scalar}
		, z {scalar} {};

	// --- Copy constructor ---
	float3(const float3& v) {
		x = v.x;
		y = v.y;
		z = v.z;
	};

	// --- Arithmetic operators ---
	float3 operator+(const float3& v) const;
	float3 operator-(const float3& v) const;
	float3 operator*(const float scalar) const;
	float3 operator/(const float scalar) const;

	// --- Assignment operators ---
	float3& operator+=(const float3& v);
	float3& operator-=(const float3& v);
	float3& operator*=(const float scalar);
	float3& operator/=(const float scalar);

	// Specifies a compile-time constant float3
	static const float3 zero;
	static const float3 one;
	static const float3 unitX;
	static const float3 unitY;
	static const float3 unitZ;

	friend std::ostream& operator<<(std::ostream& out, const float3& v);
};

// --- Arithmetic operators ---
inline float3 float3::operator+(const float3& v) const {
	return float3(x + v.x, y + v.y, z + v.z);
}

inline float3 float3::operator-(const float3& v) const {
	return float3(x - v.x, y - v.y, z - v.z);
}

inline float3 float3::operator*(const float scalar) const {
	return float3(x * scalar, y * scalar, z * scalar);
}

inline float3 float3::operator/(const float scalar) const {
	float invScalar = 1.f / scalar;
	return float3(x * invScalar, y * invScalar, z * invScalar);
}

// --- Assignment operators ---

inline float3& float3::operator+=(const float3& v) {
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

inline float3& float3::operator-=(const float3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

inline float3& float3::operator*=(const float scalar) {
	x = x * scalar;
	y = y * scalar;
	z = z * scalar;

	return *this;
}

inline float3& float3::operator/=(const float scalar) {
	float invScalar = 1.0f / scalar;
	x = x * invScalar;
	y = y * invScalar;
	z = z * invScalar;

	return *this;
}

inline std::ostream& operator<<(std::ostream& out, const float3& v) {
	out << "x: " << v.x << ", y: " << v.z << ", z: " << v.z;

	return out;
}