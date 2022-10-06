#ifndef RECTANGLE_RENDERER_H
#define RECTANGLE_RENDERER_H

#include "Vector3.h"

class RectangleRenderer
{
public:
	int width, height;
	Vector3 color;

	RectangleRenderer() { width = 1; height = 1; color = Vector3(255); }
	RectangleRenderer(int vWidth, int vHeight, Vector3 vColor) { width = vWidth; height = vHeight; color = vColor; }
};

#endif
