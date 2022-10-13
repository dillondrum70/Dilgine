#include "PlayerController.h"
#include "GameObject.h"

#include "SDL2/SDL.h"
#include "System.h"

void PlayerController::Update()
{
	if (!gameObject)
	{
		gpr460::engine.system->ErrorMessage(gpr460::ERROR_MISSING_GAMEOBJECT_REFERENCE);
		gpr460::engine.system->LogToErrorFile(gpr460::ERROR_MISSING_GAMEOBJECT_REFERENCE);
		return;
	}

	Transform* transform = gameObject->GetTransform();

	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

	Vector2 input = Vector2(keyboardState[SDL_SCANCODE_RIGHT] - keyboardState[SDL_SCANCODE_LEFT], keyboardState[SDL_SCANCODE_DOWN] - keyboardState[SDL_SCANCODE_UP]);

	transform->position += input * speed;
}