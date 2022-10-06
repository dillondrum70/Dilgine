#ifndef WORLD_H
#define WORLD_H
#include "GameObject.h"

#include <vector>

struct SDL_Window;

class World
{
private:
	std::vector<GameObject*> gameObjects;

public:
	World() { }
	~World() { CleanUp(); }
	
	void Init(SDL_Window* pWindow);
	void CleanUp();

	std::vector<GameObject*>& GetGameObjects() { return gameObjects; }

	void Update();

	void Render(SDL_Renderer*& prRenderer);
};

#endif
