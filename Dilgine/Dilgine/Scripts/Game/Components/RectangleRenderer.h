#ifndef RECTANGLE_RENDERER_H
#define RECTANGLE_RENDERER_H

#include "Vector3.h"

#include <vector>

class Transform;
class GameObject;
struct SDL_Renderer;

const int RREND_DEFAULT_WIDTH = 50, RREND_DEFAULT_HEIGHT = 50;
const Vector3 RREND_DEFAULT_COLOR = Vector3(255);

class RectangleRenderer
{
public:
	int width, height;
	Vector3 baseColor;
	Vector3 color;

	GameObject* gameObject;

	enum { compID = 'RREN' };

	static void Create(GameObject& gameObject, int vWidth = RREND_DEFAULT_WIDTH, int vHeight = RREND_DEFAULT_HEIGHT, Vector3 vColor = RREND_DEFAULT_COLOR);

	RectangleRenderer() { width = 50; height = 50; baseColor = RREND_DEFAULT_COLOR; color = RREND_DEFAULT_COLOR; gameObject = nullptr; }
	RectangleRenderer(int vWidth, int vHeight, Vector3 vColor, GameObject* pGameObject) { width = vWidth; height = vHeight; baseColor = vColor; color = vColor; gameObject = pGameObject; }

	static void RenderAll(SDL_Renderer*& prRenderer);
};

#endif
