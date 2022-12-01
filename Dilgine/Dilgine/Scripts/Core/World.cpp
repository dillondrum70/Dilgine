#include "World.h"

#include "System.h"
//#include <windows.h>

#include "SDL2/SDL.h"
#include "vulkan/vulkan.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"

#include <chrono>

const std::string path = "..\\Dilgine\\Data\\level0.dat";

void World::Init(SDL_Window* pWindow)
{
	int width, height;
	SDL_GetWindowSize(pWindow, &width, &height);

	window = pWindow;

	/*TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	std::wcout << std::wstring(buffer).substr(0, pos);*/
	//std::string path = "..\\Dilgine\\Data\\level0.dat";
	//std::cout << std::endl << path << std::endl;
	
	//serial.LoadLevel(path);

	GameObject player;
	Transform::Create(player, Vector3(0.0f), Vector3(0.0f), Vector3(1.0f));
	PlayerController::Create(player, 0.05f);
	MeshRenderer::Create(player, CUBE_PATH, PAUL_TEXTURE_PATH);
	AddGameObject(player);

	GameObject room;
	Transform::Create(room, Vector3(0.0f), Vector3(0.0f), Vector3(3.0f));
	MeshRenderer::Create(room, VIKING_MODEL_PATH, VIKING_TEXTURE_PATH);
	AddGameObject(room);

	GameObject camera;
	Camera::Create(camera, .3f, 0.05f, 0.01f, Vector3(0.0f), Vector3(5.0f, 5.0f, 5.0f));
	AddGameObject(camera);

	mainCamera = &components.cameraComponents[0];

	/*GameObject background;
	//Add Transform component
	Transform::Create(background, Vector2(width / 2, height / 2));
	//Add RectangleRenderer component
	RectangleRenderer::Create(background, width, height, Vector3(150, 150, 150));
	AddGameObject(background);

	GameObject player;
	Transform::Create(player, Vector2(width / 4, height / 2));
	RectangleRenderer::Create(player, 50, 50, Vector3(0, 255, 255));
	RectangleCollider::Create(player, 50, 50);
	PlayerController::Create(player, 1);
	CollisionColorChanger::Create(player, Vector3(0, 0, 255));
	AddGameObject(player);

	int wallCount = 20;
	for (int i = 0; i < wallCount; i++)
	{
		GameObject wall;
		Transform::Create(wall, Vector2(rand() % width, rand() % height));
		RectangleRenderer::Create(wall, 50, 50, Vector3(255, 150, 0));
		RectangleCollider::Create(wall, 50, 50);
		CollisionColorChanger::Create(wall, Vector3(0, 0, 255));
		AddGameObject(wall);
	}*/
}

void World::AddGameObject(GameObject& rObj)
{
	if (activeGameObjects + 1 >= gpr460::MAX_GAMEOBJECTS)
	{
		gpr460::engine->system->ErrorMessage(gpr460::ERROR_GAMEOBJECT_OVERFLOW);
		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_GAMEOBJECT_OVERFLOW);
	}

	//Add GameObject to cache
	gameObjects[activeGameObjects] = rObj;
	//INC GameObject count
	activeGameObjects++;
}

void World::CleanUp()
{
	
}

void World::PrintFrame()
{
	int* frame = gpr460::engine->stack.alloc<int>();
	*frame = gpr460::engine->frame;
	//std::cout << "Frame: " << *frame << std::endl;
	//std::cout << "Returned Frame Address: " << frame << std::endl;
}

void World::Update()
{
	Camera::UpdateMain();	//Update just main camera, there is also a function to update all cameras

	PlayerController::UpdateAll();

	//Checks each collider against each other collider once
	//for(int i = 0; ...
		//for(int j = i; ...
	CollisionColorChanger::UpdateAll();

	//Call AllCollisions on player and print returned collider addresses
	{
		int count;
		//Basically check if there are any rect collider components before calling AllCollisions
		if (components.rectColliderComponents[0].GetGameObject() != nullptr)
		{
			RectangleCollider* colliders = components.rectColliderComponents[0].AllCollisions(count);

			//for (int i = 0; i < count; i++)
			//{
			//	std::cout << "Returned Collider: " << &colliders[i] << std::endl;
			//}
			//std::cout << std::endl;
		}
	}

	//PrintFrame();
	//std::cout << std::endl << std::endl;;

	//Clear stack at end of each frame
	gpr460::engine->stack.clear();
}

