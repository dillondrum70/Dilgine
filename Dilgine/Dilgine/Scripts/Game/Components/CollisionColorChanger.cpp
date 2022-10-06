#include "CollisionColorChanger.h"
#include "GameObject.h"
#include "System.h"

#include "SDL2/SDL.h"

void CollisionColorChanger::Update()
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

	//if(col->CheckCollision())
}