#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Vector2.h"

#include <iostream>

class GameObject;

class Transform
{
public:
	enum { compID = 'TRNS' };

	static void Deserialize(GameObject& gameObject, std::istream& stream);

	static void Create(GameObject& gameObject, Vector2 pos);

	Vector2 position;

	Transform() { position = Vector2(0); }
	Transform(Vector2 vPos) { position = vPos; }

	friend std::ostream& operator<<(std::ostream& os, const Transform& t) { os << t.position;  return os; }
};

#endif