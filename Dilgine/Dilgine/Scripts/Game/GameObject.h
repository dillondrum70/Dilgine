#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "CollisionColorChanger.h"
#include "PlayerController.h"
#include "RectangleCollider.h"
#include "RectangleRenderer.h"
#include "Transform.h"

#include <vector>

struct SDL_Renderer;

class GameObject
{
public:
	GameObject()
	{
        transform = nullptr;
        renderer = nullptr; 
        collider = nullptr; 
        player = nullptr;
        colorChanger = nullptr;
	}

    ~GameObject();

    //RectangleRenderer CreateRenderer(int vWidth, int vHeight, Vector3 vColor);
    //RectangleCollider CreateCollider(int vWidth, int vHeight, GameObject* vGameObject);
    //PlayerController CreatePlayerController(GameObject* vGameObject);
    //CollisionColorChanger CreateCollisionColorChanger(Vector3 vColor, GameObject* vGameObject);

    Transform* GetTransform() { return transform; }
    RectangleRenderer* GetRenderer() { return renderer; }
    RectangleCollider* GetCollider() { return collider; }
    PlayerController* GetPlayer() { return player; }
    CollisionColorChanger* GetColorChanger() { return colorChanger; }

    void SetTransform(Transform* vTrans) { transform = vTrans; }
    void SetRenderer(RectangleRenderer* vRend) { renderer = vRend; }
    void SetCollider(RectangleCollider* vCol) { collider = vCol; }
    void SetPlayer(PlayerController* vControl) { player = vControl; }
    void SetColorChanger(CollisionColorChanger* vChange) { colorChanger = vChange; }

    void CreateTransform(GameObject& rObj, Vector3 vPos);
    void CreateRectangleRenderer(GameObject& rObj, int vWidth, int vheight, Vector3 vColor);
    void CreateRectangleCollider(GameObject& rObj, int vWidth, int vHeight);
    void CreatePlayerController(GameObject& rObj, int vSpeed);
    void CreateCollisionColorChange(GameObject& rObj, Vector3 vColor);

    //void Update(std::vector<GameObject>& gameObjects);
    //void Render(SDL_Renderer*& prRenderer);

protected:
    Transform* transform;
    RectangleRenderer* renderer;
    RectangleCollider* collider;
    PlayerController* player;
    CollisionColorChanger* colorChanger;

};

#endif
