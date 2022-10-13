#ifndef WORLD_H
#define WORLD_H
#include "GameObject.h"

#include <vector>

struct SDL_Window;


namespace gpr460
{
	static const int MAX_GAMEOBJECTS = 500;
}

struct Components
{
	Transform transformComponents[gpr460::MAX_GAMEOBJECTS];
	RectangleRenderer rectRendererComponents[gpr460::MAX_GAMEOBJECTS];
	RectangleCollider rectColliderComponents[gpr460::MAX_GAMEOBJECTS];
	PlayerController playerControllerComponents[gpr460::MAX_GAMEOBJECTS];
	CollisionColorChanger colorChangeComponents[gpr460::MAX_GAMEOBJECTS];
};

class World
{
private:
	GameObject gameObjects[gpr460::MAX_GAMEOBJECTS];

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

	GameObject* GetGameObjects() { return gameObjects; }
	Components& GetComponents() { return components; }

	void Update();

	void Render(SDL_Renderer*& prRenderer);
};

#endif
