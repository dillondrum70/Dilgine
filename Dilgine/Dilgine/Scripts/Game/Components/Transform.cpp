#include "Transform.h"
#include "GameObject.h"

//TRNS [x y]
void Transform::Deserialize(GameObject& gameObject, std::istream& stream)
{
	stream.ignore(100, '[');

	float x = 0, y = 0, z = 0;

	stream >> x;
	stream >> y;
	stream >> z;

	gameObject.CreateTransform(gameObject, Vector3(x, y, z));

	stream.ignore(100, ']');
}

void Transform::Create(GameObject& gameObject, Vector3 pos) 
{ 
	gameObject.CreateTransform(gameObject, pos); 
}