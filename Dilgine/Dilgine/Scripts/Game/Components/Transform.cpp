#include "Transform.h"
#include "GameObject.h"

void Transform::Create(GameObject& gameObject, Vector2 pos) 
{ 
	gameObject.CreateTransform(gameObject, pos); 
}