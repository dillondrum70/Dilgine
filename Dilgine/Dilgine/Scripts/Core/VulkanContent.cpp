#include "VulkanContent.h"

#include "System.h"

#include <set>

void EngineVulkan::Cleanup()
{
    vkDestroySurfaceKHR(vInstance, surface, nullptr);
    vkDestroyDevice(logicalDevice, nullptr);
    vkDestroyInstance(vInstance, nullptr);
}

void EngineVulkan::InitVulkan(SDL_Window* window)
{
    CreateInstance(window);

    CreateSurface(window);

    ChoosePhysicalDevice();

    CreateLogicalDevice();
}

void EngineVulkan::CreateInstance(SDL_Window* window)
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

    //Get number of extensions from SDL
    uint32_t sdlExtensionCount = 0;
    SDL_Vulkan_GetInstanceExtensions(window, &sdlExtensionCount, nullptr);

    //Get actual extensions from SDL
    std::vector<const char*> sdlExtensions; //can add additional extensions to this
    size_t additionalExstensionCount = 0;
    sdlExtensions.resize(additionalExstensionCount + sdlExtensionCount);
    SDL_Vulkan_GetInstanceExtensions(window, &sdlExtensionCount, sdlExtensions.data() + additionalExstensionCount);

    //Pass to Create Info struct
    cInfo.enabledExtensionCount = sdlExtensionCount;
    cInfo.ppEnabledExtensionNames = sdlExtensions.data();

    InitValidationLayers(&cInfo);

    //Pass in creation info, custom callback (which we don't need), and the actual instance
    VkResult result = vkCreateInstance(&cInfo, nullptr, &vInstance);

    if (result != VK_SUCCESS)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_CREATE_SDL_RENDERER_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_CREATE_SDL_RENDERER_FAILED);
        throw std::runtime_error("Failed to create Vulkan Instance");
    }
}

void EngineVulkan::InitValidationLayers(VkInstanceCreateInfo* cInfo)
{
    //Check if any validation layers don't exist
    if (enableValidationLayers)
    {
        //Loads in validation layer names and errors if any of them don't exist
        if (!CheckValidationSupport())
        {
            gpr460::engine->system->ErrorMessage(gpr460::ERROR_VALIDATION_LAYER_DNE);
            gpr460::engine->system->LogToErrorFile(gpr460::ERROR_VALIDATION_LAYER_DNE);
            throw std::runtime_error("One or more validation layers requested, but not available");
        }

        //Set CreateInfo values for validation layers
        cInfo->enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        cInfo->ppEnabledLayerNames = validationLayers.data();
    }
    else
    {
        cInfo->enabledLayerCount = 0;
    }
}

void EngineVulkan::ChoosePhysicalDevice()
{
    //List devices
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(gpr460::engine->vulkanEngine.vInstance, &deviceCount, nullptr);

    //Error if no GPUs can support Vulkan
    if (deviceCount == 0)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_NO_GPU_VULKAN_SUPPORT);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_NO_GPU_VULKAN_SUPPORT);
        throw std::runtime_error("Failed to find GPUs with Vulkan support");
    }

    //Get physical devices
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(gpr460::engine->vulkanEngine.vInstance, &deviceCount, devices.data());

    std::cout << "Physical Devices:\n";
    //Check if devices have capabilities we need
    for (const auto& device : devices)
    {
        //Simply accept first valid GPU we find
        if (CheckDevice(device))
        {
            physicalDevice = device;
            break;
        }
    }

    //If physical device not set, we didn't find a good enough GPU
    if (physicalDevice == VK_NULL_HANDLE)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_NO_CAPABLE_GPU);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_NO_CAPABLE_GPU);
        throw std::runtime_error("Failed to find capable GPU");
    }
    else
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

        std::cout << "Chosen Device:\n\t" << deviceProperties.deviceName << std::endl;
    }
}

void EngineVulkan::CreateLogicalDevice()
{
    QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

    //Information needed to create logical device
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;

        //Required even if there is only one queue
        queueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
    }

    //Info about device's features
    VkPhysicalDeviceFeatures deviceFeatures{};

    //Required create info for device
    VkDeviceCreateInfo cInfo{};
    cInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    cInfo.pQueueCreateInfos = queueCreateInfos.data();
    cInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    cInfo.pEnabledFeatures = &deviceFeatures;

    //Extension info for VkDeviceCreateInfo
    cInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    cInfo.ppEnabledExtensionNames = deviceExtensions.data();

    //Pass validation layer info if enabled
    if (enableValidationLayers)
    {
        cInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        cInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
    {
        cInfo.enabledLayerCount = 0;
    }

    //Create logical device
    if (vkCreateDevice(physicalDevice, &cInfo, nullptr, &logicalDevice) != VK_SUCCESS)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_CREATE_LOGICAL_DEVICE_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_CREATE_LOGICAL_DEVICE_FAILED);
        throw std::runtime_error("Failed to create logical device");
    }

    //Get device queue info
    vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &presentQueue);
}

void EngineVulkan::CreateSurface(SDL_Window* window)
{
    //SDL's implementation of creating a Vulkan surface
    if (SDL_Vulkan_CreateSurface(window, vInstance, &surface) == SDL_FALSE)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_CREATE_SURFAE_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_CREATE_SURFAE_FAILED);
        throw std::runtime_error("Failed to create window surface");
    }
}

bool EngineVulkan::CheckValidationSupport()
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

bool EngineVulkan::CheckDevice(VkPhysicalDevice device)
{
    //Get properties
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    std::cout << "\t" << deviceProperties.deviceName << std::endl;

    //Get features
    //VkPhysicalDeviceFeatures deviceFeatures;
    //vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    QueueFamilyIndices indices = FindQueueFamilies(device);

    //Return true if physical device is a discrete (non-integrated) GPU and has geometry shader feature
    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && indices.isComplete();
}

bool EngineVulkan::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
    //Get number of extensions
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    //Load in extensions
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    //Define extensions needed
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    //Erase extensions from required list as they are found
    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    //If required extensions list is not empty, we have not found everything we need
    return requiredExtensions.empty();
}

QueueFamilyIndices EngineVulkan::FindQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    //Get number of queue families
    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    //Get information about queue families
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    

    //Check each queue family and find one that supports VK_QUEUE_GRAPHICS_BIT
    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        //Bitwise AND to tell if it supports
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }

        //Check for presentation support
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport)
        {
            indices.presentFamily = i;
        }

        //Only need to find index of first queue family that supports VK_QUEUE_GRAPHICS_BIT
        if (indices.isComplete())
        {
            break;
        }

        i++;
    }

    return indices;
}