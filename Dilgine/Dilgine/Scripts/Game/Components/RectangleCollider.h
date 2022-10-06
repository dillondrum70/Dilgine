#ifndef RECTANGLE_COLLIDER_H
#define RECTANGLE_COLLIDER_H

class GameObject;

class RectangleCollider
{
private:
	GameObject* gameObject;

public:
	float width, height;

	RectangleCollider() { width = 1; height = 1; gameObject = nullptr; }
	RectangleCollider(float vWidth, float vHeight, GameObject* vGameObject) { width = vWidth; height = vHeight; gameObject = vGameObject; }

	GameObject* GetGameObject() { return gameObject; }

	bool CheckCollision(RectangleCollider* other);
};

#endif
