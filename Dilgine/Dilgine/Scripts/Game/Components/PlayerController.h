#ifndef PLAYER_CONTROLLER_H

class GameObject;

class PlayerController
{
private:
	GameObject* gameObject;

	float speed = 1;

public:
	PlayerController() { gameObject = nullptr; }
	PlayerController(GameObject* vGameObject) { gameObject = vGameObject; }

	GameObject* GetGameObject() { return gameObject; }
	float GetSpeed() { return speed; }

	void SetSpeed(float vSpeed) { speed = vSpeed; }

	void Update();
};

#endif
