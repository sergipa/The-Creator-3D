#include "ModuleResources.h"
#include "Texture.h"
#include "Mesh.h"
#include "Prefab.h"
#include "GameObject.h"
#include "Material.h"
#include "Resource.h"
#include "Data.h"
#include "Resource.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleMeshImporter.h"
#include "ModuleTextureImporter.h"
#include "ModulePrefabImporter.h"
#include "ModuleMaterialImporter.h"


ModuleResources::ModuleResources(Application* app, bool start_enabled, bool is_game) : Module(app, start_enabled, is_game)
{
	name = "Resources";
}

ModuleResources::~ModuleResources()
{
	for (std::map<uint, GameObject*>::iterator it = gameobjects_list.begin(); it != gameobjects_list.end(); ++it) {
		it->second->DeleteFromResourcesDestructor();
		it->second = nullptr;
	}
	gameobjects_list.clear();

	for (std::map<uint, Texture*>::iterator it = textures_list.begin(); it != textures_list.end(); ++it) {
		RELEASE(it->second);
	}
	textures_list.clear();
	for (std::map<uint, Mesh*>::iterator it = meshes_list.begin(); it != meshes_list.end(); ++it) {
		RELEASE(it->second);
	}
	meshes_list.clear();
	for (std::map<uint, Prefab*>::iterator it = prefabs_list.begin(); it != prefabs_list.end(); ++it) {
		RELEASE(it->second);
	}
	prefabs_list.clear();
	
	for (std::map<uint, Material*>::iterator it = materials_list.begin(); it != materials_list.end(); ++it) {
		RELEASE(it->second);
	}
	materials_list.clear();
}

bool ModuleResources::Init(Data * editor_config)
{
	FillResourcesLists();
	return true;
}

void ModuleResources::FillResourcesLists()
{
	std::string assets_folder_path = App->file_system->StringToPathFormat(ASSETS_FOLDER_PATH);
	std::vector<std::string> files_in_assets = App->file_system->GetAllFilesRecursively(assets_folder_path);

	if (!App->file_system->DirectoryExist(LIBRARY_FOLDER_PATH)) App->file_system->Create_Directory(LIBRARY_FOLDER_PATH);
	if (!App->file_system->DirectoryExist(LIBRARY_TEXTURES_FOLDER_PATH)) App->file_system->Create_Directory(LIBRARY_TEXTURES_FOLDER_PATH);
	if (!App->file_system->DirectoryExist(LIBRARY_MESHES_FOLDER_PATH)) App->file_system->Create_Directory(LIBRARY_MESHES_FOLDER_PATH);
	if (!App->file_system->DirectoryExist(LIBRARY_PREFABS_FOLDER_PATH)) App->file_system->Create_Directory(LIBRARY_PREFABS_FOLDER_PATH);
	if (!App->file_system->DirectoryExist(LIBRARY_MATERIALS_FOLDER_PATH)) App->file_system->Create_Directory(LIBRARY_MATERIALS_FOLDER_PATH);
	
	for (std::vector<std::string>::iterator it = files_in_assets.begin(); it != files_in_assets.end(); it++)
	{
		CreateResource(*it);
	}
}

void ModuleResources::AddResource(Resource * resource)
{
	if (resource == nullptr) return;
	Resource::ResourceType type = resource->GetType();
	switch (type)
	{
	case Resource::TextureResource:
		AddTexture((Texture*)resource);
		break;
	case Resource::MeshResource:
		AddMesh((Mesh*)resource);
		break;
	case Resource::SceneResource:
		break;
	case Resource::AnimationResource:
		break;
	case Resource::PrefabResource:
		AddPrefab((Prefab*)resource);
		break;
	case Resource::ScriptResource:
		break;
	case Resource::AudioResource:
		break;
	case Resource::ParticleFXResource:
		break;
	case Resource::FontResource:
		break;
	case Resource::MaterialResource:
		AddMaterial((Material*)resource);
		break;
	case Resource::Unknown:
		break;
	}
}

