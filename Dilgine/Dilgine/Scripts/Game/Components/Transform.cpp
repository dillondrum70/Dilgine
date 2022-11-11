#include "Transform.h"
#include "GameObject.h"

//TRNS [x y]
void Transform::Deserialize(GameObject& gameObject, std::istream& stream)
{
	stream.ignore(100, '[');

	int x = 0, y = 0;

	stream >> x;
	stream >> y;

	gameObject.CreateTransform(gameObject, Vector2(x, y));

	stream.ignore(100, ']');
}

void Transform::Create(GameObject& gameObject, Vector2 pos) 
{ 
	gameObject.CreateTransform(gameObject, pos); 
}