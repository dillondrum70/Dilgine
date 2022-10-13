#include "RectangleRenderer.h"
#include "Transform.h"

#include "SDL2/SDL.h"

void RectangleRenderer::Render(std::vector<RectangleRenderer>& renderers, std::vector<Transform>& transforms, int rendererCount, SDL_Renderer*& prRenderer)
{
	SDL_Rect* rects = new SDL_Rect[rendererCount];

	for (int i = 0; i < rendererCount; i++)
	{
		rects[i].w = renderers[i].width;
		rects[i].h = renderers[i].height;
		rects[i].x = transforms[i].position.x;
		rects[i].y = transforms[i].position.y;
	}

	SDL_RenderFillRects(prRenderer, rects, rendererCount);
}