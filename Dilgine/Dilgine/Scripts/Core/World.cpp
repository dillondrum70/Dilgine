#include "World.h"

#include "System.h"
//#include <windows.h>

#include "SDL2/SDL.h"

const std::string path = "..\\Dilgine\\Data\\level0.dat";

void World::Init(SDL_Window* pWindow)
{
	int width, height;
	SDL_GetWindowSize(pWindow, &width, &height);

	/*TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	std::wcout << std::wstring(buffer).substr(0, pos);*/
	//std::string path = "..\\Dilgine\\Data\\level0.dat";
	//std::cout << std::endl << path << std::endl;
	serial.LoadLevel(path);

	/*GameObject background;
	//Add Transform component
	Transform::Create(background, Vector2(width / 2, height / 2));
	//Add RectangleRenderer component
	RectangleRenderer::Create(background, width, height, Vector3(150, 150, 150));
	AddGameObject(background);

	GameObject player;
	Transform::Create(player, Vector2(width / 4, height / 2));
	RectangleRenderer::Create(player, 50, 50, Vector3(0, 255, 255));
	RectangleCollider::Create(player, 50, 50);
	PlayerController::Create(player, 1);
	CollisionColorChanger::Create(player, Vector3(0, 0, 255));
	AddGameObject(player);

	int wallCount = 20;
	for (int i = 0; i < wallCount; i++)
	{
		GameObject wall;
		Transform::Create(wall, Vector2(rand() % width, rand() % height));
		RectangleRenderer::Create(wall, 50, 50, Vector3(255, 150, 0));
		RectangleCollider::Create(wall, 50, 50);
		CollisionColorChanger::Create(wall, Vector3(0, 0, 255));
		AddGameObject(wall);
	}*/
}

//void World::CreateTransform(GameObject& rObj, Vector2 vPos)
//{
//	if (activeTransforms + 1 >= gpr460::MAX_GAMEOBJECTS)
//	{
//		gpr460::engine->system->ErrorMessage(gpr460::ERROR_COMPONENT_OVERFLOW);
//		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_COMPONENT_OVERFLOW);
//	}
//
//	components.transformComponents[activeTransforms] = Transform(vPos);
//	rObj.SetTransform(&components.transformComponents[activeTransforms]);
//	activeTransforms++;
//}
//
//void World::CreateRectangleRenderer(GameObject& rObj, int vWidth, int vheight, Vector3 vColor)
//{
//	if (activeRectRenderers + 1 >= gpr460::MAX_GAMEOBJECTS)
//	{
//		gpr460::engine->system->ErrorMessage(gpr460::ERROR_COMPONENT_OVERFLOW);
//		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_COMPONENT_OVERFLOW);
//	}
//
//	components.rectRendererComponents[activeRectRenderers] = RectangleRenderer(vWidth, vheight, vColor, &gameObjects[activeGameObjects]);
//	rObj.SetRenderer(&components.rectRendererComponents[activeRectRenderers]);
//	activeRectRenderers++;
//}
//
//void World::CreateRectangleCollider(GameObject& rObj, int vWidth, int vHeight)
//{
//
//	if (activeRectColliders + 1 >= gpr460::MAX_GAMEOBJECTS)
//	{
//		gpr460::engine->system->ErrorMessage(gpr460::ERROR_COMPONENT_OVERFLOW);
//		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_COMPONENT_OVERFLOW);
//	}
//
//	components.rectColliderComponents[activeRectColliders] = RectangleCollider(vWidth, vHeight, &gameObjects[activeGameObjects]);
//	rObj.SetCollider(&components.rectColliderComponents[activeRectColliders]);
//	activeRectColliders++;
//}
//
//void World::CreatePlayerController(GameObject& rObj, int vSpeed)
//{
//	if (activePlayerControllers + 1 >= gpr460::MAX_GAMEOBJECTS)
//	{
//		gpr460::engine->system->ErrorMessage(gpr460::ERROR_COMPONENT_OVERFLOW);
//		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_COMPONENT_OVERFLOW);
//	}
//
//	components.playerControllerComponents[activePlayerControllers] = (PlayerController(vSpeed, &gameObjects[activeGameObjects]));
//	rObj.SetPlayer(&components.playerControllerComponents[activePlayerControllers]);
//	activePlayerControllers++;
//}
//
//void World::CreateCollisionColorChange(GameObject& rObj, Vector3 vColor)
//{
//	if (activeColorChange + 1 >= gpr460::MAX_GAMEOBJECTS)
//	{
//		gpr460::engine->system->ErrorMessage(gpr460::ERROR_COMPONENT_OVERFLOW);
//		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_COMPONENT_OVERFLOW);
//	}
//
//	components.colorChangeComponents[activeColorChange] = CollisionColorChanger(vColor, &gameObjects[activeGameObjects]);
//	rObj.SetColorChanger(&components.colorChangeComponents[activeColorChange]);
//	activeColorChange++;
//}

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

void World::PrintFrame()
{
	int* frame = gpr460::engine->stack.alloc<int>();
	*frame = gpr460::engine->frame;
	std::cout << "Frame: " << *frame << std::endl;
	std::cout << "Returned Frame Address: " << frame << std::endl;
}

void World::Update()
{
	PlayerController::UpdateAll();

	//Checks each collider against each other collider once
	//for(int i = 0; ...
		//for(int j = i; ...
	CollisionColorChanger::UpdateAll();

	//Call AllCollisions on player and print returned collider addresses
	{
		int count;
		RectangleCollider* colliders = components.rectColliderComponents[0].AllCollisions(count);

		for (int i = 0; i < count; i++)
		{
			std::cout << "Returned Collider: " << &colliders[i] << std::endl;
		}
		std::cout << std::endl;
	}

	//PrintFrame();
	std::cout << std::endl << std::endl;;

	//Clear stack at end of each frame
	gpr460::engine->stack.clear();
}

void World::Render(SDL_Renderer*& prRenderer)
{
	SDL_SetRenderDrawColor(prRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(prRenderer);

	RectangleRenderer::RenderAll(prRenderer);

	SDL_RenderPresent(prRenderer);
}