void World::Render(SDL_Renderer*& prRenderer)
{
	//SDL_SetRenderDrawColor(prRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	//SDL_RenderClear(prRenderer);

	//RectangleRenderer::RenderAll(prRenderer);

	//SDL_RenderPresent(prRenderer);

	//Reference to vulkan engine
	EngineVulkan& vulkan = gpr460::engine->vulkanEngine;

	//First frame's fence (inside of EngineVulkan) is set to signaled since there are no previous frames to pull from
	//Wait on host for any or all fences to be signaled, we wait for all with VK_TRUE, shouldn't time out so we have UINT64_MAX
	vkWaitForFences(vulkan.logicalDevice, 1, &vulkan.inFlightFences[vulkan.currentFrame], VK_TRUE, UINT64_MAX);

	//Get the next image, don't time out, signal imageAvailableSemaphore when done, save index of image from swapChainImages array
	uint32_t imageIndex = 0;
	VkResult result = vkAcquireNextImageKHR(vulkan.logicalDevice, vulkan.swapChain, UINT64_MAX, vulkan.imageAvailableSemaphores[vulkan.currentFrame], VK_NULL_HANDLE, &imageIndex);
	
	//Need to recreate swapchain and restart render process if out of date
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		vulkan.RecreateSwapChain(window);
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		gpr460::engine->system->ErrorMessage(gpr460::ERROR_ACQUIRE_SWAPCHAIN_IMAGE_FAILED);
		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_ACQUIRE_SWAPCHAIN_IMAGE_FAILED);
		throw std::runtime_error("Failed to acquire swapchain image");
	}

	//Update the uniform buffer
	UpdateUniformBuffers(vulkan.currentFrame);

	//Must manually reset to an unsignaled state afterwards
	//Do so after out of date check to avoid a deadlock where nothing is executed because the fence resets us before reaching the check since nothing would have been done last frame
	vkResetFences(vulkan.logicalDevice, 1, &vulkan.inFlightFences[vulkan.currentFrame]);

	//Reset the command buffer foor new frame
	vkResetCommandBuffer(vulkan.commandBuffers[vulkan.currentFrame], 0);

	//Record the commands to buffer
	vulkan.RecordCommandBuffers(vulkan.commandBuffers[vulkan.currentFrame], imageIndex);

	//Info to submit command buffer
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	//Tell command buffer which semaphores to waiton before execution in which stages
	VkSemaphore waitSemaphores[] = { vulkan.imageAvailableSemaphores[vulkan.currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &vulkan.commandBuffers[vulkan.currentFrame];
	//Semaphores to signal when command buffer is done
	VkSemaphore signalSemaphores[] = { vulkan.renderFinishedSemaphores[vulkan.currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	//Submit command buffer to graphics queue to be run on GPU
	if (vkQueueSubmit(vulkan.graphicsQueue, 1, &submitInfo, vulkan.inFlightFences[vulkan.currentFrame]) != VK_SUCCESS)
	{
		gpr460::engine->system->ErrorMessage(gpr460::ERROR_SUBMIT_DRAW_FAILED);
		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_SUBMIT_DRAW_FAILED);
		throw std::runtime_error("Failed to submit draw command buffer");
	}

	//Last step of draw is displaying the generated graphic to the screen using the present queue
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	//Which semaphores to wait on until presentation can happen
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	//swap chains to present images to and index of image for each swap chain, almost always 1
	VkSwapchainKHR swapChains[] = { vulkan.swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;	//Can store VkResult array to check if every swapchain presentation was successful, not needed if only 1, just use return value of the functions

	//Sumbit request to present an image to swap chain
	result = vkQueuePresentKHR(vulkan.presentQueue, &presentInfo);

	//Recreate swapchain if problem occurs, if anything other than these errors occurs (and it is not successful), error
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || vulkan.framebufferResized)
	{
		vulkan.framebufferResized = false;	//Done after semaphores are set and checked to avoud a signaled semaphore not being waited on
		vulkan.RecreateSwapChain(window);
	}
	else if (result != VK_SUCCESS)
	{
		gpr460::engine->system->ErrorMessage(gpr460::ERROR_PRESENT_SWAPCHAIN_IMAGE_FAILED);
		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_PRESENT_SWAPCHAIN_IMAGE_FAILED);
		throw std::runtime_error("Failed to present swapchain image");
	}

	//Increment and wrap frame index
	vulkan.currentFrame = (vulkan.currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}


void World::UpdateUniformBuffers(uint32_t currentImage)
{
	EngineVulkan& vulkan = gpr460::engine->vulkanEngine;

	//Calculate time since started rendering, independent of frame rate
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	VkExtent2D extents = gpr460::engine->vulkanEngine.swapChainExtent;

	for (int i = 0; i < activeMeshRenderers; i++)
	{
		UniformBufferObject ubo{};
		//Calculate model matrix, takes transformation, rotation, and rotation axis, rotates 90 degrees per second
		//ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//ubo.model = glm::identity<glm::mat4>();
		Vector3 pos = components.transformComponents[i].position;
		Vector3 rot = components.transformComponents[i].rotation;
		Vector3 scale = components.transformComponents[i].scale;

		//Calculate matrices for Translation, Rotation, and Scale
		glm::mat4 transMat = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, pos.z));
		glm::mat4 rotMat = glm::eulerAngleXYZ(rot.x, rot.y, rot.z);
		glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, scale.z));

		//Construct model matrix T * R * S
		ubo.model = transMat * rotMat * scaleMat;	

		Vector3 eye = mainCamera->CalculateEyePosition();
		Vector3 lookAt = mainCamera->GetLookAtPosition();
		//View matrix (eye position, center look position, up vector), looks from above at a 45 degree angle
		//ubo.view = glm::lookAt(mainCamera->GetZoom() * glm::vec3(eye.x, eye.y, eye.z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = glm::lookAt((/*((float)mainCamera->GetOrbiting() ? mainCamera->GetZoom() : 1) **/ glm::vec3(eye.x, eye.y, eye.z)), glm::vec3(lookAt.x, lookAt.y, lookAt.z), glm::vec3(0.0f, 0.0f, 1.0f));

		//Projection matrix, (Y field of view, aspect ratio, near clip, far clip), 45 degree vertical field of view
		ubo.proj = glm::perspective(glm::radians(VERTICAL_FOV), extents.width / (float)extents.height, NEAR_PLANE, FAR_PLANE);
		ubo.proj[1][1] *= -1;	//glm designed for OpenGL where Y clip coordinate is inverted, must undo the invert done by glm, otherwise, image is rendered upside-down

		//Copy memory to uniform buffers mapped
		memcpy(components.meshRendererComponents[i].vulkanObj->uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
	}
}