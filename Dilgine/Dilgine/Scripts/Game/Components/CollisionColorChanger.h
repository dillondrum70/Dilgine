#ifndef COLLISION_COLOR_CHANGER_H
#define COLLISION_COLOR_CHANGER_H

#include "Vector3.h"

#include <vector>

class GameObject;

class CollisionColorChanger
{
private:
	Vector3 color;
	
	GameObject* gameObject;

public:
	CollisionColorChanger() { color = Vector3(255); gameObject = nullptr; }
	CollisionColorChanger(Vector3 vColor, GameObject* vGameObject) { color = vColor; gameObject = vGameObject; }

	void Update(std::vector<GameObject*> gameObjects);
};

#endif