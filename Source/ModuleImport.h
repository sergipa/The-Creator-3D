#pragma once
#include "Module.h"
#include <string>

class GameObject;
class Texture;
struct aiNode;
struct aiScene;
class Mesh;

class ModuleImport :
	public Module
{
public:
	ModuleImport(Application * app, bool start_enabled = true);
	~ModuleImport();

	bool CleanUp();

	void LoadFile(std::string path);
	bool LoadMesh(const char* path);
	Texture* LoadTexture(const char* path, bool attach_to_gameobject);

	//Loads images used in engine like files icons
	Texture* LoadEngineImages(const char* path);

	Mesh* LoadMeshFromLibrary(std::string path);

private:
	bool LoadMeshNode(GameObject* parent, aiNode* node, const aiScene* scene, const char* path);
	std::string GetFileName(const char* path);
	std::string GetFileExtension(const char* path);
};

void Callback(const char* message, char* c);
