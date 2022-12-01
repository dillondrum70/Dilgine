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
	stream.ignore(100, ' ');
	std::getline(stream, texture, ']');	//Read texture path until you reach the closing bracket of component in data file

	gameObject.CreateMeshRenderer(gameObject, model, texture);

	//stream.ignore(100, ']');	//Don't need to ignore, getline handled that
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