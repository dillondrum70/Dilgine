#include "System.h"

void runMainLoop(gpr460::EngineState* engine);
void frameStep(void* arg);
Uint32 GetTicks();

int main(int argc, char* argv[])
{
    const int WIDTH = 640;
    const int HEIGHT = 480;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    gpr460::System* system = gpr460::System::Create();
    system->Init();

    int* leak = DBG_NEW int[4096];

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("SDL2 Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //declare and initialize world
    World* world = DBG_NEW World();
    world->Init(window);

    //EngineState engine;
    gpr460::System::engine.quit = false;
    gpr460::System::engine.renderer = renderer;
    gpr460::System::engine.frame = 0;
    gpr460::System::engine.frameStart = GetTicks();
    gpr460::System::engine.system = system;
    gpr460::System::engine.world = world;

    runMainLoop(&gpr460::System::engine);

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

    return 0;
}

void MainLoopProcesses(void* engine)
{
    Uint32 now = GetTicks();
    if (now - ((gpr460::EngineState*)engine)->frameStart >= 16)
    {
        frameStep(engine);
    }
}

void runMainLoop(gpr460::EngineState* engine)
{
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(MainLoopProcesses, engine, 0, true);
#else
    while (!engine->quit)
    {
        /*Uint32 now = GetTicks();
        if (now - engine->frameStart >= 16)
        {
            frameStep(engine);
        }*/
        MainLoopProcesses(engine);
    }
#endif
}

void frameStep(void* arg)
{
    gpr460::EngineState* engine = (gpr460::EngineState*)arg;
    SDL_Event event;

    Uint32 now = GetTicks();

    engine->frame++;
    engine->frameStart = now;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            engine->quit = true;
        }

        if (event.type == SDL_KEYDOWN)
        {
            std::cout << "Key pressed!\n";
            if (event.key.keysym.sym == SDLK_k)
            {
                std::cout << "K pressed!\n";

                // TODO: Add calls to ErrorMessage and LogToErrorFile here
                engine->system->ErrorMessage(gpr460::ERROR_PRESSED_K);
                engine->system->LogToErrorFile(gpr460::ERROR_PRESSED_K);
                // DONE
            }
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                engine->quit = true;
            }
        }
    }

    engine->world->Update();
    //int x = (SDL_sinf(engine->frame / 100.0f) * 100.0f) + 200;

    /*SDL_Rect r = {
        x,
        100,
        50,
        50
    };*/

    SDL_SetRenderDrawColor(engine->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

    SDL_RenderClear(engine->renderer);

    engine->world->Render();
    //SDL_SetRenderDrawColor(engine->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    //SDL_RenderFillRect(engine->renderer, &r);
    SDL_RenderPresent(engine->renderer);
}

Uint32 GetTicks()
{
    return SDL_GetTicks();
}