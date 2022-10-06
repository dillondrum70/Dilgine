#include "CollisionColorChanger.h"
#include "GameObject.h"
#include "System.h"

#include "SDL2/SDL.h"

#include <iostream>

void CollisionColorChanger::Update(std::vector<GameObject*> gameObjects)
{
	if (!gameObject)
	{
		gpr460::engine.system->ErrorMessage(gpr460::ERROR_MISSING_RECTCOLLIDER_REFERENCE);
		gpr460::engine.system->LogToErrorFile(gpr460::ERROR_MISSING_RECTCOLLIDER_REFERENCE);
		return;
	}

	RectangleCollider* col = gameObject->GetCollider();

	if (!col)
	{
		gpr460::engine.system->ErrorMessage(gpr460::ERROR_MISSING_RECTCOLLIDER_REFERENCE);
		gpr460::engine.system->LogToErrorFile(gpr460::ERROR_MISSING_RECTCOLLIDER_REFERENCE);
		return;
	}

	RectangleRenderer* rend = gameObject->GetRenderer();

	if (!rend)
	{
		gpr460::engine.system->ErrorMessage(gpr460::ERROR_MISSING_RECTRENDERER_REFERENCE);
		gpr460::engine.system->LogToErrorFile(gpr460::ERROR_MISSING_RECTRENDERER_REFERENCE);
		return;
	}

	for (GameObject* obj : gameObjects)
	{
		if (obj && obj != gameObject && obj->GetCollider() && col->CheckCollision(obj->GetCollider()))
		{
			rend->color = color;
			break;
		}
		else
		{
			rend->color = rend->baseColor;
		}
	}
}