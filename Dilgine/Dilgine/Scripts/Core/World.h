#ifndef WORLD_H
#define WORLD_H
#include "GameObject.h"

#include <vector>

struct SDL_Window;

struct Components
{
	static const int MAX_GAMEOBJECTS = 500;
	Transform transformComponents[MAX_GAMEOBJECTS];
	RectangleRenderer rectRendererComponents[MAX_GAMEOBJECTS];
	RectangleCollider rectColliderComponents[MAX_GAMEOBJECTS];
	PlayerController playerControllerComponents[MAX_GAMEOBJECTS];
	CollisionColorChanger colorChangeComponents[MAX_GAMEOBJECTS];
};

class World
{
private:
	std::vector<GameObject> gameObjects;

	Components components;

public:
	
	int activeGameObjects = 0;
	int activeTransforms = 0;
	int activeRectRenderers = 0;
	int activeRectColliders = 0;
	int activePlayerControllers = 0;
	int activeColorChange = 0;

	World() { }
	~World() { CleanUp(); }
	
	void Init(SDL_Window* pWindow);
	void CleanUp();

	std::vector<GameObject>& GetGameObjects() { return gameObjects; }
	Components& GetComponents() { return components; }

	void Update();

	void Render(SDL_Renderer*& prRenderer);
};

namespace gpr460
{
	static World* world;
}

#endif
