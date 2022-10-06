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
	renderer = DBG_NEW RectangleRenderer(vWidth, vHeight, vColor);
	return renderer;
}


RectangleCollider* GameObject::CreateCollider(int vWidth, int vHeight, GameObject* vGameObject)
{
	collider = DBG_NEW RectangleCollider(vWidth, vHeight, vGameObject);
	return collider;
}

PlayerController* GameObject::CreatePlayerController(GameObject* vGameObject)
{
	player = DBG_NEW PlayerController(vGameObject);
	return player;
}

CollisionColorChanger* GameObject::CreateCollisionColorChanger(Vector3 vColor, GameObject* vGameObject)
{
	colorChanger = DBG_NEW CollisionColorChanger(vColor, vGameObject);
	return colorChanger;
}

void GameObject::Update()
{
	if (player)
		player->Update();
	if (colorChanger)
		colorChanger->Update();
}

void GameObject::Render(SDL_Renderer* pRenderer)
{
	if (renderer && pRenderer)
	{
		SDL_Rect rect = {
		transform.position.x,
		transform.position.y,
		renderer->width,
		renderer->height
		};

		
		SDL_SetRenderDrawColor(pRenderer, renderer->color.x, renderer->color.y, renderer->color.z, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(pRenderer, &rect);
	}
}