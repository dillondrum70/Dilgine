#include "RectangleRenderer.h"
#include "Transform.h"
#include "GameObject.h"
#include "System.h"

#include "SDL2/SDL.h"

/*void RectangleRenderer::Render(RectangleRenderer* renderers, Transform* transforms, const int rendererCount, int transCount, SDL_Renderer*& prRenderer)
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
}*/

//RREN [w h (col.x col.y col.z)]
void RectangleRenderer::Deserialize(GameObject& gameObject, std::istream& stream)
{
	stream.ignore(100, '[');

	int w = 0, h = 0;
	Vector3 col;

	stream >> w;
	stream >> h;

	stream.ignore(100, '(');
	stream >> col.x;
	stream >> col.y;
	stream >> col.z;
	//stream.ignore(100, ')');
	gameObject.CreateRectangleRenderer(gameObject, w, h, col);

	stream.ignore(100, ']');
}

void RectangleRenderer::Create(GameObject& gameObject, int vWidth, int vHeight, Vector3 vColor)
{
	gameObject.CreateRectangleRenderer(gameObject, vWidth, vHeight, vColor);
}

void RectangleRenderer::RenderAll(SDL_Renderer*& prRenderer)
{
	Components& components = gpr460::engine->world->GetComponents();
	
	for (int i = 0; i < gpr460::engine->world->activeRectRenderers; i++)
	{
		RectangleRenderer& rectRender = components.rectRendererComponents[i];

		if (rectRender.gameObject && rectRender.gameObject->GetTransform() && prRenderer)
		{
			Transform* trans = rectRender.gameObject->GetTransform();
			SDL_Rect rect = {
			trans->position.x - (rectRender.width / 2),
			trans->position.y - (rectRender.height / 2),
			rectRender.width,
			rectRender.height
			};

			SDL_SetRenderDrawColor(prRenderer, rectRender.color.x, rectRender.color.y, rectRender.color.z, SDL_ALPHA_OPAQUE);
			SDL_RenderFillRect(prRenderer, &rect);
		}
	}
}