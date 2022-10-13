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
	PlayerController() { gameObject = nullptr; }
	PlayerController(GameObject* vGameObject) { gameObject = vGameObject; }
	~PlayerController() { }

	GameObject* GetGameObject() { return gameObject; }
	int GetSpeed() { return speed; }

	void SetSpeed(int vSpeed) { speed = vSpeed; }

	static void UpdateAll();
	
};

#endif
