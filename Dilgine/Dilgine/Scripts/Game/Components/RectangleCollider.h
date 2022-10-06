#ifndef RECTANGLE_COLLIDER_H
#define RECTANGLE_COLLIDER_H

class GameObject;

class RectangleCollider
{
private:
	GameObject* gameObject;

public:
	int width, height;

	RectangleCollider() { width = 50; height = 50; gameObject = nullptr; }
	RectangleCollider(int vWidth, int vHeight, GameObject* vGameObject) { width = vWidth; height = vHeight; gameObject = vGameObject; }

	GameObject* GetGameObject() { return gameObject; }

	bool CheckCollision(RectangleCollider* other);

	bool ResolveCollision(RectangleCollider* other);
};

#endif