void ModuleResources::ImportFile(std::string path)
{
	std::string extension = App->file_system->GetFileExtension(path);
	std::string file_name = App->file_system->GetFileName(path);
	Resource::ResourceType type = AssetExtensionToResourceType(extension);

	bool exist = false;

	switch (type)
	{
	case Resource::TextureResource:
		if (!App->file_system->DirectoryExist(ASSETS_TEXTURES_FOLDER_PATH)) App->file_system->Create_Directory(ASSETS_TEXTURES_FOLDER_PATH);
		if (App->file_system->FileExist(ASSETS_TEXTURES_FOLDER + file_name)) exist = true; break;
		App->file_system->Copy_File(path, ASSETS_TEXTURES_FOLDER + file_name);
		path = ASSETS_TEXTURES_FOLDER + file_name;
		break;
	case Resource::MeshResource:
		if (!App->file_system->DirectoryExist(ASSETS_FBX_FOLDER_PATH)) App->file_system->Create_Directory(ASSETS_FBX_FOLDER_PATH);
		if (App->file_system->FileExist(ASSETS_FBX_FOLDER + file_name)) exist = true; break;
		App->file_system->Copy_File(path, ASSETS_FBX_FOLDER + file_name);
		path = ASSETS_FBX_FOLDER + file_name;
		break;
	case Resource::SceneResource:
		break;
	case Resource::AnimationResource:
		break;
	case Resource::PrefabResource:
		if (!App->file_system->DirectoryExist(ASSETS_PREFABS_FOLDER_PATH)) App->file_system->Create_Directory(ASSETS_PREFABS_FOLDER_PATH);
		if (App->file_system->FileExist(ASSETS_PREFABS_FOLDER + file_name)) exist = true; break;
		App->file_system->Copy_File(path, ASSETS_PREFABS_FOLDER + file_name);
		path = ASSETS_PREFABS_FOLDER + file_name;
		break;
	case Resource::ScriptResource:
		break;
	case Resource::AudioResource:
		break;
	case Resource::ParticleFXResource:
		break;
	case Resource::FontResource:
		break;
	case Resource::RenderTextureResource:
		break;
	case Resource::GameObjectResource:
		break;
	case Resource::MaterialResource:
		break;
	case Resource::Unknown:
		break;
	default:
		break;
	}

	if (!exist)CreateResource(path);
}

Texture * ModuleResources::GetTexture(std::string name) const
{
	for (std::map<uint, Texture*>::const_iterator it = textures_list.begin(); it != textures_list.end(); it++)
	{
		if (it->second != nullptr && it->second->GetName() == name) return it->second;
	}
	return nullptr;
}

Texture * ModuleResources::GetTexture(UID uid) const
{
	if (textures_list.find(uid) != textures_list.end()) return textures_list.at(uid);
	return nullptr;
}

void ModuleResources::AddTexture(Texture * texture)
{
	if (texture != nullptr)
	{
		textures_list[texture->GetUID()] = texture;
	}
}

void ModuleResources::RemoveTexture(Texture * texture)
{
	if (texture)
	{
		std::map<uint, Texture*>::iterator it = textures_list.find(texture->GetUID());
		if (it != textures_list.end()) textures_list.erase(it);
	}
}

std::map<uint, Texture*> ModuleResources::GetTexturesList() const
{
	return textures_list;
}

Mesh * ModuleResources::GetMesh(std::string name) const
{
	for (std::map<uint, Mesh*>::const_iterator it = meshes_list.begin(); it != meshes_list.end(); it++)
	{
		if (it->second != nullptr && it->second->GetName() == name) return it->second;
	}
	return nullptr;
}

Mesh * ModuleResources::GetMesh(UID uid) const
{
	if (meshes_list.find(uid) != meshes_list.end()) return meshes_list.at(uid);
	return nullptr;
}

void ModuleResources::AddMesh(Mesh * mesh)
{
	if (mesh != nullptr)
	{
		meshes_list[mesh->GetUID()] = mesh;
	}
}

void ModuleResources::RemoveMesh(Mesh * mesh)
{
	if (mesh)
	{
		std::map<uint, Mesh*>::iterator it = meshes_list.find(mesh->GetUID());
		if (it != meshes_list.end()) meshes_list.erase(it);
	}
}

std::map<uint, Mesh*> ModuleResources::GetMeshesList() const
{
	return meshes_list;
}

Prefab * ModuleResources::GetPrefab(std::string name) const
{
	for (std::map<uint, Prefab*>::const_iterator it = prefabs_list.begin(); it != prefabs_list.end(); it++)
	{
		if (it->second != nullptr && it->second->GetName() == name) return it->second;
	}
	return nullptr;
}

Prefab * ModuleResources::GetPrefab(UID uid) const
{
	if (prefabs_list.find(uid) != prefabs_list.end()) return prefabs_list.at(uid);
	return nullptr;
}

void ModuleResources::AddPrefab(Prefab * prefab)
{
	if (prefab != nullptr)
	{
		prefabs_list[prefab->GetUID()] = prefab;
	}
}

