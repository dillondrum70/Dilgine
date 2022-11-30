#include "PlayerController.h"
#include "GameObject.h"
#include "Vector2.h"

#include "SDL2/SDL.h"
#include "System.h"

//PCTL [s]
void PlayerController::Deserialize(GameObject& gameObject, std::istream& stream)
{
	stream.ignore(100, '[');

	float s = 0;

	stream >> s;

	gameObject.CreatePlayerController(gameObject, s);

	stream.ignore(100, ']');
}

void PlayerController::Create(GameObject& gameObject, float vSpeed)
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

			//Arrow Keys to move
			Vector2 input = Vector2(keyboardState[SDL_SCANCODE_LEFT] - keyboardState[SDL_SCANCODE_RIGHT], keyboardState[SDL_SCANCODE_DOWN] - keyboardState[SDL_SCANCODE_UP]);
			
			transform->position += Vector3(input.x, input.y, 0) * components.playerControllerComponents[i].GetSpeed();

			//Press space to return to origin
			if (keyboardState[SDL_SCANCODE_SPACE])
			{
				transform->position = Vector3(0.0f);
			}

			std::cout << transform->position << std::endl;
		}
		else
		{
			gpr460::engine->system->ErrorMessage(gpr460::ERROR_MISSING_TRANSFORM_REFERENCE);
			gpr460::engine->system->LogToErrorFile(gpr460::ERROR_MISSING_TRANSFORM_REFERENCE);
		}
	}
}