#ifndef RECTANGLE_RENDERER_H

#include "Vector3.h"

class RectangleRenderer
{
public:
	float width, height;
	Vector3 color;

	RectangleRenderer() { width = 1; height = 1; color = Vector3(255); }
	RectangleRenderer(float vWidth, float vHeight, Vector3 vColor) { width = vWidth; height = vHeight; color = vColor; }
};

#endif
