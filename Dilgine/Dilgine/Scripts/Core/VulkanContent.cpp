#include "VulkanContent.h"

#include "System.h"

#include <set>
#include <cstdint>
#include <limits>
#include <algorithm>

void EngineVulkan::Cleanup()
{
    for (auto imageView : swapChainImageViews)
    {
        vkDestroyImageView(logicalDevice, imageView, nullptr);
    }

    if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(vInstance, debugMessenger, nullptr);
    }

    vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
    vkDestroySurfaceKHR(vInstance, surface, nullptr);
    vkDestroyDevice(logicalDevice, nullptr);
    vkDestroyInstance(vInstance, nullptr);
}


void EngineVulkan::InitVulkan(SDL_Window* window)
{
    std::cout << "\n------------------------------\n\n";
    std::cout << "Initializing Vulkan...\n";
    std::cout << "\n------------------------------\n\n";

    std::cout << "Initializing Vulkan Instance...\n";
    CreateInstance(window);
    std::cout << "Vulkan Instance Initialized...\n\n";

    std::cout << "Initializing Debug Messenger...\n";
    CreateDebugMessenger();
    std::cout << "Debug Messenger Initialized...\n\n";

    std::cout << "Initializing Vulkan Surface...\n";
    CreateSurface(window);
    std::cout << "Vulkan Surface Initialized...\n\n";

    std::cout << "Initializing Physical Device...\n";
    ChoosePhysicalDevice();
    std::cout << "Physical Device Initialized...\n\n";

    std::cout << "Initializing Logical Device...\n";
    CreateLogicalDevice();
    std::cout << "Logical Device Initialized...\n\n";

    std::cout << "Initializing Swap Chain...\n";
    CreateSwapChain(window);
    std::cout << "Swap Chain Initialized...\n\n";

    std::cout << "Initializing Image Views...\n";
    CreateImageViews();
    std::cout << "Image Views Initialized...\n\n";

    std::cout << "\n------------------------------\n\n";
    std::cout << "Vulkan Initialization Complete\n";
    std::cout << "\n------------------------------\n\n";
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
    std::vector<const char*> sdlExtensions(sdlExtensionCount); //can add additional extensions to this
    //size_t additionalExstensionCount = 0;

    //sdlExtensions.resize(/*additionalExstensionCount + */ sdlExtensionCount);
    SDL_Vulkan_GetInstanceExtensions(window, &sdlExtensionCount, sdlExtensions.data() /* + additionalExstensionCount*/);

    if (enableValidationLayers)
    {
        sdlExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        //additionalExstensionCount++;
        sdlExtensionCount++;
    }

    std::cout << "Required Extensions:\n";
    for (const auto& extension : sdlExtensions)
    {
        std::cout << "\t" << extension << std::endl;
    }

    //Pass to Create Info struct
    cInfo.enabledExtensionCount = sdlExtensionCount /* + additionalExstensionCount*/;
    cInfo.ppEnabledExtensionNames = sdlExtensions.data();

    //Initialize validation layers
    InitValidationLayers(&cInfo);

    //Set instance create info data for debug messenger if validation layers enabled
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers)
    {
        cInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        cInfo.ppEnabledLayerNames = validationLayers.data();

        PopulateDebugMessengerCreateInfo(debugCreateInfo);
        cInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else
    {
        cInfo.enabledLayerCount = 0;

        cInfo.pNext = nullptr;
    }

    //Pass in creation info, custom callback (which we don't need), and the actual instance
    VkResult result = vkCreateInstance(&cInfo, nullptr, &vInstance);

    if (result != VK_SUCCESS)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_CREATE_SDL_RENDERER_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_CREATE_SDL_RENDERER_FAILED);
        throw std::runtime_error("Failed to create Vulkan Instance");
    }
}


VKAPI_ATTR VkBool32 VKAPI_CALL EngineVulkan::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}


void EngineVulkan::CreateDebugMessenger()
{
    if (!enableValidationLayers) return;

    //Create info for debug messenger, specify callback function that writes to console
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    

    if (CreateDebugUtilsMessengerEXT(vInstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_CREATE_DEBUG_MESSENGER_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_CREATE_DEBUG_MESSENGER_FAILED);
        throw std::runtime_error("Failed to set up debug messenger");
    }
}

void EngineVulkan::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = DebugCallback;
    createInfo.pUserData = nullptr;
}


VkResult EngineVulkan::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) 
{
    //Create the debug messenger
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}


void EngineVulkan::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) 
{
    //Destroy messenger if it exists
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}


void EngineVulkan::CreateSurface(SDL_Window* window)
{
    //SDL's implementation of creating a Vulkan surface
    if (SDL_Vulkan_CreateSurface(window, vInstance, &surface) == SDL_FALSE)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_CREATE_SURFACE_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_CREATE_SURFACE_FAILED);
        throw std::runtime_error("Failed to create window surface");
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


