#include "CubeRenderer.h"
#include "Transform.h"
#include "GameObject.h"
#include "System.h"

#include "SDL2/SDL.h"

//CREN [w h]
void CubeRenderer::Deserialize(GameObject& gameObject, std::istream& stream)
{
	stream.ignore(100, '[');

	int w = 0, h = 0;
	//Vector3 col;

	stream >> w;
	stream >> h;

	/*stream.ignore(100, '(');
	stream >> col.x;
	stream >> col.y;
	stream >> col.z;*/
	//stream.ignore(100, ')');
	gameObject.CreateCubeRenderer(gameObject, w, h);

	stream.ignore(100, ']');
}

void CubeRenderer::Create(GameObject& gameObject, int vWidth, int vHeight)
{
	gameObject.CreateCubeRenderer(gameObject, vWidth, vHeight);
}

void CubeRenderer::RenderAll(SDL_Renderer*& prRenderer)
{
	Components& components = gpr460::engine->world->GetComponents();

	for (int i = 0; i < gpr460::engine->world->activeCubeRenderers; i++)
	{
		CubeRenderer& cubeRender = components.cubeRendererComponents[i];

		if (cubeRender.gameObject && cubeRender.gameObject->GetTransform() && prRenderer)
		{
			Transform* trans = cubeRender.gameObject->GetTransform();

			
		}
	}
}