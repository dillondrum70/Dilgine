#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "CollisionColorChanger.h"
#include "PlayerController.h"
#include "RectangleCollider.h"
#include "RectangleRenderer.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"

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
        meshRenderer = nullptr;
        camera = nullptr;
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
    MeshRenderer* GetMeshRenderer() { return meshRenderer; }
    Camera* GetCamera() { return camera; }

    void SetTransform(Transform* vTrans) { transform = vTrans; }
    void SetRectRenderer(RectangleRenderer* vRend) { rectRenderer = vRend; }
    void SetCollider(RectangleCollider* vCol) { collider = vCol; }
    void SetPlayer(PlayerController* vControl) { player = vControl; }
    void SetColorChanger(CollisionColorChanger* vChange) { colorChanger = vChange; }
    void SetMeshRenderer(MeshRenderer* vRend) { meshRenderer = vRend; }
    void SetCamera(Camera* vCam) { camera = vCam; }

    void CreateTransform(GameObject& rObj, Vector3 vPos, Vector3 vRot, Vector3 vScale);
    void CreateRectangleRenderer(GameObject& rObj, int vWidth, int vheight, Vector3 vColor);
    void CreateRectangleCollider(GameObject& rObj, int vWidth, int vHeight);
    void CreatePlayerController(GameObject& rObj, float vSpeed);
    void CreateCollisionColorChange(GameObject& rObj, Vector3 vColor);
    void CreateMeshRenderer(GameObject& rObj, std::string modelFilePath, std::string textureFilePath);
    void CreateCamera(GameObject& rObj, float vMoveSpeed, float vRotateSpeed, float vZoom, Vector3 lookAt, Vector3 eye);

    //void Update(std::vector<GameObject>& gameObjects);
    //void Render(SDL_Renderer*& prRenderer);

protected:
    Transform* transform;
    RectangleRenderer* rectRenderer;
    RectangleCollider* collider;
    PlayerController* player;
    CollisionColorChanger* colorChanger;
    MeshRenderer* meshRenderer;
    Camera* camera;
};

#endif
