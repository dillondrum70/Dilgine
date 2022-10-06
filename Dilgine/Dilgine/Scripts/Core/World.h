#ifndef WORLD_H
#define WORLD_H
#include "GameObject.h"

#include <vector>

class SDL_Window;

class World
{
private:
	std::vector<GameObject*> gameObjects;

public:
	World() { }
	~World() { CleanUp(); }
	
	void Init(SDL_Window* pWindow);
	void CleanUp();

	void Update();

	void Render();
};

#endif
