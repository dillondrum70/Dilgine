#ifndef RECTANGLE_COLLIDER_H
#define RECTANGLE_COLLIDER_H

#include <iostream>

class GameObject;

const int RCOL_DEFAULT_WIDTH = 50, RCOL_DEFAULT_HEIGHT = 50;

class RectangleCollider
{
private:
	GameObject* gameObject;

public:
	int width, height;

	static enum { compID = 'RCOL' };

	static void Deserialize(GameObject& gameObject, std::istream& stream);

	static void Create(GameObject& gameObject, int vWidth = RCOL_DEFAULT_WIDTH, int vHeight = RCOL_DEFAULT_HEIGHT);

	RectangleCollider() { width = 50; height = 50; gameObject = nullptr; }
	RectangleCollider(int vWidth, int vHeight, GameObject* vGameObject) { width = vWidth; height = vHeight; gameObject = vGameObject; }

	GameObject* GetGameObject() { return gameObject; }

	bool CheckCollision(RectangleCollider& other);

	RectangleCollider* AllCollisions(int& count);
};

#endif
