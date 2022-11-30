#include "Input.h"

#include "System.h"

void Input::UpdateInput()
{
    keyboardState = SDL_GetKeyboardState(NULL);

    Uint8 mouse = SDL_GetMouseState(&mouseState.mouseX, &mouseState.mouseY);

    mouseState.leftMouse = mouse & SDL_BUTTON_LMASK;
    mouseState.rightMouse = mouse & SDL_BUTTON_RMASK;

	SDL_Event event;

    bool wheel = false;
    bool motion = false;

    while (SDL_PollEvent(&event))
    {
        /*if (event.type == SDL_QUIT)
        {
            quit = true;
        }

        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                quit = true;
            }
        }*/
        if (event.type == SDL_QUIT)
        {
            gpr460::engine->quit = true;
        }

        //Click and drag cursor to move camera for testing
        if (event.type == SDL_MOUSEMOTION)
        {
            motion = true;
            mouseState.mouseXChange = event.motion.xrel;
            mouseState.mouseYChange = event.motion.yrel;

            mouseState.mouseX = event.motion.x;
            mouseState.mouseY = event.motion.y;
        }

        //Scroll mouse wheel to zoom camera for testing
        if (event.type == SDL_MOUSEWHEEL)
        {
            wheel = true;
            mouseState.mouseWheelXChange = event.wheel.preciseX;
            mouseState.mouseWheelYChange = event.wheel.preciseY;
        }
    }

    if (!motion)
    {
        mouseState.mouseXChange = event.motion.xrel;
        mouseState.mouseYChange = event.motion.yrel;
    }

    //Reset wheel change to 0 if it was not changed in loop
    if (!wheel)
    {
        mouseState.mouseWheelXChange = 0.0f;
        mouseState.mouseWheelYChange = 0.0f;
    }
}