void ModuleResources::RemovePrefab(Prefab * prefab)
{
	if (prefab)
	{
		std::map<uint, Prefab*>::iterator it = prefabs_list.find(prefab->GetUID());
		if (it != prefabs_list.end()) prefabs_list.erase(it);
	}
}

std::map<uint, Prefab*> ModuleResources::GetPrefabsList() const
{
	return prefabs_list;
}

GameObject * ModuleResources::GetGameObject(std::string name) const
{
	for (std::map<uint, GameObject*>::const_iterator it = gameobjects_list.begin(); it != gameobjects_list.end(); it++)
	{
		if (it->second != nullptr && it->second->GetName() == name) return it->second;
	}
	return nullptr;
}

GameObject * ModuleResources::GetGameObject(UID uid) const
{
	if (gameobjects_list.find(uid) != gameobjects_list.end()) return gameobjects_list.at(uid);
	return nullptr;
}

void ModuleResources::AddGameObject(GameObject * gameobject)
{
	if (gameobject != nullptr)
	{
		gameobjects_list[gameobject->GetUID()] = gameobject;
	}
}

void ModuleResources::RemoveGameObject(GameObject * gameobject)
{
	if (gameobject)
	{
		std::map<uint, GameObject*>::iterator it = gameobjects_list.find(gameobject->GetUID());
		if (it != gameobjects_list.end()) gameobjects_list.erase(it);
	}
}

std::map<uint, GameObject*> ModuleResources::GetGameobjectsList() const
{
	return gameobjects_list;
}

Material * ModuleResources::GetMaterial(std::string name) const
{
	for (std::map<uint, Material*>::const_iterator it = materials_list.begin(); it != materials_list.end(); it++)
	{
		if (it->second != nullptr && it->second->GetName() == name) return it->second;
	}
	return nullptr;
}

Material * ModuleResources::GetMaterial(UID uid) const
{
	if (materials_list.find(uid) != materials_list.end()) return materials_list.at(uid);
	return nullptr;
}

void ModuleResources::AddMaterial(Material * material)
{
	if (material != nullptr)
	{
		materials_list[material->GetUID()] = material;
	}
}

void ModuleResources::RemoveMaterial(Material * material)
{
	if (material)
	{
		std::map<uint, Material*>::iterator it = materials_list.find(material->GetUID());
		if (it != materials_list.end()) materials_list.erase(it);
	}
}

std::map<uint, Material*> ModuleResources::GetMaterialsList() const
{
	return materials_list;
}

Resource::ResourceType ModuleResources::AssetExtensionToResourceType(std::string str)
{
	if (str == ".jpg" || str == ".png" || str == ".tga" || str == ".dds") return Resource::TextureResource;
	else if (str == ".fbx" || str == ".FBX") return Resource::MeshResource;
	else if (str == ".wav" || str == ".ogg") return Resource::AudioResource;
	else if (str == ".prefab") return Resource::PrefabResource;
	else if (str == ".mat") return Resource::MaterialResource;
	else if (str == ".animation") return Resource::AnimationResource;
	else if (str == ".particleFX") return Resource::ParticleFXResource;
	else if (str == ".scene") return Resource::SceneResource;
	else if (str == ".ttf") return Resource::FontResource;

	return Resource::Unknown;
}

Resource::ResourceType ModuleResources::LibraryExtensionToResourceType(std::string str)
{
	if (str == ".dds") return Resource::TextureResource;
	else if (str == ".mesh") return Resource::MeshResource;
	else if (str == ".scene") return Resource::SceneResource;
	else if (str == ".mat") return Resource::MaterialResource;
	else if (str == ".prefab" || str == ".fbx" || str == ".FBX") return Resource::PrefabResource;

	return Resource::Unknown;
}

std::string ModuleResources::ResourceTypeToLibraryExtension(Resource::ResourceType type)
{
	if (type == Resource::TextureResource) return ".dds";
	else if (type == Resource::MeshResource) return ".mesh";
	else if (type == Resource::SceneResource) return ".scene";
	else if (type == Resource::PrefabResource) return ".prefab";
	else if (type == Resource::MaterialResource) return ".mat";
	
	return "";
}

bool ModuleResources::HasMetaFile(std::string file_path)
{
	return App->file_system->FileExist(file_path + ".meta");
}

