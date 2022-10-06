#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "CollisionColorChanger.h"
#include "PlayerController.h"
#include "RectangleCollider.h"
#include "RectangleRenderer.h"
#include "Transform.h"

class SDL_Renderer;

class GameObject
{
public:
	GameObject()
	{
        renderer = nullptr; 
        collider = nullptr; 
        player = nullptr;
        colorChanger = nullptr;
	}

    ~GameObject();

    RectangleRenderer* CreateRenderer(float vWidth, float vHeight, Vector3 vColor);
    RectangleCollider* CreateCollider(float vWidth, float vHeight, GameObject* vGameObject);
    PlayerController* CreatePlayerController(GameObject* vGameObject);
    CollisionColorChanger* CreateCollisionColorChanger(Vector3 vColor, GameObject* vGameObject);

    Transform& GetTransform() { return transform; }
    RectangleRenderer* GetRenderer() { return renderer; }
    RectangleCollider* GetCollider() { return collider; }
    PlayerController* GetPlayer() { return player; }
    CollisionColorChanger* GetColorChanger() { return colorChanger; }

    void Update();
    void Render(SDL_Renderer* pRenderer);

protected:
    Transform transform;
    RectangleRenderer* renderer;
    RectangleCollider* collider;
    PlayerController* player;
    CollisionColorChanger* colorChanger;

};

#endif
