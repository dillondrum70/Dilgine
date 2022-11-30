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
			Vector3 posInput = Vector3(
				keyboardState[SDL_SCANCODE_LEFT] - keyboardState[SDL_SCANCODE_RIGHT], 
				keyboardState[SDL_SCANCODE_DOWN] - keyboardState[SDL_SCANCODE_UP], 
				keyboardState[SDL_SCANCODE_PAGEUP] - keyboardState[SDL_SCANCODE_PAGEDOWN]
			);
			
			transform->position += Vector3(posInput.x, posInput.y, posInput.z) * components.playerControllerComponents[i].GetSpeed();

			//WASDQE to rotate

			Vector3 rotInput = Vector3(
				keyboardState[SDL_SCANCODE_A] - keyboardState[SDL_SCANCODE_D], 
				keyboardState[SDL_SCANCODE_S] - keyboardState[SDL_SCANCODE_W], 
				keyboardState[SDL_SCANCODE_Q] - keyboardState[SDL_SCANCODE_E]
			);

			transform->rotation += Vector3(rotInput.x, rotInput.y, rotInput.z) * components.playerControllerComponents[i].GetSpeed();

			//UHJKYI to scale (Same layout as WASDQE, but shifted over)

			Vector3 scaleInput = Vector3(
				keyboardState[SDL_SCANCODE_H] - keyboardState[SDL_SCANCODE_K], 
				keyboardState[SDL_SCANCODE_J] - keyboardState[SDL_SCANCODE_U], 
				keyboardState[SDL_SCANCODE_Y] - keyboardState[SDL_SCANCODE_I]
			);

			transform->scale += Vector3(scaleInput.x, scaleInput.y, scaleInput.z) * components.playerControllerComponents[i].GetSpeed();

			//Press space to reset transform component (return to origin, set rotation to identity, scale to 1, 1, 1)
			if (keyboardState[SDL_SCANCODE_SPACE])
			{
				transform->Reset();
			}

			//std::cout << *transform << std::endl;
		}
		else
		{
			gpr460::engine->system->ErrorMessage(gpr460::ERROR_MISSING_TRANSFORM_REFERENCE);
			gpr460::engine->system->LogToErrorFile(gpr460::ERROR_MISSING_TRANSFORM_REFERENCE);
		}
	}
}