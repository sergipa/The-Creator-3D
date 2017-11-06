#pragma once
#include "Module.h"
#include "Resource.h"
#include <list>

class Texture;
class Mesh;

class ModuleResources :
	public Module
{
public:
	ModuleResources(Application* app, bool start_enabled = true, bool is_game = false);
	~ModuleResources();

	bool Init(Data* editor_config = nullptr);

	void FillResourcesLists();

	Resource* CreateResource(Resource::ResourceType type);
	void AddResource(Resource* resource);

	Texture* GetTexture(std::string name) const;
	Texture* GetTexture(UID uid) const;
	void AddTexture(Texture* texture);

	Mesh* GetMesh(std::string name) const;
	Mesh* GetMesh(UID uid) const;
	void AddMesh(Mesh* mesh);

	Resource::ResourceType AssetExtensionToResourceType(std::string str);
	Resource::ResourceType LibraryExtensionToResourceType(std::string str);
	std::string ResourceTypeToLibraryExtension(Resource::ResourceType type);

	bool HasMetaFile(std::string file_path);
	bool HasLibraryFile(std::string file_path);
	std::string GetLibraryFile(std::string file_name);
	void CreateLibraryFile(Resource::ResourceType type, std::string file_path);
	Resource* CreateResourceFromLibrary(std::string library_path);


private:
	std::list<Texture*> textures_list;
	std::list<Mesh*> meshes_list;
};

