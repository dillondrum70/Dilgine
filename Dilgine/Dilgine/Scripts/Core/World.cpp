#include "World.h"

#include "System.h"
//#include <windows.h>

#include "SDL2/SDL.h"
#include "vulkan/vulkan.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
	serial.LoadLevel(path);

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

//void World::CreateTransform(GameObject& rObj, Vector2 vPos)
//{
//	if (activeTransforms + 1 >= gpr460::MAX_GAMEOBJECTS)
//	{
//		gpr460::engine->system->ErrorMessage(gpr460::ERROR_COMPONENT_OVERFLOW);
//		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_COMPONENT_OVERFLOW);
//	}
//
//	components.transformComponents[activeTransforms] = Transform(vPos);
//	rObj.SetTransform(&components.transformComponents[activeTransforms]);
//	activeTransforms++;
//}
//
//void World::CreateRectangleRenderer(GameObject& rObj, int vWidth, int vheight, Vector3 vColor)
//{
//	if (activeRectRenderers + 1 >= gpr460::MAX_GAMEOBJECTS)
//	{
//		gpr460::engine->system->ErrorMessage(gpr460::ERROR_COMPONENT_OVERFLOW);
//		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_COMPONENT_OVERFLOW);
//	}
//
//	components.rectRendererComponents[activeRectRenderers] = RectangleRenderer(vWidth, vheight, vColor, &gameObjects[activeGameObjects]);
//	rObj.SetRenderer(&components.rectRendererComponents[activeRectRenderers]);
//	activeRectRenderers++;
//}
//
//void World::CreateRectangleCollider(GameObject& rObj, int vWidth, int vHeight)
//{
//
//	if (activeRectColliders + 1 >= gpr460::MAX_GAMEOBJECTS)
//	{
//		gpr460::engine->system->ErrorMessage(gpr460::ERROR_COMPONENT_OVERFLOW);
//		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_COMPONENT_OVERFLOW);
//	}
//
//	components.rectColliderComponents[activeRectColliders] = RectangleCollider(vWidth, vHeight, &gameObjects[activeGameObjects]);
//	rObj.SetCollider(&components.rectColliderComponents[activeRectColliders]);
//	activeRectColliders++;
//}
//
//void World::CreatePlayerController(GameObject& rObj, int vSpeed)
//{
//	if (activePlayerControllers + 1 >= gpr460::MAX_GAMEOBJECTS)
//	{
//		gpr460::engine->system->ErrorMessage(gpr460::ERROR_COMPONENT_OVERFLOW);
//		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_COMPONENT_OVERFLOW);
//	}
//
//	components.playerControllerComponents[activePlayerControllers] = (PlayerController(vSpeed, &gameObjects[activeGameObjects]));
//	rObj.SetPlayer(&components.playerControllerComponents[activePlayerControllers]);
//	activePlayerControllers++;
//}
//
//void World::CreateCollisionColorChange(GameObject& rObj, Vector3 vColor)
//{
//	if (activeColorChange + 1 >= gpr460::MAX_GAMEOBJECTS)
//	{
//		gpr460::engine->system->ErrorMessage(gpr460::ERROR_COMPONENT_OVERFLOW);
//		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_COMPONENT_OVERFLOW);
//	}
//
//	components.colorChangeComponents[activeColorChange] = CollisionColorChanger(vColor, &gameObjects[activeGameObjects]);
//	rObj.SetColorChanger(&components.colorChangeComponents[activeColorChange]);
//	activeColorChange++;
//}

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
	PlayerController::UpdateAll();

	//Checks each collider against each other collider once
	//for(int i = 0; ...
		//for(int j = i; ...
	CollisionColorChanger::UpdateAll();

	//Call AllCollisions on player and print returned collider addresses
	{
		int count;
		RectangleCollider* colliders = components.rectColliderComponents[0].AllCollisions(count);

		for (int i = 0; i < count; i++)
		{
			//std::cout << "Returned Collider: " << &colliders[i] << std::endl;
		}
		//std::cout << std::endl;
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
	UpdateUniformBuffer(vulkan.currentFrame);

	//Must manually reset to an unsignaled state afterwards
	//Do so after out of date check to avoid a deadlock where nothing is executed because the fence resets us before reaching the check since nothing would have been done last frame
	vkResetFences(vulkan.logicalDevice, 1, &vulkan.inFlightFences[vulkan.currentFrame]);

	//Reset the command buffer foor new frame
	vkResetCommandBuffer(vulkan.commandBuffers[vulkan.currentFrame], 0);

	//Record the commands to buffer
	vulkan.RecordCommandBuffer(vulkan.commandBuffers[vulkan.currentFrame], imageIndex);

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


void World::UpdateUniformBuffer(uint32_t currentImage)
{
	//Calculate time since started rendering, independent of frame rate
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	VkExtent2D extents = gpr460::engine->vulkanEngine.swapChainExtent;

	UniformBufferObject ubo{};
	//Calculate model matrix, takes transformation, rotation, and rotation axis, rotates 90 degrees per second
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));	

	//View matrix (eye position, center look position, up vector), looks from above at a 45 degree angle
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));	

	//Projection matrix, (Y field of view, aspect ratio, near clip, far clip), 45 degree vertical field of view
	ubo.proj = glm::perspective(glm::radians(45.0f), extents.width / (float)extents.height, 0.1f, 10.0f);	
	ubo.proj[1][1] *= -1;	//glm designed for OpenGL where Y clip coordinate is inverted, must undo the invert done by glm, otherwise, image is rendered upside-down

	//Copy memory to uniform buffers mapped
	memcpy(gpr460::engine->vulkanEngine.uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}