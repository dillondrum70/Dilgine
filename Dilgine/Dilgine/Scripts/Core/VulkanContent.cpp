#include "VulkanContent.h"

#include "System.h"

#include <set>
#include <cstdint>
#include <limits>
#include <algorithm>

//Remember to always destroy in the reverse order things are created in case some things have dependencies on subsequently created systems
//(That is the case for many of these structures)
void EngineVulkan::Cleanup()
{
    CleanupSwapChain();

    vkDestroyBuffer(logicalDevice, indexBuffer, nullptr);
    vkFreeMemory(logicalDevice, indexBufferMemory, nullptr);

    vkDestroyBuffer(logicalDevice, vertexBuffer, nullptr);
    vkFreeMemory(logicalDevice, vertexBufferMemory, nullptr);

    vkDestroyPipeline(logicalDevice, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
    vkDestroyRenderPass(logicalDevice, renderPass, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(logicalDevice, imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(logicalDevice, renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(logicalDevice, inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(logicalDevice, commandPool, nullptr);

    vkDestroyDevice(logicalDevice, nullptr);

    if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(vInstance, debugMessenger, nullptr);
    }
    
    vkDestroySurfaceKHR(vInstance, surface, nullptr);
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
    
    std::cout << "Initializing Render Pass...\n";
    CreateRenderPass();
    std::cout << "Render Pass Initialized...\n\n";

    std::cout << "Initializing Graphics Pipeline...\n";
    CreateGraphicsPipeline();
    std::cout << "Graphics Pipeline Initialized...\n\n";

    std::cout << "Initializing Frame Buffers...\n";
    CreateFramebuffers();
    std::cout << "Frame Buffers Initialized...\n\n";

    std::cout << "Initializing Command Pool...\n";
    CreateCommandPool();
    std::cout << "Command Pool Initialized...\n\n";

    std::cout << "Initializing Vertex Buffer...\n";
    CreateVertexBuffer();
    std::cout << "Vertex Buffer Initialized...\n\n";

    std::cout << "Initializing Index Buffer...\n";
    CreateIndexBuffer();
    std::cout << "Index Buffer Initialized...\n\n";

    std::cout << "Initializing Command Buffer...\n";
    CreateCommandBuffers();
    std::cout << "Command Buffer Initialized...\n\n";

    std::cout << "Initializing Sync Objects...\n";
    CreateSyncObjects();
    std::cout << "Sync Objects Initialized...\n\n";

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
    //std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        std::cout << "validation layer: " << pCallbackData->pMessage << std::endl;
    }

    return VK_FALSE;
}


void EngineVulkan::CreateDebugMessenger()
{
    if (!enableValidationLayers) return;

    //Create info for debug messenger, specify callback function that writes to console
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    PopulateDebugMessengerCreateInfo(createInfo);

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


void EngineVulkan::CreateRenderPass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChainImageFormat;      //Format should match swap chain
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;    //Not multisampling
    //Determines how data is loaded
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;   //Existing contents of attachment of preserved
    //Determines how data is stored
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; //Rendering contents stored in memory and read later
    //Don't care about stencil load and store, we don't have a stencil buffer
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;  //Images are presented in swap chain

    //Referenced by a subpass
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    //A subpass relies on input from last subpass to alter rendering somehow, we just do one
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(logicalDevice, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_CREATE_RENDER_PASS_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_CREATE_RENDER_PASS_FAILED);
        throw std::runtime_error("Failed to create render pass");
    }

}


void EngineVulkan::CreateGraphicsPipeline()
{
    //Read bytecode data in from shader files
    auto vertShaderCode = ReadFile("shaders/Vertex/vert.spv");
    auto fragShaderCode = ReadFile("shaders/Fragment/frag.spv");

    //Create shader modules from bytecode
    VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

    //Shader Stages tell us which pipeline stage a shader is assigned to

    //Create vertex shader stage info
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT; //Tells us which pipeline stage shader will be used in
    vertShaderStageInfo.module = vertShaderModule;  //Module containing code
    vertShaderStageInfo.pName = "main";     //entry point (like int main() in C++)

    //Create fragment shader stage info
    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT; //Tells us which pipeline stage shader will be used in
    fragShaderStageInfo.module = fragShaderModule;  //Module containing code
    fragShaderStageInfo.pName = "main";     //entry point (like int main() in C++)

    //Array containing all shader stage creation info
    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    //Descriptions for Vertex input
    auto bindingDescription = Vertex::GetBindingDescription();
    auto attributeDescriptions = Vertex::GetAttributeDescriptions();

    //Vertex input state info tells us formtat of vertex data passed to vertex shader
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    //Vertex shader information
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    //Define kind of geometery to be drawn and if primitive restart should be enabled
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;   //List of verticies, every 3 is an individual triangle
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    //Region of framebuffer output is rendered to
    VkViewport viewport{};
    viewport.x = 0.0;
    viewport.y = 0.0;
    viewport.width = (float)swapChainExtent.width;
    viewport.height = (float)swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    //Region of the framebuffer to rasterize
    VkRect2D scissor{};
    scissor.offset = { 0,0 };
    scissor.extent = swapChainExtent;

    //Define viewport state info
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    //Rasterizer - turns geometry into fragments (pixels) that can be fed into fragment shader
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE; //If true, frgaments beyond near and far planes are clamped to them, discarded if false
    rasterizer.rasterizerDiscardEnable = VK_FALSE;  //If true, no geometry passed to rasterizer, framebuffer output essentially disabled
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;  //Polygon area filled with fragments (can be drawn as points or lines but need to enable GPU feature)
    rasterizer.lineWidth = 1.0f;    //Number of fragments makes line thicker
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT; //cull back faces
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE; //Vertices defined clockwise from our current viewpoint are front-facing
    rasterizer.depthBiasEnable = VK_FALSE;      //If true, you can modify depth values
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    //Multisampling, used in anti-aliasing to blend colors from multiple polygons into one pixel, requires enabling GPU feature
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    //Color blending - blend fragment shader pixel color with current pixel color, must be passed to blend state
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | 
        VK_COLOR_COMPONENT_A_BIT;   //Blend all color channels
    //Enable blending
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    //Dynamic states allow us to change certain properties without recreating the pipeline
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    //Info for creating pipeline layout
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    //Create pipeline layout
    if (vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_CREATE_PIPELINE_LAYOUT_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_CREATE_PIPELINE_LAYOUT_FAILED);
        throw std::runtime_error("Failed to create pipeline layout");
    }

    //Pass in all info structs and other objects into graphics pipeline info
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr;          //Not using stencil buffers
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    //We can pass multiple pipeline infos and create multiple in one call if we want to
    if (vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_CREATE_PIPELINE_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_CREATE_PIPELINE_FAILED);
        throw std::runtime_error("Failed to create graphics pipeline");
    }

    //Destroy shader modules at end of function, no longer needed
    vkDestroyShaderModule(logicalDevice, fragShaderModule, nullptr);
    vkDestroyShaderModule(logicalDevice, vertShaderModule, nullptr);
}


