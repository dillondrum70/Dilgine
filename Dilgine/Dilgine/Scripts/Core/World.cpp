#include "World.h"

#include "System.h"

#include "SDL2/SDL.h"

void World::Init(SDL_Window* pWindow)
{
	GameObject* background = DBG_NEW GameObject();
	Transform backTrans = background->GetTransform();
	int top, left, bottom, right;
	SDL_GetWindowBordersSize(pWindow, &top, &left, &bottom, &right);
	backTrans.position = Vector2(top / 2, left / 2);
	gameObjects.push_back(background);
}

void World::CleanUp()
{
	for (GameObject* obj : gameObjects)
	{
		delete obj;
		obj = nullptr;
	}

	gameObjects.clear();
}

void World::Update()
{
	for (GameObject* obj : gameObjects)
	{
		if (!obj)
		{
			gpr460::System::engine.system->ErrorMessage(gpr460::ERROR_MISSING_GAMEOBJECT_REFERENCE);
			gpr460::System::engine.system->LogToErrorFile(gpr460::ERROR_MISSING_GAMEOBJECT_REFERENCE);
			return;
		}

		obj->Update();
	}
}

void World::Render()
{
	for (GameObject* obj : gameObjects)
	{
		if (!obj)
		{
			gpr460::System::engine.system->ErrorMessage(gpr460::ERROR_MISSING_GAMEOBJECT_REFERENCE);
			gpr460::System::engine.system->LogToErrorFile(gpr460::ERROR_MISSING_GAMEOBJECT_REFERENCE);
			continue;
		}

		obj->Render(gpr460::System::engine.renderer);
	}
}