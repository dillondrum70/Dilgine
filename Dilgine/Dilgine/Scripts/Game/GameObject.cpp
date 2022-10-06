#include "GameObject.h"

#include "System.h"

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
	return DBG_NEW RectangleRenderer(vWidth, vHeight, vColor);
}


RectangleCollider* GameObject::CreateCollider(int vWidth, int vHeight, GameObject* vGameObject)
{
	return DBG_NEW RectangleCollider(vWidth, vHeight, vGameObject);
}

PlayerController* GameObject::CreatePlayerController(GameObject* vGameObject)
{
	return DBG_NEW PlayerController(vGameObject);
}

CollisionColorChanger* GameObject::CreateCollisionColorChanger(Vector3 vColor, GameObject* vGameObject)
{
	return DBG_NEW CollisionColorChanger(vColor, vGameObject);
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
		(int)transform.position.x,
		(int)transform.position.y,
		renderer->width,
		renderer->height
		};

		
		SDL_SetRenderDrawColor(pRenderer, renderer->color.x, renderer->color.y, renderer->color.z, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(pRenderer, &rect);
	}
}