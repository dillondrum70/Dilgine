#include "MeshRenderer.h"
#include "Transform.h"
#include "GameObject.h"
#include "System.h"
#include "VulkanObject.h"

#include "vulkan/vulkan.h"

//MREN [modelPath texturePath]
void MeshRenderer::Deserialize(GameObject& gameObject, std::istream& stream)
{
	stream.ignore(100, '[');

	std::string model, texture;

	stream >> model;
	stream >> texture;
	//int w = 0, h = 0;
	//Vector3 col;

	//stream >> w;
	//stream >> h;

	/*stream.ignore(100, '(');
	stream >> col.x;
	stream >> col.y;
	stream >> col.z;*/
	//stream.ignore(100, ')');
	gameObject.CreateMeshRenderer(gameObject, model, texture);

	stream.ignore(100, ']');
}

void MeshRenderer::Create(GameObject& gameObject, std::string modelFilePath, std::string textureFilePath)
{
	gameObject.CreateMeshRenderer(gameObject, modelFilePath, textureFilePath);
}

void MeshRenderer::RenderAll(SDL_Renderer*& prRenderer)
{
	Components& components = gpr460::engine->world->GetComponents();

	for (int i = 0; i < gpr460::engine->world->activeMeshRenderers; i++)
	{
		MeshRenderer& meshRender = components.meshRendererComponents[i];

		if (meshRender.gameObject && meshRender.gameObject->GetTransform() && prRenderer)
		{
			Transform* trans = meshRender.gameObject->GetTransform();


		}
	}
}