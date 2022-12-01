#include "Transform.h"
#include "GameObject.h"

//TRNS [(x y z) (xRot yRot zRot) (xScale yScale zScale)]
void Transform::Deserialize(GameObject& gameObject, std::istream& stream)
{
	stream.ignore(100, '[');

	float x = 0, y = 0, z = 0;

	stream.ignore(100, '(');
	stream >> x;
	stream >> y;
	stream >> z;

	Vector3 pos = Vector3(x, y, z);

	stream.ignore(100, '(');
	stream >> x;
	stream >> y;
	stream >> z;

	Vector3 rot = Vector3(x, y, z);

	stream.ignore(100, '(');
	stream >> x;
	stream >> y;
	stream >> z;

	Vector3 scale = Vector3(x, y, z);


	gameObject.CreateTransform(gameObject, pos, rot, scale);

	stream.ignore(100, ']');
}

void Transform::Create(GameObject& gameObject, Vector3 pos, Vector3 rot, Vector3 scale) 
{ 
	gameObject.CreateTransform(gameObject, pos, rot, scale); 
}

Vector3 Transform::GetForward()
{
	return Vector3(
		std::cos(rotation.x) * std::sin(rotation.z),
		-std::sin(rotation.x),
		std::cos(rotation.x) * std::cos(rotation.z)
	);
}

Vector3 Transform::GetRight()
{
	return Vector3(
		std::sin(rotation.z),
		0,
		-std::cos(rotation.z)
	);
}

Vector3 Transform::GetUp()
{
	return Vector3(
		std::sin(rotation.x) * std::sin(rotation.z),
		std::cos(rotation.x),
		std::sin(rotation.x) * std::cos(rotation.z)
	);
}

void Transform::Reset()
{
	position = Vector3(0.0f);
	rotation = Vector3(0.0f);
	scale = Vector3(1.0f);
}