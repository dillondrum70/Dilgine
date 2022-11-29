#ifndef WORLD_H
#define WORLD_H
#include "GameObject.h"
#include "Serialization.h"

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
	MeshRenderer meshRendererComponents[gpr460::MAX_GAMEOBJECTS];
};

class World
{
private:
	SDL_Window* window = nullptr;	//Necessary to store here to pass to Vulkan

	GameObject gameObjects[gpr460::MAX_GAMEOBJECTS];

	Components components;

	Serialize serial;

public:
	
	int activeGameObjects = 0;
	int activeTransforms = 0;
	int activeRectRenderers = 0;
	int activeRectColliders = 0;
	int activePlayerControllers = 0;
	int activeColorChange = 0;
	int activeMeshRenderers = 0;

	/////////////
	//float lastMouseX = 2;
	//float lastMouseY = 2;
	float mouseX = 0;
	float mouseY = 80;
	////////////

	World() { }
	~World() { CleanUp(); }
	
	void Init(SDL_Window* pWindow);
	void CleanUp();

	GameObject* GetGameObjects() { return gameObjects; }
	Components& GetComponents() { return components; }

	/*void CreateTransform(GameObject& rObj, Vector2 vPos);
	void CreateRectangleRenderer(GameObject& rObj, int vWidth, int vheight, Vector3 vColor);
	void CreateRectangleCollider(GameObject& rObj, int vWidth, int vHeight);
	void CreatePlayerController(GameObject& rObj, int vSpeed);
	void CreateCollisionColorChange(GameObject& rObj, Vector3 vColor);*/

	void AddGameObject(GameObject& rObj);

	void PrintFrame();

	void Update();

	void Render(SDL_Renderer*& prRenderer);

	void UpdateUniformBuffers(uint32_t currentImage);
};

#endif