void EngineVulkan::CreateSwapChain(SDL_Window* window)
{
    //Get details supported by swap chain necessary to determine other factors
    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice);

    //Choose/determine different swap chain details
    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities, window);

    //Save this data to member variables for later use
    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;

    //Request one more image than minimum so we don't have to wait for driver to complete inernal operations before getting another image
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    //Ensure we do not exceed maximum image count
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    //Declare create info for swapchain based on previously determined data
    VkSwapchainCreateInfoKHR cInfo{};
    cInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    cInfo.surface = surface;
    cInfo.minImageCount = imageCount;
    cInfo.imageFormat = surfaceFormat.format;
    cInfo.imageColorSpace = surfaceFormat.colorSpace;
    cInfo.imageExtent = extent;
    cInfo.imageArrayLayers = 1;
    cInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    //Get queue families and their indices
    QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    //if graphics family and present family are not on the same index, we must draw to one queue then submit to the other (VK_SHARING_MODE_CONCURRENT)
    if (indices.graphicsFamily != indices.presentFamily)
    {
        cInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;    //Ownership of image does not need to be transferred to be used in a different queue
        cInfo.queueFamilyIndexCount = 2;
        cInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else //Otherwise, if they are at the same index, no submission to other queues is necessary (VK_SHARING_MODE_EXCLUSIVE)
    {
        //One queue family maintains ownership of image
        cInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

        //These are optional values
        cInfo.queueFamilyIndexCount = 0;
        cInfo.pQueueFamilyIndices = nullptr;
    }

    //Specify default parameters for how images should be transformed in the swap chain
    cInfo.preTransform = swapChainSupport.capabilities.currentTransform;

    //Specifies that alpha channel of images in swap chain should be opaque
    cInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    //Ignore non-visible regions
    cInfo.clipped = VK_TRUE;

    //Sometimes last swap chain is necessary if swap chain becomes invalid, we don't need it
    cInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(logicalDevice, &cInfo, nullptr, &swapChain) != VK_SUCCESS)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_CREATE_SWAPCHAIN_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_CREATE_SWAPCHAIN_FAILED);
        throw std::runtime_error("Failed to create swapchain");
    }

    //Get count of swapchain images
    vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, nullptr);

    //Resize vector of swapchain images to new count and get actual images
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, swapChainImages.data());
}


void EngineVulkan::CreateImageViews()
{
    //Initialize array size
    swapChainImageViews.resize(swapChainImages.size());

    //Loop through each image
    for (size_t i = 0; i < swapChainImages.size(); i++)
    {
        VkImageViewCreateInfo cInfo{};
        cInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        cInfo.image = swapChainImages[i];
        cInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;     //Here, you specify how to interpret, i.e. 1D, 2D, 3D texture
        cInfo.format = swapChainImageFormat;

        //Swizzle color channels, here, you might give every component the same value as the red channel for grayscale
        cInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;     
        cInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        cInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        cInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        //Subresource lets us define what image is for, parts of image we need
        cInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;  
        cInfo.subresourceRange.baseMipLevel = 0;
        cInfo.subresourceRange.levelCount = 1;
        cInfo.subresourceRange.baseArrayLayer = 0;
        cInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(logicalDevice, &cInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
        {
            gpr460::engine->system->ErrorMessage(gpr460::ERROR_CREATE_IMAGE_VIEW_FAILED);
            gpr460::engine->system->LogToErrorFile(gpr460::ERROR_CREATE_IMAGE_VIEW_FAILED);
            throw std::runtime_error("Failed to create Image Views");
        }
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

    std::cout << "Layer Properties:\n";
    for (const auto& layerProperties : availableLayers)
    {
        std::cout << "\t" << layerProperties.layerName << std::endl;
    }

    std::cout << "Desired Validation Layers:\n";
    //Loop through and check if all layers exist
    for (const char* layerName : validationLayers)
    {
        std::cout << "\t" << layerName << std::endl;

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

    if (!indices.isComplete())
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_QUEUE_FAMILY_INCOMPLETE);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_QUEUE_FAMILY_INCOMPLETE);
    }

    //Check if extensions are supported
    bool extensionSupported = CheckDeviceExtensionSupport(device);

    //Check that swap chain returns values in formats and presentModes
    bool swapChainCapable = false;
    if (extensionSupported)
    {
        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
        swapChainCapable = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }
    else
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_EXTENSIONS_UNSUPPORTED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_EXTENSIONS_UNSUPPORTED);
    }

    if (!swapChainCapable)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_SWAP_CHAIN_INADEQUATE);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_SWAP_CHAIN_INADEQUATE);
    }

    //Return true if physical device is a discrete (non-integrated) GPU and has geometry shader feature
    return indices.isComplete() && extensionSupported && swapChainCapable;
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

    if (requiredExtensions.empty())
    {
        std::cout << "All required extensions accounted for\n";
    }
    else
    {
        std::cout << "Missing Extensions:\n";
        for (const auto& extension : requiredExtensions)
        {
            std::cout << "\t" << extension << std::endl;
        }
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

    if (!indices.graphicsFamily.has_value())
    {
        std::cout << "Missing graphics family queue\n";
    }
    if (!indices.presentFamily.has_value())
    {
        std::cout << "Missing present family queue\n";
    }

    return indices;
}


SwapChainSupportDetails EngineVulkan::QuerySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;

    //Gets capabilities of physical device
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    //Check number of formats
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    //Get actual formats from GPU
    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    //Get number of present modes
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    //Populate vector of present modes given the count
    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}


VkSurfaceFormatKHR EngineVulkan::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    //Find first format that meets desired parameters for format and color space
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    //If none meet preferences, just take first format
    return availableFormats[0];
}


VkPresentModeKHR EngineVulkan::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    //Check if tripler buffering (VK_PRESENT_MODE_MAILBOX_KHR) is available
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }

    //Guaranteed to be available
    return VK_PRESENT_MODE_FIFO_KHR;
}


VkExtent2D EngineVulkan::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, SDL_Window* window)
{
    //If width is at max value, this means Vulkan is allowing us to set the width and height in pixels
    if (capabilities.currentExtent.width != UINT32_MAX)
    {
        return capabilities.currentExtent;
    }
    else
    {
        //Get actual size of window in pixels from SDL
        int width, height;
        SDL_GetWindowSize(window, &width, &height);

        VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

        //Clamp width and height between the minimum and maximum values supported by the GPU
        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.width = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}