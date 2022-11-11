#include "PlayerController.h"
#include "GameObject.h"

#include "SDL2/SDL.h"
#include "System.h"

//PCTL [s]
void PlayerController::Deserialize(GameObject& gameObject, std::istream& stream)
{
	stream.ignore(100, '[');

	int s = 0;

	stream >> s;

	gameObject.CreatePlayerController(gameObject, s);

	stream.ignore(100, ']');
}

void PlayerController::Create(GameObject& gameObject, int vSpeed)
{
	gameObject.CreatePlayerController(gameObject, vSpeed);
}

void PlayerController::UpdateAll()
{
	Components& components = gpr460::engine->world->GetComponents();

	for (int i = 0; i < gpr460::engine->world->activePlayerControllers; i++)
	{
		Transform* transform = components.playerControllerComponents[i].gameObject->GetTransform();

		if (transform)
		{
			const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

			Vector2 input = Vector2(keyboardState[SDL_SCANCODE_RIGHT] - keyboardState[SDL_SCANCODE_LEFT], keyboardState[SDL_SCANCODE_DOWN] - keyboardState[SDL_SCANCODE_UP]);

			transform->position += input * components.playerControllerComponents[i].speed;
		}
		else
		{
			gpr460::engine->system->ErrorMessage(gpr460::ERROR_MISSING_TRANSFORM_REFERENCE);
			gpr460::engine->system->LogToErrorFile(gpr460::ERROR_MISSING_TRANSFORM_REFERENCE);
		}
	}
}