#include "PlayerController.h"
#include "GameObject.h"

#include "SDL2/SDL.h"
#include "System.h"

void PlayerController::Update()
{
	if (!gameObject)
	{
		gpr460::System::engine.system->ErrorMessage(gpr460::ERROR_MISSING_GAMEOBJECT_REFERENCE);
		gpr460::System::engine.system->LogToErrorFile(gpr460::ERROR_MISSING_GAMEOBJECT_REFERENCE);
		return;
	}

	Transform transform = gameObject->GetTransform();

	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

	transform.position += Vector2(keyboardState[SDLK_RIGHT] - keyboardState[SDLK_LEFT], keyboardState[SDLK_UP] - keyboardState[SDLK_DOWN]) * speed;
}