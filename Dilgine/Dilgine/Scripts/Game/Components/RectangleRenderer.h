#ifndef RECTANGLE_RENDERER_H
#define RECTANGLE_RENDERER_H

#include "Vector3.h"

#include <vector>

class Transform;
class GameObject;
struct SDL_Renderer;

class RectangleRenderer
{
public:
	int width, height;
	Vector3 baseColor;
	Vector3 color;

	GameObject* gameObject;

	RectangleRenderer() { width = 50; height = 50; baseColor = Vector3(255); color = Vector3(255); gameObject = nullptr; }
	RectangleRenderer(int vWidth, int vHeight, Vector3 vColor, GameObject* pGameObject) { width = vWidth; height = vHeight; baseColor = vColor; color = vColor; gameObject = pGameObject; }

	static void RenderAll(SDL_Renderer*& prRenderer);
};

#endif
