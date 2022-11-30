#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "Vector3.h"

#include <vector>
#include <iostream>

class Transform;
class GameObject;
struct SDL_Renderer;
class VulkanObject;

//const int MREND_DEFAULT_WIDTH = 1, MREND_DEFAULT_HEIGHT = 1;
//const Vector3 CREND_DEFAULT_COLOR = Vector3(255);

class MeshRenderer
{
public:
	int width = 1, height = 1;
	//Vector3 baseColor;
	//Vector3 color;

	GameObject* gameObject = nullptr;

	VulkanObject* vulkanObj = nullptr;

	enum { compID = 'MREN' };

	static void Deserialize(GameObject& gameObject, std::istream& stream);

	static void Create(GameObject& gameObject, std::string modelFilePath, std::string textureFilePath);

	MeshRenderer() { gameObject = nullptr; vulkanObj = nullptr; }
	MeshRenderer(GameObject* pGameObject) { gameObject = pGameObject; }

	static void RenderAll(SDL_Renderer*& prRenderer);
};

#endif
