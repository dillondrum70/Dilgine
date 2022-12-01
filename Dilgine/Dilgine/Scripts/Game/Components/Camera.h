#ifndef CAMERA_H
#define CAMERA_H
#include "Vector3.h"

#include <iostream>

class GameObject;

class Camera
{
private:
	GameObject* gameObject;

	float mouseDragX = 0;
	float mouseDragY = 0;

	float zoomSpeed = 1.0f;	//Speed at which camera zooms when using scroll wheel
	float moveSpeed = 1.0f;	//Speed at which the camera moves
	float rotateSpeed = 1.0f;	//Speed at which the camera moves
	float zoom = 5.0f;	//Distance from the center in the direction of eyeDirection
	Vector3 lookAtPosition = Vector3(0.0f);	//Point the camera looks at
	Vector3 eyePosition = Vector3(0.0f);	//Independent of transform, unit vector that specifies direction of camera's position

	bool orbiting = true; //If orbiting, right mouse was pressed and camera orbits its center point, otherwise, it moves and looks around freely

public:
	enum { compID = 'CAMR' };

	static void Deserialize(GameObject& gameObject, std::istream& stream);

	static void Create(GameObject& gameObject, float vZoomSpeed = 1.0f, float vMoveSpeed = 1.0f, float vRotateSpeed = 1.0f, Vector3 lookAt = Vector3(0.0f), Vector3 eye = Vector3(5.0f, 5.0f, 0.0f));

	Camera() { gameObject = nullptr; }
	Camera(float vZoomSpeed, float vMoveSpeed, float vRotateSpeed, Vector3 lookAt, Vector3 eye, GameObject* vGameObject);
	~Camera() { }

	GameObject* GetGameObject() { return gameObject; }
	float GetMoveSpeed() { return moveSpeed; }
	float GetRotateSpeed() { return rotateSpeed; }
	float GetZoom() { return zoom; }
	Vector3 GetLookAtPosition() { return lookAtPosition; }
	Vector3 GetEyePosition() { return eyePosition; }	
	bool GetOrbiting() { return orbiting; }

	void SetMoveSpeed(float vSpeed) { moveSpeed = vSpeed; }
	void SetRotateSpeed(float vSpeed) { rotateSpeed = vSpeed; }

	Vector3 CalculateEyePosition();//We might do extra changes to eye position if we are orbiting

	void Update();
	static void UpdateMain();
	static void UpdateAll();

};

#endif