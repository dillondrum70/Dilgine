#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Vector2.h"

class Transform
{
public:
	Vector2 position;

	Transform() { position = Vector2(0); }
	Transform(Vector2 vPos) { position = vPos; }
};

#endif