std::string ModuleResources::GetLibraryFile(std::string file_path)
{
	std::string extension = App->file_system->GetFileExtension(file_path);

	Resource::ResourceType type = AssetExtensionToResourceType(extension);

	if (extension == ".fbx" || extension == ".FBX") type = Resource::PrefabResource;

	std::string library_file;
	std::string directory;
	std::string file_name = App->file_system->GetFileNameWithoutExtension(file_path);

	switch (type)
	{
	case Resource::TextureResource:
		directory = App->file_system->StringToPathFormat(LIBRARY_TEXTURES_FOLDER);
		if (App->file_system->FileExistInDirectory(file_name + ".dds", directory, false))
		{
			library_file = directory + file_name + ".dds";
		}
		break;
	case Resource::MeshResource:
		directory = App->file_system->StringToPathFormat(LIBRARY_MESHES_FOLDER);
		if (App->file_system->FileExistInDirectory(file_name + ".mesh", directory, false))
		{
			library_file = directory + file_name + ".mesh";
		}
		break;
	case Resource::SceneResource:
		break;
	case Resource::AnimationResource:
		break;
	case Resource::PrefabResource:
		directory = App->file_system->StringToPathFormat(LIBRARY_PREFABS_FOLDER);
		if (App->file_system->FileExistInDirectory(file_name + ".prefab", directory, false))
		{
			library_file = directory + file_name + ".prefab";
		}
		break;
	case Resource::ScriptResource:
		break;
	case Resource::AudioResource:
		break;
	case Resource::ParticleFXResource:
		break;
	case Resource::FontResource:
		break;
	case Resource::MaterialResource:
		directory = App->file_system->StringToPathFormat(LIBRARY_MATERIALS_FOLDER);
		if (App->file_system->FileExistInDirectory(file_name + ".mat", directory, false))
		{
			library_file = directory + file_name + ".mat";
		}
		break;
	case Resource::Unknown:
		break;
	default:
		break;
	}

	return library_file;
}

std::string ModuleResources::CreateLibraryFile(Resource::ResourceType type, std::string file_path)
{
	std::string ret;

	if (!App->file_system->DirectoryExist(LIBRARY_FOLDER_PATH)) App->file_system->Create_Directory(LIBRARY_FOLDER_PATH);

	switch (type)
	{
	case Resource::TextureResource:
		if (!App->file_system->DirectoryExist(LIBRARY_TEXTURES_FOLDER_PATH)) App->file_system->Create_Directory(LIBRARY_TEXTURES_FOLDER_PATH);
		ret = App->texture_importer->ImportTexture(file_path);
		break;
	case Resource::MeshResource:
		if (!App->file_system->DirectoryExist(LIBRARY_MESHES_FOLDER_PATH)) App->file_system->Create_Directory(LIBRARY_MESHES_FOLDER_PATH);
		ret = App->mesh_importer->ImportMesh(file_path);
		break;
	case Resource::SceneResource:
		break;
	case Resource::AnimationResource:
		break;
	case Resource::PrefabResource:
		if (!App->file_system->DirectoryExist(LIBRARY_PREFABS_FOLDER_PATH)) App->file_system->Create_Directory(LIBRARY_PREFABS_FOLDER_PATH);
		ret = App->prefab_importer->ImportPrefab(file_path);
		break;
	case Resource::ScriptResource:
		break;
	case Resource::AudioResource:
		break;
	case Resource::ParticleFXResource:
		break;
	case Resource::FontResource:
		break;
	case Resource::MaterialResource:
		if (!App->file_system->DirectoryExist(LIBRARY_MATERIALS_FOLDER_PATH)) App->file_system->Create_Directory(LIBRARY_MATERIALS_FOLDER_PATH);
		ret = App->material_importer->ImportMaterial(file_path);
		break;
	case Resource::Unknown:
		break;
	default:
		break;
	}

	return ret;
}

Resource * ModuleResources::CreateResourceFromLibrary(std::string library_path)
{
	std::string extension = App->file_system->GetFileExtension(library_path);
	std::string name = App->file_system->GetFileNameWithoutExtension(library_path);
	Resource::ResourceType type = LibraryExtensionToResourceType(extension);
	Resource* resource = nullptr;

	switch (type)
	{
	case Resource::TextureResource:
		if (GetTexture(name) != nullptr) break;
		resource = (Resource*)App->texture_importer->LoadTextureFromLibrary(library_path);
		break;
	case Resource::MeshResource:
		if (GetMesh(name) != nullptr) break;
		resource = (Resource*)App->mesh_importer->LoadMeshFromLibrary(library_path);
		break;
	case Resource::SceneResource:
		break;
	case Resource::AnimationResource:
		break;
	case Resource::PrefabResource:
		if (GetPrefab(name) != nullptr) break;
		resource = (Resource*)App->prefab_importer->LoadPrefabFromLibrary(library_path);
		break;
	case Resource::ScriptResource:
		break;
	case Resource::AudioResource:
		break;
	case Resource::ParticleFXResource:
		break;
	case Resource::FontResource:
		break;
	case Resource::MaterialResource:
		if (GetMaterial(name) != nullptr) break;
		resource = (Resource*)App->material_importer->LoadMaterialFromLibrary(library_path);
		break;
	case Resource::Unknown:
		break;
	default:
		break;
	}

	if (resource != nullptr)
	{
		AddResource(resource);
	}

	return resource;
}

