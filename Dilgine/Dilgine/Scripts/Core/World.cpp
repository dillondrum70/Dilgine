#include "World.h"

#include "System.h"

#include "SDL2/SDL.h"

void World::Init(SDL_Window* pWindow)
{
	int width, height;
	SDL_GetWindowSize(pWindow, &width, &height);

	GameObject* background = DBG_NEW GameObject();
	Transform& backTrans = background->GetTransform();
	backTrans.position = Vector2(width / 2, height / 2);
	background->CreateRenderer(width, height, Vector3(150, 150, 150));
	gameObjects.push_back(background);

	GameObject* player = DBG_NEW GameObject();
	Transform& playerTrans = player->GetTransform();
	playerTrans.position = Vector2(width / 4, height / 2);
	player->CreateRenderer(50, 50, Vector3(0, 255, 255));
	player->CreateCollider(50, 50, player);
	player->CreatePlayerController(player);
	player->CreateCollisionColorChanger(Vector3(0, 0, 255), player);
	gameObjects.push_back(player);

	GameObject* wall = DBG_NEW GameObject();
	Transform& wallTrans = wall->GetTransform();
	wallTrans.position = Vector2( (3 * width) / 4, height / 2);
	wall->CreateRenderer(50, 50, Vector3(255, 150, 0));
	wall->CreateCollider(50, 50, wall);
	wall->CreateCollisionColorChanger(Vector3(0, 0, 255), wall);
	gameObjects.push_back(wall);
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
			gpr460::engine.system->ErrorMessage(gpr460::ERROR_MISSING_GAMEOBJECT_REFERENCE);
			gpr460::engine.system->LogToErrorFile(gpr460::ERROR_MISSING_GAMEOBJECT_REFERENCE);
			return;
		}

		obj->Update();
	}
}

void World::Render(SDL_Renderer*& prRenderer)
{
	for (GameObject* obj : gameObjects)
	{
		if (!obj)
		{
			gpr460::engine.system->ErrorMessage(gpr460::ERROR_MISSING_GAMEOBJECT_REFERENCE);
			gpr460::engine.system->LogToErrorFile(gpr460::ERROR_MISSING_GAMEOBJECT_REFERENCE);
			continue;
		}

		if (prRenderer)
		{
			obj->Render(prRenderer);
		}
		else
		{
			gpr460::engine.system->ErrorMessage(gpr460::ERROR_MISSING_ENGINE_RENDERER);
			gpr460::engine.system->LogToErrorFile(gpr460::ERROR_MISSING_ENGINE_RENDERER);
		}
	}
}