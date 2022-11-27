#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "CollisionColorChanger.h"
#include "PlayerController.h"
#include "RectangleCollider.h"
#include "RectangleRenderer.h"
#include "Transform.h"
#include "CubeRendererComponent.h"

#include <vector>

struct SDL_Renderer;

class GameObject
{
public:
	GameObject()
	{
        transform = nullptr;
        rectRenderer = nullptr; 
        collider = nullptr; 
        player = nullptr;
        colorChanger = nullptr;
        cubeRenderer = nullptr;
	}

    ~GameObject();

    //RectangleRenderer CreateRenderer(int vWidth, int vHeight, Vector3 vColor);
    //RectangleCollider CreateCollider(int vWidth, int vHeight, GameObject* vGameObject);
    //PlayerController CreatePlayerController(GameObject* vGameObject);
    //CollisionColorChanger CreateCollisionColorChanger(Vector3 vColor, GameObject* vGameObject);

    Transform* GetTransform() { return transform; }
    RectangleRenderer* GetRectRenderer() { return rectRenderer; }
    RectangleCollider* GetCollider() { return collider; }
    PlayerController* GetPlayer() { return player; }
    CollisionColorChanger* GetColorChanger() { return colorChanger; }
    CubeRenderer* GetCubeRenderer() { return cubeRenderer; }

    void SetTransform(Transform* vTrans) { transform = vTrans; }
    void SetRectRenderer(RectangleRenderer* vRend) { rectRenderer = vRend; }
    void SetCollider(RectangleCollider* vCol) { collider = vCol; }
    void SetPlayer(PlayerController* vControl) { player = vControl; }
    void SetColorChanger(CollisionColorChanger* vChange) { colorChanger = vChange; }
    void SetCubeRenderer(CubeRenderer* vRend) { cubeRenderer = vRend; }

    void CreateTransform(GameObject& rObj, Vector3 vPos);
    void CreateRectangleRenderer(GameObject& rObj, int vWidth, int vheight, Vector3 vColor);
    void CreateRectangleCollider(GameObject& rObj, int vWidth, int vHeight);
    void CreatePlayerController(GameObject& rObj, int vSpeed);
    void CreateCollisionColorChange(GameObject& rObj, Vector3 vColor);
    void CreateCubeRenderer(GameObject& rObj, int vWidth, int vheight);

    //void Update(std::vector<GameObject>& gameObjects);
    //void Render(SDL_Renderer*& prRenderer);

protected:
    Transform* transform;
    RectangleRenderer* rectRenderer;
    RectangleCollider* collider;
    PlayerController* player;
    CollisionColorChanger* colorChanger;
    CubeRenderer* cubeRenderer;
};

#endif
