#include "VulkanContent.h"

#include "System.h"

void dVulkan::InitVulkan(SDL_Window* window)
{
    //Vulkan instance
    //gpr460::engine->vInstance

    //Application info, gives information to driver, optional
    VkApplicationInfo aInfo{};
    aInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    aInfo.pApplicationName = "Triangle";
    aInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    aInfo.pEngineName = "Dilgine";
    aInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    aInfo.apiVersion = VK_API_VERSION_1_0;

    //Create info, global extension and validation layers we want to use, applies to entire program, required
    VkInstanceCreateInfo cInfo{};
    cInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    cInfo.pApplicationInfo = &aInfo;

    //Get extensions from SDL
    uint32_t sdlExtensionCount = 0;
    const char** sdlExtensions = nullptr;
    SDL_Vulkan_GetInstanceExtensions(window, &sdlExtensionCount, sdlExtensions);
    cInfo.enabledExtensionCount = sdlExtensionCount;
    cInfo.ppEnabledExtensionNames = sdlExtensions;

    //Check if any validation layers don't exist
    if (enableValidationLayers)
    {
        //Loads in validation layer names and errors if any of them don't exist
        if (!CheckValidationSupport())
        {
            throw std::runtime_error("One or more validation layers requested, but not available");
        }

        //Set CreateInfo values for validation layers
        cInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        cInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
    {
        cInfo.enabledLayerCount = 0;
    }

    //Pass in creation info, custom callback (which we don't need), and the actual instance
    VkResult result = vkCreateInstance(&cInfo, nullptr, &gpr460::engine->vInstance);

    if (result != VK_SUCCESS)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_CREATE_SDL_RENDERER_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_CREATE_SDL_RENDERER_FAILED);
        throw std::runtime_error("Failed to create Vulkan Instance");
    }
}

bool dVulkan::CheckValidationSupport()
{
    //Get number of layers
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    //Get properties of validation layers
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    //Loop through and check if all layers exist
    for (const char* layerName : validationLayers)
    {
        bool layerFound = false;

        //Actual check to see if layer existss
        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        //If any layer is not found, we return false
        if (!layerFound)
        {
            return false;
        }
    }

    return true;
}