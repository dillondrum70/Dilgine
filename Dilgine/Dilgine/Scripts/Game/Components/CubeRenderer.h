#ifndef CUBE_RENDERER_COMPONENT_H
#define CUBE_RENDERER_COMPONENT_H
#include "Vector3.h"

#include <vector>
#include <iostream>

class Transform;
class GameObject;
struct SDL_Renderer;

const int CREND_DEFAULT_WIDTH = 1, CREND_DEFAULT_HEIGHT = 1;
//const Vector3 CREND_DEFAULT_COLOR = Vector3(255);

class CubeRenderer
{
public:
	int width, height;
	//Vector3 baseColor;
	//Vector3 color;

	GameObject* gameObject;

	VulkanObject* vulkanObj;

	enum { compID = 'CREN' };

	static void Deserialize(GameObject& gameObject, std::istream& stream);

	static void Create(GameObject& gameObject, int vWidth = CREND_DEFAULT_WIDTH, int vHeight = CREND_DEFAULT_HEIGHT/*, Vector3 vColor = CREND_DEFAULT_COLOR*/);

	CubeRenderer() { width = 1; height = 1; /*baseColor = CREND_DEFAULT_COLOR; color = CREND_DEFAULT_COLOR;*/ gameObject = nullptr; }
	CubeRenderer(int vWidth, int vHeight, GameObject* pGameObject) { width = vWidth; height = vHeight; /*baseColor = vColor; color = vColor;*/ gameObject = pGameObject; }

	static void RenderAll(SDL_Renderer*& prRenderer);
};

#endif
