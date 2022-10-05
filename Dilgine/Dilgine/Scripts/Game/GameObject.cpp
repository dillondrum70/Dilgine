#include "GameObject.h"

#include "System.h"

RectangleRenderer* GameObject::CreateRenderer(float vWidth, float vHeight, Vector3 vColor)
{
	return DBG_NEW RectangleRenderer(vWidth, vHeight, vColor);
}


RectangleCollider* GameObject::CreateCollider(float vWidth, float vHeight, GameObject* vGameObject)
{
	return DBG_NEW RectangleCollider(vWidth, vHeight, vGameObject);
}

PlayerController* GameObject::CreatePlayerController(GameObject* vGameObject)
{
	return DBG_NEW PlayerController(vGameObject);
}

CollisionColorChanger* GameObject::CreateCollisionColorChanger(Vector3 vColor, GameObject* vGameObject)
{
	return DBG_NEW CollisionColorChanger(vColor, vGameObject);
}