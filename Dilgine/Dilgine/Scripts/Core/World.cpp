#include "World.h"

#include "System.h"

#include "SDL2/SDL.h"

void World::Init(SDL_Window* pWindow)
{
	int width, height;
	SDL_GetWindowSize(pWindow, &width, &height);

	GameObject background;
	//Add Transform component
	//components.transformComponents.push_back(Transform(Vector2(width / 2, height / 2)));
	components.transformComponents[activeTransforms] = (Transform(Vector2(width / 2, height / 2)));
	background.SetTransform(&components.transformComponents[activeTransforms]);
	activeTransforms++;
	//Add RectangleRenderer component
	components.rectRendererComponents[activeRectRenderers] = (RectangleRenderer(width, height, Vector3(150, 150, 150)));
	background.SetRenderer(&components.rectRendererComponents[activeRectRenderers]);
	activeRectRenderers++;
	//Add GameObject to cache
	gameObjects[activeGameObjects] = background;
	//INC GameObject count
	activeGameObjects++;

	GameObject player;
	components.transformComponents[activeTransforms] = (Vector2(width / 4, height / 2));
	player.SetTransform(&components.transformComponents[activeTransforms]);
	activeTransforms++;
	components.rectRendererComponents[activeRectRenderers] = (RectangleRenderer(50, 50, Vector3(0, 255, 255)));
	player.SetRenderer(&components.rectRendererComponents[activeRectRenderers]);
	activeRectRenderers++;
	components.rectColliderComponents[activeRectColliders] = (RectangleCollider(50, 50, &player));
	player.SetCollider(&components.rectColliderComponents[activeRectColliders]);
	activeRectColliders++;
	components.playerControllerComponents[activePlayerControllers] = (PlayerController(&player));
	player.SetPlayer(&components.playerControllerComponents[activePlayerControllers]);
	activePlayerControllers++;
	components.colorChangeComponents[activeColorChange] = (CollisionColorChanger(Vector3(0, 0, 255), &player));
	player.SetColorChanger(&components.colorChangeComponents[activeColorChange]);
	activeColorChange++;
	gameObjects[activeGameObjects] = player;
	activeGameObjects++;

	GameObject wall;
	components.transformComponents[activeTransforms] = (Vector2((3 * width) / 4, height / 2));
	wall.SetTransform(&components.transformComponents[activeTransforms]);
	activeTransforms++;
	components.rectRendererComponents[activeRectRenderers] = (RectangleRenderer(50, 50, Vector3(255, 150, 0)));
	wall.SetRenderer(&components.rectRendererComponents[activeRectRenderers]);
	activeRectRenderers++;
	components.rectColliderComponents[activeRectColliders] = (RectangleCollider(50, 50, &wall));
	wall.SetCollider(&components.rectColliderComponents[activeRectColliders]);
	activeRectColliders++;
	components.colorChangeComponents[activeColorChange] = (CollisionColorChanger(Vector3(0, 0, 255), &wall));
	wall.SetColorChanger(&components.colorChangeComponents[activeColorChange]);
	activeColorChange++;
	gameObjects[activeGameObjects] = wall;
	activeGameObjects++;
}

void World::CleanUp()
{
	/*for (GameObject obj : gameObjects)
	{
		if (obj.GetCollider())
		{
			delete obj.GetCollider();
		}
		if (obj.GetColorChanger())
		{
			delete obj.GetColorChanger();
		}
		if (obj.GetRenderer())
		{
			delete obj.GetRenderer();
		}
		if (obj.GetPlayer())
		{
			delete obj.GetPlayer();
		}
		if (obj.GetTransform())
		{
			delete obj.GetTransform();
		}
	}*/
}

void World::Update()
{
	PlayerController::Update();

	//Checks each collider against each other collider once
	//for(int i = 0; ...
		//for(int j = i; ...
	CollisionColorChanger::Update();
}

void World::Render(SDL_Renderer*& prRenderer)
{
	/*for (GameObject* obj : gameObjects)
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
	}*/

	SDL_SetRenderDrawColor(prRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(prRenderer);

	RectangleRenderer::Render(components.rectRendererComponents, components.transformComponents, activeRectRenderers, activeTransforms, prRenderer);

	SDL_RenderPresent(prRenderer);
}