#ifndef WORLD_H
#include "GameObject.h"

class World
{
private:
	GameObject* background;
	GameObject* wall;
	GameObject* player;

public:
	World() { background = nullptr; wall = nullptr; player = nullptr; }
	~World() { CleanUp(); }
	
	void Init();
	void CleanUp();

	void Update();

	void Render();
};

#endif
