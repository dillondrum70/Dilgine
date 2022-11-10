#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <iostream>

class GameObject;

class PlayerController
{
private:
	GameObject* gameObject;

	int speed = 1;

public:
	enum { compID = 'PCTL' };

	static void Create(GameObject& gameObject, int vSpeed = 1);

	PlayerController() { gameObject = nullptr; }
	PlayerController(int vSpeed, GameObject* vGameObject) { speed = vSpeed; gameObject = vGameObject; }
	~PlayerController() { }

	GameObject* GetGameObject() { return gameObject; }
	int GetSpeed() { return speed; }

	void SetSpeed(int vSpeed) { speed = vSpeed; }

	static void UpdateAll();
	
};

#endif