void EngineVulkan::CreateFramebuffers()
{
    //Initialized frame buffer size
    swapChainFramebuffers.resize(swapChainImageViews.size());

    //Create frame buffer from each ImageView
    for (size_t i = 0; i < swapChainImageViews.size(); i++)
    {
        //Create array of attachments for swapchain index
        VkImageView attachments[] = {
            swapChainImageViews[i]
        };

        //Frame buffer creation info
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        //Create frame buffer
        if (vkCreateFramebuffer(logicalDevice, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
        {
            gpr460::engine->system->ErrorMessage(gpr460::ERROR_CREATE_FRAMEBUFFER_FAILED);
            gpr460::engine->system->LogToErrorFile(gpr460::ERROR_CREATE_FRAMEBUFFER_FAILED);
            throw std::runtime_error("Failed to create framebuffers");
        }
    }
}


void EngineVulkan::CreateCommandPool()
{
    QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(physicalDevice);

    //Information needed to create command pool
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;   //Command buffers can be reordered individually, otherwise all need to be reset
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_CREATE_FRAMEBUFFER_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_CREATE_FRAMEBUFFER_FAILED);
        throw std::runtime_error("Failed to create command pool");
    }
}


void EngineVulkan::CreateVertexBuffer()
{
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    //Create memory buffer to copy vertex data into, this buffer will be copied into vertex buffer, hence the TRANSER_SRC_BIT
    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    //Access a portion of memory, copies vertex data to mapped memory, then unmaps buffer memory
    void* data = nullptr;
    vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(logicalDevice, stagingBufferMemory);

    //Can be used as destination buffer in memory transfer
    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

    //Copy staging buffer to vertex buffer
    CopyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    //Free memory
    vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
}


