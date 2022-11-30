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

	float moveSpeed = 1.0f;	//Speed at which the camera moves
	float rotateSpeed = 1.0f;	//Speed at which the camera moves
	float zoom = 5.0f;	//Distance from the center in the direction of eyeDirection
	Vector3 lookAtPosition = Vector3(0.0f);	//Point the camera looks at
	Vector3 eyeDirection = Vector3(0.0f);	//Independent of transform, unit vector that specifies direction of camera's position

public:
	enum { compID = 'CAMR' };

	static void Deserialize(GameObject& gameObject, std::istream& stream);

	static void Create(GameObject& gameObject, float vMoveSpeed = 1.0f, float vRotateSpeed = 1.0f, float vZoom = 5.0f, Vector3 lookAt = Vector3(0.0f), Vector3 eye = Vector3(5.0f, 5.0f, 0.0f));

	Camera() { gameObject = nullptr; }
	Camera(float vMoveSpeed, float vRotateSpeed, float vZoom, Vector3 lookAt, Vector3 eye, GameObject* vGameObject) { moveSpeed = vMoveSpeed; rotateSpeed = vRotateSpeed; zoom = vZoom; lookAtPosition = lookAt; eyeDirection = eye; gameObject = vGameObject; }
	~Camera() { }

	GameObject* GetGameObject() { return gameObject; }
	float GetMoveSpeed() { return moveSpeed; }
	float GetRotateSpeed() { return rotateSpeed; }
	float GetZoom() { return zoom; }
	Vector3 GetLookAtPosition() { return lookAtPosition; }
	Vector3 GetEyeDirection() { return eyeDirection; }

	void SetMoveSpeed(float vSpeed) { moveSpeed = vSpeed; }
	void SetRotateSpeed(float vSpeed) { rotateSpeed = vSpeed; }

	void Update();
	static void UpdateMain();
	static void UpdateAll();

};

#endif