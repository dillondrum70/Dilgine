#include "VulkanObject.h"

#include "System.h"

void VulkanObject::CreateObject(std::string modelFilePath, std::string textureFilePath)
{
    EngineVulkan& vulkan = gpr460::engine->vulkanEngine;

    textureImage = &vulkan.textureImages[textureFilePath];
    textureImageMemory = &vulkan.textureImageMemorys[textureFilePath];
    textureImageView = &vulkan.textureImageViews[textureFilePath];
    textureSampler = &vulkan.textureSamplers[textureFilePath];

    vertices = &vulkan.models[modelFilePath];
    indices = &vulkan.modelIndices[modelFilePath];
    vertexBuffer = &vulkan.vertexBuffers[modelFilePath];
    vertexBufferMemory = &vulkan.vertexBufferMemorys[modelFilePath];
    indexBuffer = &vulkan.indexBuffers[modelFilePath];
    indexBufferMemory = &vulkan.indexBufferMemorys[modelFilePath];
    
    CreateDescriptorSetLayout();	//Binding model-view projection matrix
    CreateGraphicsPipeline();	//Handles rendering steps like vertex, geometry, and fragment shaders
    CreateUniformBuffer();	//Create all uniform buffers, i.e. model-view projection matrix buffer
    CreateDescriptorPool();	//Pool that descriptor sets are allocated to
    CreateDescriptorSets();	//Set of descriptors for a model
}

void VulkanObject::DestroyObject()
{
    EngineVulkan& vulkan = gpr460::engine->vulkanEngine;

    vkDestroyDescriptorPool(vulkan.logicalDevice, descriptorPool, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroyBuffer(vulkan.logicalDevice, uniformBuffers[i], nullptr);
        vkFreeMemory(vulkan.logicalDevice, uniformBuffersMemory[i], nullptr);
    }

    vkDestroyPipeline(vulkan.logicalDevice, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(vulkan.logicalDevice, pipelineLayout, nullptr);

    vkDestroyDescriptorSetLayout(vulkan.logicalDevice, descriptorSetLayout, nullptr);
}

void VulkanObject::CreateDescriptorSetLayout()
{
    EngineVulkan& vulkan = gpr460::engine->vulkanEngine;

    //Layout binding for model-view projection
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;    //Descriptor is a uniform buffer
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;   //Descriptor is used during vertex shader stage
    uboLayoutBinding.pImmutableSamplers = nullptr;  //Used for descriptors related to image sampling

    //Layout binding for texture sampler
    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    //Store all layout bindings
    std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
    //Info to create and bind descriptor layout
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    //Create layout
    if (vkCreateDescriptorSetLayout(vulkan.logicalDevice, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_CREATE_DESCRIPTOR_SET_LAYOUT_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_CREATE_DESCRIPTOR_SET_LAYOUT_FAILED);
        throw std::runtime_error("Failed to create descriptor set layout");
    }
}

void VulkanObject::CreateGraphicsPipeline()
{
    EngineVulkan& vulkan = gpr460::engine->vulkanEngine;

    //Read bytecode data in from shader files
    auto vertShaderCode = vulkan.ReadFile("shaders/Vertex/vert.spv");
    auto fragShaderCode = vulkan.ReadFile("shaders/Fragment/frag.spv");

    //Create shader modules from bytecode
    VkShaderModule vertShaderModule = vulkan.CreateShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = vulkan.CreateShaderModule(fragShaderCode);

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
    viewport.width = (float)vulkan.swapChainExtent.width;
    viewport.height = (float)vulkan.swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    //Region of the framebuffer to rasterize
    VkRect2D scissor{};
    scissor.offset = { 0,0 };
    scissor.extent = vulkan.swapChainExtent;

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
    //Vertices defined counter-clockwise from our current viewpoint are front-facing
    //NOTE: We must define our vertices in a clockwise order because we then flip them around in the projection matrix due to glm
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
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

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;     //Do we test new fragments against depth buffer?
    depthStencil.depthWriteEnable = VK_TRUE;    //Do we write fragments that pass the depth buffer?
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;   //Lower depth = closer, depth of new fragments should be less
    //If enabled, allows you to keep only fragments that are within depth range
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f;
    depthStencil.maxDepthBounds = 1.0f;
    //If enabled, allows you to configure stencil buffer operations
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {};
    depthStencil.back = {};

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
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;  //Descriptor Set Layout for Model-View Projection matrix
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    //Create pipeline layout
    if (vkCreatePipelineLayout(vulkan.logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
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
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = vulkan.renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    //We can pass multiple pipeline infos and create multiple in one call if we want to
    if (vkCreateGraphicsPipelines(vulkan.logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_CREATE_PIPELINE_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_CREATE_PIPELINE_FAILED);
        throw std::runtime_error("Failed to create graphics pipeline");
    }

    //Destroy shader modules at end of function, no longer needed
    vkDestroyShaderModule(vulkan.logicalDevice, fragShaderModule, nullptr);
    vkDestroyShaderModule(vulkan.logicalDevice, vertShaderModule, nullptr);
}

void VulkanObject::CreateUniformBuffer()
{
    EngineVulkan& vulkan = gpr460::engine->vulkanEngine;

    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    //Resize arrays, 1 for each frame in flight
    //uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    //uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    //uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        //Create uniform buffer
        vulkan.CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            uniformBuffers[i], uniformBuffersMemory[i]);

        //Map the memory, this is a pointer we can write data to later
        //We don't want to map memory every time we update the memory because that takes time
        vkMapMemory(vulkan.logicalDevice, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }
}

void VulkanObject::CreateDescriptorPool()
{
    EngineVulkan& vulkan = gpr460::engine->vulkanEngine;

    //Size of descriptor pool
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    //Creation info for descriptor pool
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT); //Max number of descriptor sets is number of frames we have

    if (vkCreateDescriptorPool(vulkan.logicalDevice, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_CREATE_DESCRIPTOR_POOL_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_CREATE_DESCRIPTOR_POOL_FAILED);
        throw std::runtime_error("Faild to create descriptor pool");
    }
}

void VulkanObject::CreateDescriptorSets()
{
    EngineVulkan& vulkan = gpr460::engine->vulkanEngine;

    //Allocation info for descriptor sets
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    //Allocate space for descriptor sets
    descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(vulkan.logicalDevice, &allocInfo, descriptorSets.data()) != VK_SUCCESS)
    {
        gpr460::engine->system->ErrorMessage(gpr460::ERROR_ALLOCATE_DESCRIPTOR_SETS_FAILED);
        gpr460::engine->system->LogToErrorFile(gpr460::ERROR_ALLOCATE_DESCRIPTOR_SETS_FAILED);
        throw std::runtime_error("Failed to allocate descriptor sets!");
    }

    //For each descriptor set, populate the descriptor
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        //Info for descriptor set buffer
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = *textureImageView;
        imageInfo.sampler = *textureSampler;

        //Need to update descriptor sets,
        std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        //Specify which descriptor set and its binding are destination
        descriptorWrites[0].dstSet = descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;    //How many array elements to update
        descriptorWrites[0].pBufferInfo = &bufferInfo;
        descriptorWrites[0].pImageInfo = nullptr;
        descriptorWrites[0].pTexelBufferView = nullptr;

        //Texture sampler descriptor writes
        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = descriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;    //How many array elements to update
        descriptorWrites[1].pBufferInfo = nullptr;
        descriptorWrites[1].pImageInfo = &imageInfo;
        descriptorWrites[1].pTexelBufferView = nullptr;

        //Apply the actual updates
        vkUpdateDescriptorSets(vulkan.logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}