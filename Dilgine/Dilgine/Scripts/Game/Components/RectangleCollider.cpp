#include "RectangleCollider.h"

#include "GameObject.h"
#include "System.h"

bool RectangleCollider::CheckCollision(RectangleCollider* other)
{
	if (!other)
	{
		gpr460::System::engine.system->ErrorMessage(gpr460::ERROR_MISSING_RECTCOLLIDER_REFERENCE);
		gpr460::System::engine.system->LogToErrorFile(gpr460::ERROR_MISSING_RECTCOLLIDER_REFERENCE);
		return false;
	}

	if (!other->GetGameObject() || !gameObject)
	{
		gpr460::System::engine.system->ErrorMessage(gpr460::ERROR_MISSING_GAMEOBJECT_REFERENCE);
		gpr460::System::engine.system->LogToErrorFile(gpr460::ERROR_MISSING_GAMEOBJECT_REFERENCE);
		return false;
	}

	Transform otherTransform = other->GetGameObject()->GetTransform();


}