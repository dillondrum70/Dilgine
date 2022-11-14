#ifndef COLLISION_COLOR_CHANGER_H
#define COLLISION_COLOR_CHANGER_H

#include "Vector3.h"

#include <vector>
#include <iostream>

class GameObject;

const Vector3 COLOR_CHANGE_DEFAULT = Vector3(255);

class CollisionColorChanger
{
private:
	Vector3 color;
	
	GameObject* gameObject;

public:
	enum { compID = 'CHNG' };

	static void Deserialize(GameObject& gameObject, std::istream& stream);

	static void Create(GameObject& gameObject, Vector3 vColor = COLOR_CHANGE_DEFAULT);

	CollisionColorChanger() { color = Vector3(255); gameObject = nullptr; }
	CollisionColorChanger(Vector3 vColor, GameObject* vGameObject) { color = vColor; gameObject = vGameObject; }

	static void UpdateAll();
};

#endif