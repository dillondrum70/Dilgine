#ifndef SERIALIZATION_H
#define SERIALIZATION_H
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <iostream>

class GameObject;

typedef void (*ComponentLoadFunction)(GameObject& gameObject, std::istream& stream);

union CharId {
	char charData[4];
	int compId;
};

class Serialize
{
private:
	std::unordered_map<int, ComponentLoadFunction> componentCreationMap;

	void RegisterComponentFunctions();

	//Unneccesary overhead, making a function call here wastes time
	//void AddComponent(GameObject& gameObject, int compId, std::istream& stream) { componentCreationMap[compId](gameObject, stream); }

	void ParseLine(std::istream& stream, GameObject& gameObject);
public:
	Serialize() { RegisterComponentFunctions(); }

	void LoadLevel(std::string filename);
};

#endif
