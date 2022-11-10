#include "RectangleCollider.h"

#include "GameObject.h"
#include "System.h"

#include <list>

void RectangleCollider::Create(GameObject& gameObject, int vWidth, int vHeight)
{
	gameObject.CreateRectangleCollider(gameObject, vWidth, vHeight);
}

bool RectangleCollider::CheckCollision(RectangleCollider& other)
{
	if (!other.GetGameObject() || !gameObject)
	{
		gpr460::engine->system->ErrorMessage(gpr460::ERROR_MISSING_GAMEOBJECT_REFERENCE);
		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_MISSING_GAMEOBJECT_REFERENCE);
		return false;
	}

	Transform* otherTransform = other.GetGameObject()->GetTransform();

	if (abs(otherTransform->position.x - gameObject->GetTransform()->position.x) < (other.width + width) / 2 &&
		abs(otherTransform->position.y - gameObject->GetTransform()->position.y) < (other.height + height) / 2)
	{
		return true;
	}

	return false;
}

RectangleCollider* RectangleCollider::AllCollisions(int& count)
{
	if (!gameObject)
	{
		gpr460::engine->system->ErrorMessage(gpr460::ERROR_MISSING_GAMEOBJECT_REFERENCE);
		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_MISSING_GAMEOBJECT_REFERENCE);
		return nullptr;
	}

	RectangleCollider* rectCollider = gpr460::engine->world->GetComponents().rectColliderComponents;
	Components& components = gpr460::engine->world->GetComponents();

	std::vector<int> included;

	for (int i = 0; i < gpr460::engine->world->activeRectColliders; i++)
	{
		GameObject* obj = rectCollider[i].GetGameObject();
		if (&rectCollider[i] != this && CheckCollision(rectCollider[i]))
		{
			included.push_back(i);
		}
	}

	count = included.size();
	RectangleCollider* colliders = gpr460::engine->stack.alloc<RectangleCollider>(included.size());
	for (int i = 0; i < count; i++)
	{
		colliders[i] = rectCollider[included[i]];
	}

	return colliders;
}