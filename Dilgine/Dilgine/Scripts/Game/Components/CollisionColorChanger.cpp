#include "CollisionColorChanger.h"
#include "GameObject.h"
#include "System.h"

#include "SDL2/SDL.h"

#include <iostream>

void CollisionColorChanger::UpdateAll()
{
	RectangleCollider* rectCollider = gpr460::engine->world->GetComponents().rectColliderComponents;
	Components& components = gpr460::engine->world->GetComponents();

	std::vector<bool> changed(gpr460::engine->world->activeRectColliders);	//Stores true if collider color has already been changed

	for (int i = 0; i < gpr460::engine->world->activeRectColliders; i++)
	{
		GameObject* object1 = rectCollider[i].GetGameObject();
		if (object1 && !changed[i])
		{
			for (int j = i + 1; j < gpr460::engine->world->activeRectColliders; j++)
			{
				GameObject* object2 = rectCollider[j].GetGameObject();
				if (object2 && !changed[j] && object1->GetCollider() && object2->GetCollider() &&
					object1->GetRenderer() && object2->GetRenderer() &&
					object1->GetColorChanger() && object2->GetColorChanger())
				{
					if (object1->GetCollider()->CheckCollision(*object2->GetCollider()))
					{
						object1->GetRenderer()->color = object1->GetColorChanger()->color;
						object2->GetRenderer()->color = object2->GetColorChanger()->color;
						changed[i] = true;
						changed[j] = true;
					}
					else
					{
						if (!changed[i])
							object1->GetRenderer()->color = object1->GetRenderer()->baseColor;
						if (!changed[j])
							object2->GetRenderer()->color = object2->GetRenderer()->baseColor;
					}
				}
			}
		}
	}
}