#pragma once
#include "Module.h"
#include <string>
#include "Assimp\include\quaternion.h"
#include "Assimp\include\vector3.h"

class Mesh;
struct aiNode;
struct aiScene;
class GameObject;
class Data;

class ModuleMeshImporter :
	public Module
{
public:
	ModuleMeshImporter(Application* app, bool start_enabled = true, bool is_game = false);
	~ModuleMeshImporter();

	bool Init(Data* editor_config = nullptr);
	bool CleanUp();

	std::string ImportMesh(std::string path);
	Mesh* LoadMeshFromLibrary(std::string path);
	void SaveMeshToLibrary(Mesh& mesh);

private:
	//Loads the model and returns the root gameobject
	GameObject* LoadMeshNode(GameObject* parent, aiNode* node, const aiScene* scene, const char* path);
	void GetDummyTransform(aiNode& node, aiVector3D& pos, aiQuaternion& rot, aiVector3D& scale);
};

void Callback(const char* message, char* c);

