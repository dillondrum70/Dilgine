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

	static void Create(GameObject& gameObject, Vector3 pos, Vector3 rot, Vector3 scale);

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	Transform() { position = Vector3(0.0f); rotation = Vector3(0.0f); scale = Vector3(0.0f);}
	Transform(Vector3 vPos, Vector3 vRot, Vector3 vScale) { position = vPos; rotation = vRot; scale = vScale; }

	void Reset();

	friend std::ostream& operator<<(std::ostream& os, const Transform& t) { os << t.position << " " << t.rotation << " " << t.scale;  return os; }
};

#endif