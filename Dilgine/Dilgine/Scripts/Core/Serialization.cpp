#include "Serialization.h"

#include "GameObject.h"
#include "System.h"

void Serialize::RegisterComponentFunctions()
{
	componentCreationMap[Transform::compID] = Transform::Deserialize;
	componentCreationMap[RectangleRenderer::compID] = RectangleRenderer::Deserialize;
	componentCreationMap[RectangleCollider::compID] = RectangleCollider::Deserialize;
	componentCreationMap[PlayerController::compID] = PlayerController::Deserialize;
	componentCreationMap[CollisionColorChanger::compID] = CollisionColorChanger::Deserialize;
	componentCreationMap[MeshRenderer::compID] = MeshRenderer::Deserialize;
	componentCreationMap[Camera::compID] = Camera::Deserialize;
}

void Serialize::LoadLevel(std::string filename)
{
	std::ifstream fin;

	fin.open(filename);

	if (!fin.fail())
	{
		while (fin)
		{
			GameObject current;

			std::string temp;
			std::getline(fin, temp);

			std::istringstream line(temp);

			ParseLine(line, current);

			gpr460::engine->world->AddGameObject(current);
		}

		fin.close();
	}
	else
	{
		gpr460::engine->system->ErrorMessage(gpr460::ERROR_LEVEL_OPEN_FAIL);
		gpr460::engine->system->LogToErrorFile(gpr460::ERROR_LEVEL_OPEN_FAIL);
	}
}

void Serialize::ParseLine(std::istream& stream, GameObject& gameObject)
{
	int currentId = -1;

	stream >> currentId;
	stream.ignore(100, ':');
	

	while (stream)
	{
		CharId id;

		stream.ignore(100, ' ');
		
		//Endianness, must store characters in reverse order
		id.charData[3] = stream.get();
		id.charData[2] = stream.get();
		id.charData[1] = stream.get();
		id.charData[0] = stream.get();

		if (componentCreationMap[id.compId])
		{
			componentCreationMap[id.compId](gameObject, stream);
		}
	}
}