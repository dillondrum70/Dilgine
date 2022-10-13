#include "System.h"

void runMainLoop();
void frameStep();
Uint32 GetTicks();

gpr460::EngineState* gpr460::engine;

int main(int argc, char* argv[])
{
    gpr460::engine = DBG_NEW gpr460::EngineState();

    const int WIDTH = 640;
    const int HEIGHT = 480;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    gpr460::System* system = gpr460::System::Create();
    system->Init();

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("SDL2 Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int* leak = DBG_NEW int[4096];

    if (!renderer)
    {
        system->ErrorMessage(gpr460::ERROR_CREATE_SDL_RENDERER_FAILED);
        system->LogToErrorFile(gpr460::ERROR_CREATE_SDL_RENDERER_FAILED);
        return 0;
    }

    //declare and initialize world
    World* world = DBG_NEW World();

    //EngineState engine;
    gpr460::engine->quit = false;
    gpr460::engine->renderer = renderer;
    gpr460::engine->frame = 0;
    gpr460::engine->frameStart = GetTicks();
    gpr460::engine->system = system;
    gpr460::engine->world = world;

    world->Init(window);

    runMainLoop();

    //delete and cleanup world first
    world->CleanUp();
    if(world)
        delete world;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    system->Shutdown();
    if (system)
    {
        delete system;
    }

    delete gpr460::engine;

    return 0;
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
        /*Uint32 now = GetTicks();
        if (now - engine->frameStart >= 16)
        {
            frameStep(engine);
        }*/
        MainLoopProcesses();
    }
#endif
}

void frameStep()
{
    //gpr460::EngineState* engine = (gpr460::EngineState*)arg;
    SDL_Event event;

    Uint32 now = GetTicks();

    gpr460::engine->frame++;
    gpr460::engine->frameStart = now;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            gpr460::engine->quit = true;
        }

        if (event.type == SDL_KEYDOWN)
        {
            std::cout << "Key pressed!\n";
            if (event.key.keysym.sym == SDLK_k)
            {
                std::cout << "K pressed!\n";

                // TODO: Add calls to ErrorMessage and LogToErrorFile here
                gpr460::engine->system->ErrorMessage(gpr460::ERROR_PRESSED_K);
                gpr460::engine->system->LogToErrorFile(gpr460::ERROR_PRESSED_K);
                // DONE
            }
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                gpr460::engine->quit = true;
            }
        }
    }

    gpr460::engine->world->Update();
    //int x = (SDL_sinf(engine->frame / 100.0f) * 100.0f) + 200;

    /*SDL_Rect r = {
        x,
        100,
        50,
        50
    };*/

    

    gpr460::engine->world->Render(gpr460::engine->renderer);
    //SDL_SetRenderDrawColor(engine->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    //SDL_RenderFillRect(engine->renderer, &r);
    
}

Uint32 GetTicks()
{
    return SDL_GetTicks();
}