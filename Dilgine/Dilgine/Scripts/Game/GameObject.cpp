#include "GameObject.h"

#include "System.h"

#include "SDL2/SDL.h"

GameObject::~GameObject()
{
	if (renderer)
	{
		delete renderer;
		renderer = nullptr;
	}
	if (collider)
	{
		delete collider;
		collider = nullptr;
	}
	if (player)
	{
		delete player;
		player = nullptr;
	}
	if (colorChanger)
	{
		delete colorChanger;
		colorChanger = nullptr;
	}
}

RectangleRenderer* GameObject::CreateRenderer(int vWidth, int vHeight, Vector3 vColor)
{
	if (!renderer)
	{
		renderer = DBG_NEW RectangleRenderer(vWidth, vHeight, vColor);
		return renderer;
	}
	else
	{
		gpr460::engine.system->ErrorMessage(gpr460::ERROR_COMPONENT_EXISTS);
		gpr460::engine.system->LogToErrorFile(gpr460::ERROR_COMPONENT_EXISTS);
	}
}


RectangleCollider* GameObject::CreateCollider(int vWidth, int vHeight, GameObject* vGameObject)
{
	if (!collider)
	{
		collider = DBG_NEW RectangleCollider(vWidth, vHeight, vGameObject);
		return collider;
	}
	else
	{
		gpr460::engine.system->ErrorMessage(gpr460::ERROR_COMPONENT_EXISTS);
		gpr460::engine.system->LogToErrorFile(gpr460::ERROR_COMPONENT_EXISTS);
	}
}

PlayerController* GameObject::CreatePlayerController(GameObject* vGameObject)
{
	if (!player)
	{
		player = DBG_NEW PlayerController(vGameObject);
		return player;
	}
	else
	{
		gpr460::engine.system->ErrorMessage(gpr460::ERROR_COMPONENT_EXISTS);
		gpr460::engine.system->LogToErrorFile(gpr460::ERROR_COMPONENT_EXISTS);
	}
}

CollisionColorChanger* GameObject::CreateCollisionColorChanger(Vector3 vColor, GameObject* vGameObject)
{
	if (!colorChanger)
	{
		colorChanger = DBG_NEW CollisionColorChanger(vColor, vGameObject);
		return colorChanger;
	}
	else
	{
		gpr460::engine.system->ErrorMessage(gpr460::ERROR_COMPONENT_EXISTS);
		gpr460::engine.system->LogToErrorFile(gpr460::ERROR_COMPONENT_EXISTS);
	}
}

void GameObject::Update()
{
	if (player)
		player->Update();
	if (colorChanger)
		colorChanger->Update();
}

void GameObject::Render(SDL_Renderer*& prRenderer)
{

	if (renderer && prRenderer)
	{
		SDL_Rect rect = {
		transform.position.x - (renderer->width / 2),
		transform.position.y - (renderer->height / 2),
		renderer->width,
		renderer->height
		};
		
		SDL_SetRenderDrawColor(prRenderer, renderer->color.x, renderer->color.y, renderer->color.z, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(prRenderer, &rect);
	}
}