void EngineVulkan::CreateIndexBuffer()
{
    //Byte size of buffer
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    //Construct and allocate staging buffer just like you would when creating vertex buffer
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    //Copy index data into staging buffer
    void* data;
    vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(logicalDevice, stagingBufferMemory);

    //Create index buffer
    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    //Copy staging buffer into index buffer
    CopyBuffer(stagingBuffer, indexBuffer, bufferSize);

    //Free staging memory
    vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
}


void EngineVulkan::CreateCommandBuffers()
{
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    //Creation info for allocation of command buffer
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;  //Operations submitted to queue, can't be called from other command buffer
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

    if (vkAllocateCommandBuffers(logicalDevice, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_ALLOCATE_COMMAND_BUFFER_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_ALLOCATE_COMMAND_BUFFER_FAILED);
        throw std::runtime_error("Failed to allocate command buffers");
    }
}


void EngineVulkan::CreateSyncObjects()
{
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    //Semaphore creation info
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    //Fence creation info
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;     //Start signaled so it does not wait on first frame, no previous frames to wait on

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
        {
            gpr460::engine->system->ErrorMessage(gpr460::ERROR_CREATE_SEMAPHORES_AND_FENCES_FAILED);
            gpr460::engine->system->LogToErrorFile(gpr460::ERROR_CREATE_SEMAPHORES_AND_FENCES_FAILED);
            throw std::runtime_error("Failed to create semaphores and fences");
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


std::vector<char> EngineVulkan::ReadFile(const std::string& filename)
{
    //Start at end of file (we can allocate a buffer this way, read as binary, no text transformations
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_OPEN_FILE_VULKAN_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_OPEN_FILE_VULKAN_FAILED);
        throw std::runtime_error("Failed to open file for Vulkan");
    }

    size_t fileSize = (size_t)file.tellg(); //Get position of input sequence
    std::vector<char> buffer(fileSize);

    file.seekg(0);  //Set position of input sequence
    file.read(buffer.data(), fileSize); //Read all bytes at once into buffer

    file.close();

    return buffer;
}


VkShaderModule EngineVulkan::CreateShaderModule(const std::vector<char>& code)
{
    //Set up info necessary to create shader module (i.e. the code)
    VkShaderModuleCreateInfo cInfo{};
    cInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    cInfo.codeSize = code.size();
    cInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());   //uint32_t data must be aligned, but vectors take care of that

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(logicalDevice, &cInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_CREATE_SHADER_MODULE_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_CREATE_SHADER_MODULE_FAILED);
        throw std::runtime_error("Failed to create shader module");
    }

    return shaderModule;
}


void EngineVulkan::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
    //Specific details about command buffer's usage
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_BEGIN_COMMAND_BUFFER_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_BEGIN_COMMAND_BUFFER_FAILED);
        throw std::runtime_error("Failed to begin recording command buffer");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex]; //Bind swapchain
    //Size of render area
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = swapChainExtent;
    //Define values used for clearing screen (clear to black)
    VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    //Begin the render pass, INLINE means commands will be inlined into primary command buffer, no secondary command buffers executed
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    //Bind pipeline to command buffer
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    //Set viewport
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapChainExtent.width);
    viewport.height = static_cast<float>(swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    //Set scissor
    VkRect2D scissor{};
    scissor.offset = { 0,0 };
    scissor.extent = swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    //Bind vertex buffers to command buffer
    VkBuffer vertexBuffers[] = { vertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    //Bind index buffer to command buffer
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

    //Param1 = Command Buffer
    //Param2 = vertex count
    //Param3 = instance count
    //Param4 = first vertex
    //Param5 = first instance
    //Execute command buffer operations
    //vkCmdDraw(commandBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);

    //Param1 = Command Buffer, command buffer into which command is recorded
    //Param2 = index count, number of vertices to draw
    //Param3 = instance count, number of instances to draw
    //Param4 = first vertex, base index within index buffer
    //Param5 = vertex offset, added to vertex index before indexing into vertex buffer
    //Param6 = first instance, ID of first instance to draw
    //Execute command buffer operations with index array
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    //End render pass
    vkCmdEndRenderPass(commandBuffer);

    //Check if command buffer ended properly
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_RECORD_COMMAND_BUFFER_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_RECORD_COMMAND_BUFFER_FAILED);
        throw std::runtime_error("Failed to record command buffer");
    }
}


