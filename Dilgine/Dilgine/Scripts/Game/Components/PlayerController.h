#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <iostream>

class GameObject;

class PlayerController
{
private:
	GameObject* gameObject;

	float speed = 1.0f;

public:
	enum { compID = 'PCTL' };

	static void Deserialize(GameObject& gameObject, std::istream& stream);

	static void Create(GameObject& gameObject, float vSpeed = 1.0f);

	PlayerController() { gameObject = nullptr; }
	PlayerController(float vSpeed, GameObject* vGameObject) { speed = vSpeed; gameObject = vGameObject; }
	~PlayerController() { }

	GameObject* GetGameObject() { return gameObject; }
	float GetSpeed() { return speed; }

	void SetSpeed(float vSpeed) { speed = vSpeed; }

	static void UpdateAll();
	
};

#endif
