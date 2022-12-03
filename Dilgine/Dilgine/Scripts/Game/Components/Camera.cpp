#include "Camera.h"
#include "GameObject.h"

#include "SDL2/SDL.h"
#include "System.h"

Camera::Camera(float vZoomSpeed, float vMoveSpeed, float vRotateSpeed, Vector3 lookAt, Vector3 eye, bool vAttachToTransform, GameObject* vGameObject)
{
	zoomSpeed = vZoomSpeed;
	moveSpeed = vMoveSpeed; 
	rotateSpeed = vRotateSpeed;
	lookAtPosition = lookAt; 
	eyePosition = eye; 
	gameObject = vGameObject;
	attachToTransform = vAttachToTransform;
}

//attachToTransform stored as 0 or 1
//CAMR [zoomSpeed moveSpeed rotateSpeed (lookAt.x lookAt.y, lookAt.z) (eye.x, eye.y, eye.z) attachToTransform]
void Camera::Deserialize(GameObject& gameObject, std::istream& stream)
{
	stream.ignore(100, '[');

	float mSpeed = 0, rSpeed = 0, zSpeed = 0;
	Vector3 lookAt = Vector3(0.0f), eye = Vector3(0.0f);
	std::string checkInput = "";
	bool attach = true;

	stream >> zSpeed;
	stream >> mSpeed;
	stream >> rSpeed;
	

	stream.ignore(100, '(');
	stream >> lookAt.x;
	stream >> lookAt.y;
	stream >> lookAt.z;

	stream.ignore(100, '(');
	stream >> eye.x;
	stream >> eye.y;
	stream >> eye.z;

	stream.ignore(100, ' ');
	std::getline(stream, checkInput, ']');	//Read until character before ]

	if (checkInput == "0" || checkInput == "false")
	{
		attach = false;
	}

	//e = e.Normalized();

	gameObject.CreateCamera(gameObject, zSpeed, mSpeed, rSpeed, lookAt, eye, attach);

	//stream.ignore(100, ']'); //Don't need to ignore, getline handled that
}

void Camera::Create(GameObject& gameObject, float vZoomSpeed, float vMoveSpeed, float vRotateSpeed, Vector3 lookAt, Vector3 eye, bool vAttachToTransform)
{
	gameObject.CreateCamera(gameObject, vZoomSpeed, vMoveSpeed, vRotateSpeed, lookAt, eye, vAttachToTransform);
}

Vector3 Camera::CalculateLookAtPosition()
{
	if (attachToTransform)
	{
		Transform* transform = gameObject->GetTransform();

		Vector3 positionOffset = Vector3(0.0f);

		if (transform)
		{
			//TODO: Add transform->rotation to this calculation
			Vector3 transEye = (transform->position + lookAtPosition);
			return transEye;
		}
	}

	return lookAtPosition;
}

Vector3 Camera::CalculateEyePosition()
{
	if (attachToTransform)
	{
		Transform* transform = gameObject->GetTransform();

		Vector3 positionOffset = Vector3(0.0f);

		if (transform)
		{
			//TODO: Add transform->rotation to this calculation
			Vector3 transEye = (transform->position + eyePosition);
			return transEye;
		}
	}

	return eyePosition;
}

void Camera::Update()
{
	Vector3 lookDirForward = (eyePosition - lookAtPosition).Normalized();
	Vector3 lookDirRight = Vector3::Cross(lookDirForward, Vector3::Up()).Normalized();
	Vector3 lookDirUp = Vector3::Cross(lookDirForward, lookDirRight).Normalized();
	float dist = (lookAtPosition - eyePosition).Magnitude();	//Calculates distance between lookAt and eye so we can maintain that distance when orbiting for example

	//Zoom
	eyePosition -= lookDirForward * gpr460::engine->input.GetMouseState().mouseWheelYChange * zoomSpeed;

	MouseState mouseState = gpr460::engine->input.GetMouseState();

	float mouseDragX = mouseState.mouseXChange * rotateSpeed;
	float mouseDragY = mouseState.mouseYChange * rotateSpeed;

	//Orbit the center point, can't move
	if (mouseState.rightMouse)
	{
		orbiting = true;

		//Rotation
		//mouseDragX -= mouseState.mouseXChange * rotateSpeed;

		//mouseDragY += mouseState.mouseYChange * rotateSpeed;

		eyePosition += (lookDirRight * mouseDragX) + (lookDirUp * -mouseDragY);
		Vector3 diff = (eyePosition - lookAtPosition).Normalized() * dist;
		eyePosition = lookAtPosition + diff;
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

		Vector3 move = ((lookDirRight * posInput.x) + (lookDirForward * posInput.y) + (lookDirUp * posInput.z)) * moveSpeed;

		eyePosition += move;

		//Rotation
		//mouseDragX -= mouseState.mouseXChange * rotateSpeed;

		//mouseDragY -= mouseState.mouseYChange * rotateSpeed;

		lookAtPosition += move;
		lookAtPosition -= (lookDirRight * mouseDragX) + (lookDirUp * -mouseDragY);
		Vector3 diff = (lookAtPosition - eyePosition).Normalized() * dist;
		lookAtPosition = eyePosition + diff;
	}
	
	//std::cout << eyePosition << "  " << lookAtPosition << std::endl;
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