void EngineVulkan::CleanupSwapChain()
{
    //Destroy frame buffers first
    for (size_t i = 0; i < swapChainFramebuffers.size(); i++)
    {
        vkDestroyFramebuffer(logicalDevice, swapChainFramebuffers[i], nullptr);
    }

    //Destroy image views (frame buffers depend on these and therefore must be destroyed first)
    for (size_t i = 0; i < swapChainImageViews.size(); i++)
    {
        vkDestroyImageView(logicalDevice, swapChainImageViews[i], nullptr);
    }

    //Destroy swapchain last
    vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
}


void EngineVulkan::RecreateSwapChain(SDL_Window* window)
{
    //Check minimized, lock Vulkan in a loop if extents are 0 that way it can't attempt a draw and error
    int width = 0, height = 0;
    SDL_Vulkan_GetDrawableSize(window, &width, &height);
    while (width == 0 || height == 0)
    {
        //Check extents
        SDL_Vulkan_GetDrawableSize(window, &width, &height);

        //Wait for an event to check next, don't need to waste processing power on infinite loop
        SDL_Event sdlEvent;
        SDL_WaitEvent(&sdlEvent);   //Waiting here allows window to be un-minimized, if we didn't wait, it would update too quickly and minimize the window when un-minimized
    }

    //Wait until logical device is idle so we don't recreate swap chain in the middle of a process
    vkDeviceWaitIdle(logicalDevice);

    //Destroy current swapchain
    CleanupSwapChain();

    //Recreate swapchain and members dependent on it
    CreateSwapChain(window);
    CreateImageViews();
    CreateFramebuffers();
}


int EngineVulkan::FramebufferResizeCallback(void* data, SDL_Event* sdlEvent)
{
    //check if event was a window resized event
    if (sdlEvent->type == SDL_WINDOWEVENT &&
        sdlEvent->window.event == SDL_WINDOWEVENT_RESIZED)
    {
        SDL_Window* win = SDL_GetWindowFromID(sdlEvent->window.windowID);
        if (win == (SDL_Window*)data)
        {
            gpr460::engine->vulkanEngine.framebufferResized = true;
        }
    }

    return 0;
}


uint32_t EngineVulkan::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    //Get memory properties of the GPU
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    //Iterate of bitmask filter of memory types and determine if corresponding bit is set
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        //Check if bit is set and it is not only non-zero, but matches desired properties
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    gpr460::engine->system->ErrorMessage(gpr460::ERROR_FIND_MEMORY_TYPE_FAILED);
    gpr460::engine->system->LogToErrorFile(gpr460::ERROR_FIND_MEMORY_TYPE_FAILED);
    throw std::runtime_error("Failed to find suitable memory type");
}


void EngineVulkan::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; //Buffer is owned only by graphics queue

    if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_CREATE_BUFFER_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_CREATE_BUFFER_FAILED);
        throw std::runtime_error("Failed to create buffer");
    }

    VkMemoryRequirements memRequirements;	//Specifies how memory should be allocated
    vkGetBufferMemoryRequirements(logicalDevice, buffer, &memRequirements);

    //Information about allocating 
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;    //Size of memory to allocate
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);    //Type of memory to allocate

    //Allocate vertex buffer memory
    if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_ALLOCATE_BUFFER_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_ALLOCATE_BUFFER_FAILED);
        throw std::runtime_error("Failed to allocate buffer memory");
    }

    //Bind vertex buffer to its memory
    vkBindBufferMemory(logicalDevice, buffer, bufferMemory, 0);
}


void EngineVulkan::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;   //Memory transfers are conducted with command buffers
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    
    //Allocate space for command buffer
    //NOTE: In larger productions, multiple objects should be made in a single allocation by using offsets to differentiate
    //This is because many GPUs have a set limit on the number of allocations they have which can be low, around 4096 on the NVIDIA GTX 1080
    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    //Begin writing to command buffer
    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    //Copy buffer to copy information from source buffer to destination buffer
    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    //Stop writing to command buffer
    vkEndCommandBuffer(commandBuffer);

    //Information to submit command buffer to graphics queue
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    //Submit command buffer to graphics queue then wait for the queue to finish (idle)
    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    //Free memory
    vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);
}


