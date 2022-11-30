#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Vector3.h"

#include <iostream>

class GameObject;

class Transform
{
public:
	enum { compID = 'TRNS' };

	static void Deserialize(GameObject& gameObject, std::istream& stream);

	static void Create(GameObject& gameObject, Vector3 pos);

	Vector3 position;

	Transform() { position = Vector3(0.0f); }
	Transform(Vector3 vPos) { position = vPos; }

	friend std::ostream& operator<<(std::ostream& os, const Transform& t) { os << t.position;  return os; }
};

#endif