void ModuleResources::CreateResource(std::string file_path)
{
	std::string extension = App->file_system->GetFileExtension(file_path);
	std::string library_path;
	if (AssetExtensionToResourceType(extension) == Resource::ResourceType::Unknown) return;
	Resource* resource = nullptr;
	if (HasMetaFile(file_path))
	{
		Data data;
		if (data.LoadJSON(file_path + ".meta"))
		{
			library_path = data.GetString("Library_path");
			Resource::ResourceType type = (Resource::ResourceType)data.GetInt("Type");
			if (!App->file_system->FileExist(library_path))
			{
				if (extension == ".fbx" || extension == ".FBX") type = Resource::MeshResource;
				library_path = CreateLibraryFile(type, file_path);
			}
			resource = CreateResourceFromLibrary(library_path);
			if (resource != nullptr)
			{
				resource->SetAssetsPath(file_path);
			}
		}
	}
	else if (GetLibraryFile(file_path) != "")
	{
		std::string path = GetLibraryFile(file_path);
		resource = CreateResourceFromLibrary(path);
		if (resource != nullptr)
		{
			resource->SetAssetsPath(file_path);
			if (!HasMetaFile(file_path)) resource->CreateMeta();
		}
	}
	else
	{
		Resource::ResourceType type = AssetExtensionToResourceType(extension);
		library_path = CreateLibraryFile(type, file_path);
		resource = CreateResourceFromLibrary(library_path);
		if (resource != nullptr)
		{
			resource->SetAssetsPath(file_path);
			resource->CreateMeta();
		}
	}
}

void ModuleResources::DeleteResource(std::string file_path)
{
	std::string extension = App->file_system->GetFileExtension(file_path);
	Resource::ResourceType type = LibraryExtensionToResourceType(extension);
	std::string resource_name = App->file_system->GetFileNameWithoutExtension(file_path);

	Texture* texture = nullptr;
	Mesh* mesh = nullptr;
	Prefab* prefab = nullptr;
	Material* material = nullptr;

	switch (type)
	{
	case Resource::TextureResource:
		texture = GetTexture(resource_name);
		App->file_system->Delete_File(texture->GetLibraryPath());
		App->file_system->Delete_File(file_path + ".meta");
		RemoveTexture(texture);
		break;
	case Resource::MeshResource:
		mesh = GetMesh(resource_name);
		App->file_system->Delete_File(mesh->GetLibraryPath());
		App->file_system->Delete_File(file_path + ".meta");
		RemoveMesh(mesh);
		break;
	case Resource::SceneResource:
		break;
	case Resource::AnimationResource:
		break;
	case Resource::PrefabResource:
		prefab = GetPrefab(resource_name);
		App->file_system->Delete_File(prefab->GetLibraryPath());
		App->file_system->Delete_File(file_path + ".meta");
		RemovePrefab(prefab);
		break;
	case Resource::ScriptResource:
		break;
	case Resource::AudioResource:
		break;
	case Resource::ParticleFXResource:
		break;
	case Resource::FontResource:
		break;
	case Resource::RenderTextureResource:
		break;
	case Resource::GameObjectResource:
		/*GameObject* gameobject = GetGameObject(file_path);
		RemoveGameObject(gameobject);*/
		break;
	case Resource::MaterialResource:
		material = GetMaterial(resource_name);
		App->file_system->Delete_File(material->GetLibraryPath());
		App->file_system->Delete_File(file_path + ".meta");
		RemoveMaterial(material);
		break;
	case Resource::Unknown:
		break;
	default:
		break;
	}
}

bool ModuleResources::CheckResourceName(std::string& name)
{
	bool ret = false;

	for (std::string::iterator it = name.begin(); it != name.end(); it++)
	{
		if (*it == ':' || *it == '\\' || *it == '//' || *it == '*' || *it == '?' || *it == '"' || *it == '<' || *it == '>' || *it == '|')
		{
			*it = '_';
			ret = true;
		}
	}

	return ret;
}
