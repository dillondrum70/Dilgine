#include "Camera.h"
#include "GameObject.h"

#include "SDL2/SDL.h"
#include "System.h"

Camera::Camera(float vMoveSpeed, float vRotateSpeed, float vZoom, Vector3 lookAt, Vector3 eye, GameObject* vGameObject)
{
	moveSpeed = vMoveSpeed; 
	rotateSpeed = vRotateSpeed; 
	zoom = vZoom; 
	lookAtPosition = lookAt; 
	eyePosition = eye; 
	gameObject = vGameObject;

	//Get the current mouse drage values from the passed eye position, use inverse trig functions to reverse the process of drag values to eye position
	//eye.x = cos(mouseDragX) * cos(mouseDragY), eye.z = sin(mouseDragY)
	//It's all algebra from there
	mouseDragY = std::asin(eye.z);
	if (mouseDragY != 0)
	{
		mouseDragX = std::acos(eye.x / std::cos(mouseDragY));
	}
	else
	{
		//If denominator would be zero, just inverse cosine the x
		mouseDragX = std::acos(eye.x);
	}
	
	std::cout << mouseDragY << "  " << mouseDragX << std::endl;
}

//CAMR [moveSpeed rotateSpeed zoom (lookAt.x lookAt.y, lookAt.z) (eye.x, eye.y, eye.z)]
void Camera::Deserialize(GameObject& gameObject, std::istream& stream)
{
	stream.ignore(100, '[');

	float ms = 0, rs = 0, z = 0;
	Vector3 l = Vector3(0.0f), e = Vector3(0.0f);

	stream >> ms;
	stream >> rs;
	stream >> z;

	stream.ignore(100, '(');
	stream >> l.x;
	stream >> l.y;
	stream >> l.z;

	stream.ignore(100, '(');
	stream >> e.x;
	stream >> e.y;
	stream >> e.z;

	//e = e.Normalized();

	gameObject.CreateCamera(gameObject, ms, rs, z, l, e);

	stream.ignore(100, ']');
}

void Camera::Create(GameObject& gameObject, float vMoveSpeed, float vRotateSpeed, float vZoom, Vector3 lookAt, Vector3 eye)
{
	gameObject.CreateCamera(gameObject, vMoveSpeed, vRotateSpeed, vZoom, lookAt, eye);
}

void Camera::Update()
{
	//Zoom
	zoom -= gpr460::engine->input.GetMouseState().mouseWheelYChange;

	MouseState mouseState = gpr460::engine->input.GetMouseState();

	//Orbit the center point, can't move
	if (mouseState.rightMouse)
	{
		orbiting = true;

		//Rotation
		mouseDragX -= mouseState.mouseXChange * rotateSpeed;

		mouseDragY += mouseState.mouseYChange * rotateSpeed;

		//80 is an arbitrary number to limit the speed of the camera's movement when you drag the cursor
		eyePosition = Vector3((std::cos(mouseDragX)) * (std::cos(mouseDragY)), (std::sin(mouseDragX)) * (std::cos(mouseDragY)), (std::sin(mouseDragY)));
	}
	//Move in local space of camera (i.e. pressing W moves in whatever direction the camera is facing
	//and center point rotates around camera (allowing camera to look around while remaining stationary)
	else if (mouseState.leftMouse)
	{
		orbiting = false;

		//Movement
		const Uint8* keyboardState = gpr460::engine->input.GetKeyboardState();

		//WASDQE to move
		Vector3 posInput = Vector3(
			keyboardState[SDL_SCANCODE_A] - keyboardState[SDL_SCANCODE_D],
			keyboardState[SDL_SCANCODE_S] - keyboardState[SDL_SCANCODE_W],
			keyboardState[SDL_SCANCODE_Q] - keyboardState[SDL_SCANCODE_E]
		);

		lookAtPosition += posInput * moveSpeed;
	}
	
}

void Camera::UpdateMain()
{
	Camera* camera = gpr460::engine->world->mainCamera;

	if (camera)
	{
		camera->Update();
	}
}

void Camera::UpdateAll()
{
	Components& components = gpr460::engine->world->GetComponents();

	for (int i = 0; i < gpr460::engine->world->activeCameras; i++)
	{
		Camera* camera = &components.cameraComponents[i];

		if (camera)
		{
			camera->Update();

			////Arrow Keys to move
			//Vector3 posInput = Vector3(
			//	keyboardState[SDL_SCANCODE_LEFT] - keyboardState[SDL_SCANCODE_RIGHT],
			//	keyboardState[SDL_SCANCODE_DOWN] - keyboardState[SDL_SCANCODE_UP],
			//	keyboardState[SDL_SCANCODE_PAGEUP] - keyboardState[SDL_SCANCODE_PAGEDOWN]
			//);

			//transform->position += Vector3(posInput.x, posInput.y, posInput.z) * components.playerControllerComponents[i].GetSpeed();

			////YGHJTU to rotate (Same pattern as WASDQE)

			//Vector3 rotInput = Vector3(
			//	keyboardState[SDL_SCANCODE_G] - keyboardState[SDL_SCANCODE_J],
			//	keyboardState[SDL_SCANCODE_H] - keyboardState[SDL_SCANCODE_Y],
			//	keyboardState[SDL_SCANCODE_T] - keyboardState[SDL_SCANCODE_U]
			//);

			//transform->rotation += Vector3(rotInput.x, rotInput.y, rotInput.z) * components.playerControllerComponents[i].GetSpeed();

			////OKL;IP to scale (Same layout as WASDQE, but shifted over)

			//Vector3 scaleInput = Vector3(
			//	keyboardState[SDL_SCANCODE_K] - keyboardState[SDL_SCANCODE_SEMICOLON],
			//	keyboardState[SDL_SCANCODE_L] - keyboardState[SDL_SCANCODE_O],
			//	keyboardState[SDL_SCANCODE_I] - keyboardState[SDL_SCANCODE_P]
			//);

			//transform->scale += Vector3(scaleInput.x, scaleInput.y, scaleInput.z) * components.playerControllerComponents[i].GetSpeed();

			////Press space to reset transform component (return to origin, set rotation to identity, scale to 1, 1, 1)
			//if (keyboardState[SDL_SCANCODE_SPACE])
			//{
			//	transform->Reset();
			//}

			//std::cout << *transform << std::endl;
		}
		else
		{
			gpr460::engine->system->ErrorMessage(gpr460::ERROR_MISSING_TRANSFORM_REFERENCE);
			gpr460::engine->system->LogToErrorFile(gpr460::ERROR_MISSING_TRANSFORM_REFERENCE);
		}
	}
}