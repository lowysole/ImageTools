#include "float2.h"

// --- Arithmetic Operators ---

float2 float2::operator+(const float2& v) const {
	return float2(x + v.x, y + v.y);
}

float2 float2::operator-(const float2& v) const {
	return float2(x - v.x, y - v.y);
}

float2 float2::operator*(const float scalar) const {
	return float2(x * scalar, y * scalar);
}

float2 float2::operator/(const float scalar) const {
	float invScalar = 1 / scalar;
	return float2(x * invScalar, y * invScalar);
}

// --- Assign Operators ---

float2& float2::operator=(const float2& v) {
	x = v.x;
	y = v.y;

	return *this;
}

float2& float2::operator+=(const float2& v) {
	x += v.x;
	y += v.y;

	return *this;
}

float2& float2::operator-=(const float2& v) {
	x = x - v.x;
	y = y - v.y;

	return *this;
}

float2& float2::operator*=(const float scalar) {
	x = x * scalar;
	y = y * scalar;

	return *this;
}

float2& float2::operator/=(const float scalar) {
	float invScalar = 1.f / scalar;
	x = x * invScalar;
	y = y * invScalar;

	return *this;
}

// --- Vector Operations ---

float float2::Length() const {
	return sqrt(x * x + y * y);
}

float float2::LengthSq() const {
	return x * x + y * y;
}

float2& float2::Normalized() {
	float length = Length();
	x /= length;
	y /= length;

	return *this;
}

float2 float2::Normalize() const {
	float length = Length();
	return float2(x / length, y / length);
}

float float2::Dot(const float2& v) const {
	return (x * v.x + y * v.y);
}

float float2::AngleBetween(const float2& v) const {
	return acos(Dot(v) / sqrt(LengthSq() * v.LengthSq()));
}

float2 float2::xx() const {
	return float2(x, x);
}

 float2 float2::xy() const {
	return float2(x, y);
}

 float2 float2::yx() const {
	return float2(y, x);
}

 float2 float2::yy() const {
	return float2(y, y);
}

std::ostream& operator<<(std::ostream& out, const float2& f) {
	out << "x: " << f.x << ", y: " << f.y;
	return out;
}

const float2 float2::zero = float2(0, 0);
const float2 float2::one = float2(1, 1);
const float2 float2::unitX = float2(1, 0);
const float2 float2::unitY = float2(0, 1);