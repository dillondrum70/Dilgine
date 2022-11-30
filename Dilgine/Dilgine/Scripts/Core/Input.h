#ifndef INPUT_H
#define INPUT_H

#include "SDL2/SDL.h"

struct MouseState
{
	float mouseWheelXChange = 0.0f;
	float mouseWheelYChange = 0.0f;

	int mouseX = 0;
	int mouseY = 0;

	int mouseXChange = 0;
	int mouseYChange = 0;

	bool leftMouse = false;
	bool rightMouse = false;
};

//Inputs are all recorded before Update
class Input
{
private:
	const Uint8* keyboardState;

	MouseState mouseState;

public:
	void UpdateInput();

	bool GetKeyDown(SDL_Scancode code) { return keyboardState[code]; }
	const Uint8* GetKeyboardState() { return keyboardState; }
	MouseState GetMouseState() { return mouseState; }
};

#endif
