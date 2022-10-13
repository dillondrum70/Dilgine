#ifndef RECTANGLE_RENDERER_H
#define RECTANGLE_RENDERER_H

#include "Vector3.h"

#include <vector>

class Transform;
struct SDL_Renderer;

class RectangleRenderer
{
public:
	int width, height;
	Vector3 baseColor;
	Vector3 color;

	RectangleRenderer() { width = 50; height = 50; baseColor = Vector3(255); color = Vector3(255); }
	RectangleRenderer(int vWidth, int vHeight, Vector3 vColor) { width = vWidth; height = vHeight; baseColor = vColor; color = vColor; }

	static void Render(std::vector<RectangleRenderer>& renderers, std::vector<Transform>& transforms, int rendererCount, SDL_Renderer*& prRenderer);
};

#endif
