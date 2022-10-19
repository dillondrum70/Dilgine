#include "World.h"

#include "System.h"

#include "SDL2/SDL.h"

void World::Init(SDL_Window* pWindow)
{
	int width, height;
	SDL_GetWindowSize(pWindow, &width, &height);

	GameObject background;
	//Add Transform component
	CreateTransform(background, Vector2(width / 2, height / 2));
	//Add RectangleRenderer component
	CreateRectangleRenderer(background, width, height, Vector3(150, 150, 150));
	AddGameObject(background);

	GameObject player;
	CreateTransform(player, Vector2(width / 4, height / 2));
	CreateRectangleRenderer(player, 50, 50, Vector3(0, 255, 255));
	CreateRectangleCollider(player, 50, 50);
	CreatePlayerController(player, 1);
	CreateCollisionColorChange(player, Vector3(0, 0, 255));
	AddGameObject(player);

	GameObject wall;
	CreateTransform(wall, Vector2((3 * width) / 4, height / 2));
	CreateRectangleRenderer(wall, 50, 50, Vector3(255, 150, 0));
	CreateRectangleCollider(wall, 50, 50);
	CreateCollisionColorChange(wall, Vector3(0, 0, 255));
	AddGameObject(wall);
}

void World::CreateTransform(GameObject& rObj, Vector2 vPos)
{
	if (activeTransforms + 1 >= gpr460::MAX_GAMEOBJECTS)
	{
		gpr460::engine->system->ErrorMessage(gpr460::ERROR_COMPONENT_OVERFLOW);
		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_COMPONENT_OVERFLOW);
	}

	components.transformComponents[activeTransforms] = Transform(vPos);
	rObj.SetTransform(&components.transformComponents[activeTransforms]);
	activeTransforms++;
}

void World::CreateRectangleRenderer(GameObject& rObj, int vWidth, int vheight, Vector3 vColor)
{
	if (activeRectRenderers + 1 >= gpr460::MAX_GAMEOBJECTS)
	{
		gpr460::engine->system->ErrorMessage(gpr460::ERROR_COMPONENT_OVERFLOW);
		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_COMPONENT_OVERFLOW);
	}

	components.rectRendererComponents[activeRectRenderers] = RectangleRenderer(vWidth, vheight, vColor, &gameObjects[activeGameObjects]);
	rObj.SetRenderer(&components.rectRendererComponents[activeRectRenderers]);
	activeRectRenderers++;
}

void World::CreateRectangleCollider(GameObject& rObj, int vWidth, int vHeight)
{

	if (activeRectColliders + 1 >= gpr460::MAX_GAMEOBJECTS)
	{
		gpr460::engine->system->ErrorMessage(gpr460::ERROR_COMPONENT_OVERFLOW);
		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_COMPONENT_OVERFLOW);
	}

	components.rectColliderComponents[activeRectColliders] = RectangleCollider(vWidth, vHeight, &gameObjects[activeGameObjects]);
	rObj.SetCollider(&components.rectColliderComponents[activeRectColliders]);
	activeRectColliders++;
}

void World::CreatePlayerController(GameObject& rObj, int vSpeed)
{
	if (activePlayerControllers + 1 >= gpr460::MAX_GAMEOBJECTS)
	{
		gpr460::engine->system->ErrorMessage(gpr460::ERROR_COMPONENT_OVERFLOW);
		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_COMPONENT_OVERFLOW);
	}

	components.playerControllerComponents[activePlayerControllers] = (PlayerController(vSpeed, &gameObjects[activeGameObjects]));
	rObj.SetPlayer(&components.playerControllerComponents[activePlayerControllers]);
	activePlayerControllers++;
}

void World::CreateCollisionColorChange(GameObject& rObj, Vector3 vColor)
{
	if (activeColorChange + 1 >= gpr460::MAX_GAMEOBJECTS)
	{
		gpr460::engine->system->ErrorMessage(gpr460::ERROR_COMPONENT_OVERFLOW);
		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_COMPONENT_OVERFLOW);
	}

	components.colorChangeComponents[activeColorChange] = CollisionColorChanger(vColor, &gameObjects[activeGameObjects]);
	rObj.SetColorChanger(&components.colorChangeComponents[activeColorChange]);
	activeColorChange++;
}

void World::AddGameObject(GameObject& rObj)
{
	if (activeGameObjects + 1 >= gpr460::MAX_GAMEOBJECTS)
	{
		gpr460::engine->system->ErrorMessage(gpr460::ERROR_GAMEOBJECT_OVERFLOW);
		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_GAMEOBJECT_OVERFLOW);
	}

	//Add GameObject to cache
	gameObjects[activeGameObjects] = rObj;
	//INC GameObject count
	activeGameObjects++;
}

void World::CleanUp()
{
	
}

void World::Update()
{
	PlayerController::UpdateAll();

	//Checks each collider against each other collider once
	//for(int i = 0; ...
		//for(int j = i; ...
	CollisionColorChanger::UpdateAll();
}

void World::Render(SDL_Renderer*& prRenderer)
{
	SDL_SetRenderDrawColor(prRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(prRenderer);

	RectangleRenderer::RenderAll(prRenderer);

	SDL_RenderPresent(prRenderer);
}