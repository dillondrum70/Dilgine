#include "System.h"
#include "VulkanContent.h"

//Vulkan - Low Level Graphics Backend
//SDL - Higher level, used mainly to talk with Windows and create a window
//GLSL - Shader language used
//GLM - Data types that align with those used in GLSL so things like matrices can be created and passed to GLSL
//STB - Image manipulation, used for textures

void Run();
void runMainLoop();
void frameStep();
Uint32 GetTicks();

gpr460::EngineState* gpr460::engine;

int main(int argc, char* argv[])
{
    try
    {
        Run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}

void Run()
{
    gpr460::engine = DBG_NEW gpr460::EngineState();

    const int WIDTH = 640;
    const int HEIGHT = 480;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    gpr460::System* system = gpr460::System::Create();
    system->Init();

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Dilgine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer)
    {
        system->ErrorMessage(gpr460::ERROR_CREATE_SDL_RENDERER_FAILED);
        system->LogToErrorFile(gpr460::ERROR_CREATE_SDL_RENDERER_FAILED);
        return;
    }

    //int* leak = DBG_NEW int[4096];
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    
    std::cout << "Available Extensions:\n";
    for (const auto& extension : extensions) 
    {
        std::cout << "\t" << extension.extensionName << std::endl;
    }

    //declare and initialize world
    World* world = DBG_NEW World();

    //EngineState engine;
    gpr460::engine->quit = false;
    gpr460::engine->renderer = renderer;
    //vulkan instance set in VulkanInit
    gpr460::engine->frame = 0;
    gpr460::engine->frameStart = GetTicks();
    gpr460::engine->system = system;
    gpr460::engine->world = world;

    gpr460::engine->vulkanEngine.InitVulkan(window);
    SDL_AddEventWatch(gpr460::engine->vulkanEngine.FramebufferResizeCallback, window);  //Add framebuffer resize event, will tell vulkan when SDL_Window resized

    world->Init(window);
    
    system->TrackMemory();  //Start memory checkpoint
    runMainLoop();
    system->LogMemory();    //Check memory leaks from checkpoint

    //delete and cleanup world first
    world->CleanUp();
    if (world)
        delete world;

    gpr460::engine->vulkanEngine.Cleanup();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    system->Shutdown();
    if (system)
    {
        delete system;
    }

    delete gpr460::engine;
}

void MainLoopProcesses()
{
    Uint32 now = GetTicks();
    if (now - gpr460::engine->frameStart >= 16)
    {
        frameStep();
    }
}

void runMainLoop()
{
#ifdef __EMSCRIPTEN__
    //emscripten_set_main_loop_arg(MainLoopProcesses, engine, 0, true);
    emscripten_set_main_loop(MainLoopProcesses, 0, true);
#else
    while (!gpr460::engine->quit)
    {
        MainLoopProcesses();
    }
#endif

    vkDeviceWaitIdle(gpr460::engine->vulkanEngine.logicalDevice);
}

void frameStep()
{
    //gpr460::EngineState* engine = (gpr460::EngineState*)arg;
    //SDL_Event event;

    Uint32 now = GetTicks();

    gpr460::engine->frame++;
    gpr460::engine->frameStart = now;

    //while (SDL_PollEvent(&event))
    //{
    //    if (event.type == SDL_QUIT)
    //    {
    //        gpr460::engine->quit = true;
    //    }

    //    if (event.type == SDL_KEYDOWN)
    //    {
    //        if (event.key.keysym.sym == SDLK_ESCAPE)
    //        {
    //            gpr460::engine->quit = true;
    //        }
    //    }

    //    //Click and drag cursor to move camera for testing
    //    if (event.type == SDL_MOUSEMOTION && event.motion.state && SDL_BUTTON_LMASK)
    //    {
    //        gpr460::engine->world->mouseX -= event.motion.xrel;
    //        gpr460::engine->world->mouseY += event.motion.yrel;
    //    }

    //    //Scroll mouse wheel to zoom camera for testing
    //    if (event.type == SDL_MOUSEWHEEL)
    //    {
    //        gpr460::engine->world->zoom -= event.wheel.preciseY;
    //    }
    //}
    gpr460::engine->input.UpdateInput();

    if (gpr460::engine->input.GetKeyDown(SDL_SCANCODE_ESCAPE))
    {
        gpr460::engine->quit = true;
    }

    gpr460::engine->world->Update();

    gpr460::engine->world->Render(gpr460::engine->renderer);
}

Uint32 GetTicks()
{
    return SDL_GetTicks();
}