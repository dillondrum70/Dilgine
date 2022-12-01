#ifndef VECTOR3_H
#define VECTOR3_H
#include <math.h>
#include <iostream>

struct Vector3
{
	float x, y, z;

	Vector3() { x = 0; y = 0; z = 0; }
	Vector3(float val) { x = val; y = val; z = val; }
	Vector3(float valX, float valY, float valZ) { x = valX; y = valY; z = valZ; }

	inline float Magnitude() { return sqrt((x * x) + (y * y) + (z * z)); }
	Vector3 Normalized() {
		float mag = Magnitude();
		if (mag == 0)
			return Vector3(0);
		return Vector3(x / mag, y / mag, z / mag);
	}

	static inline Vector3 Cross(Vector3 a, Vector3 b) { return Vector3((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x)); }

	const static inline Vector3 Forward() { return Vector3(0, 1, 0); }
	const static inline Vector3 Right() { return Vector3(1, 0, 0); }
	const static inline Vector3 Up() { return Vector3(0, 0, 1); }

	Vector3 operator+(const Vector3& rhs) { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vector3 operator-(const Vector3& rhs) { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
	Vector3 operator*(const float rhs) { return Vector3(x * rhs, y * rhs, z * rhs); }
	void operator+=(const Vector3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; }
	void operator-=(const Vector3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; }
	void operator*=(const float rhs) { x *= rhs; y *= rhs; z *= rhs; }

	bool operator==(const Vector3& rhs) const { return (x == rhs.x && y == rhs.y && z == rhs.z); }
	bool operator!=(const Vector3& rhs) const { return (x != rhs.x || y != rhs.y || z != rhs.z); }

	friend std::ostream& operator<<(std::ostream& os, const Vector3& v) { os << "(" << v.x << ", " << v.y << ", " << v.z << ")"; return os; }
};

#endif
