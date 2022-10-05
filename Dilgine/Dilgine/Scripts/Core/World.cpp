#include "World.h"

#include "System.h"

#include "SDL2/SDL.h"

void World::Init()
{
	background = DBG_NEW GameObject();
	Transform backTrans = background->GetTransform();
	//SDL_GetWindowBordersSize()
	//backTrans.position = Vector2()
}

void World::CleanUp();

void World::Update();

void World::Render();