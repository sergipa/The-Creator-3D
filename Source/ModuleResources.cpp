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
	for (std::vector<Texture*>::iterator it = textures_list.begin(); it != textures_list.end(); ++it) {
		RELEASE(*it);
	}
	for (std::vector<Mesh*>::iterator it = meshes_list.begin(); it != meshes_list.end(); ++it) {
		RELEASE(*it);
	}
}

bool ModuleResources::Init(Data * editor_config)
{
	if (!App->file_system->DirectoryExist(LIBRARY_FOLDER_PATH)) App->file_system->Create_Directory(LIBRARY_FOLDER_PATH);
	if (!App->file_system->DirectoryExist(LIBRARY_TEXTURES_FOLDER_PATH)) App->file_system->Create_Directory(LIBRARY_TEXTURES_FOLDER_PATH);
	if (!App->file_system->DirectoryExist(LIBRARY_MESHES_FOLDER_PATH)) App->file_system->Create_Directory(LIBRARY_MESHES_FOLDER_PATH);
	if (!App->file_system->DirectoryExist(LIBRARY_PREFABS_FOLDER_PATH)) App->file_system->Create_Directory(LIBRARY_PREFABS_FOLDER_PATH);
	if (!App->file_system->DirectoryExist(LIBRARY_MATERIALS_FOLDER_PATH)) App->file_system->Create_Directory(LIBRARY_MATERIALS_FOLDER_PATH);
	FillResourcesLists();
	return true;
}

void ModuleResources::FillResourcesLists()
{
	std::string assets_folder_path = App->file_system->StringToPathFormat(ASSETS_FOLDER_PATH);
	std::vector<std::string> files_in_assets = App->file_system->GetAllFilesRecursively(assets_folder_path);
	
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
	/*std::string library_path = CreateLibraryFile(type, path);
	Resource* resource = nullptr;
	resource = CreateResourceFromLibrary(library_path);
	AddResource(resource);*/

	std::string extension = App->file_system->GetFileExtension(path);
	std::string file_name = App->file_system->GetFileName(path);
	Resource::ResourceType type = AssetExtensionToResourceType(extension);

	switch (type)
	{
	case Resource::TextureResource:
		if (!App->file_system->DirectoryExist(ASSETS_TEXTURES_FOLDER_PATH)) App->file_system->Create_Directory(ASSETS_TEXTURES_FOLDER_PATH);
		App->file_system->Copy_File(path, ASSETS_TEXTURES_FOLDER + file_name);
		path = ASSETS_TEXTURES_FOLDER + file_name;
		break;
	case Resource::MeshResource:
		if (!App->file_system->DirectoryExist(ASSETS_FBX_FOLDER_PATH)) App->file_system->Create_Directory(ASSETS_FBX_FOLDER_PATH);
		App->file_system->Copy_File(path, ASSETS_FBX_FOLDER + file_name);
		path = ASSETS_FBX_FOLDER + file_name;
		break;
	case Resource::SceneResource:
		break;
	case Resource::AnimationResource:
		break;
	case Resource::PrefabResource:
		if (!App->file_system->DirectoryExist(ASSETS_PREFABS_FOLDER_PATH)) App->file_system->Create_Directory(ASSETS_PREFABS_FOLDER_PATH);
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

	CreateResource(path);
}

Texture * ModuleResources::GetTexture(std::string name) const
{
	for (std::vector<Texture*>::const_iterator it = textures_list.begin(); it != textures_list.end(); it++)
	{
		if ((*it) != nullptr && (*it)->GetName() == name) return (*it);
	}
	return nullptr;
}

Texture * ModuleResources::GetTexture(UID uid) const
{
	for (std::vector<Texture*>::const_iterator it = textures_list.begin(); it != textures_list.end(); it++)
	{
		if ((*it) != nullptr && (*it)->GetUID() == uid) return (*it);
	}
	return nullptr;
}

void ModuleResources::AddTexture(Texture * texture)
{
	if (texture != nullptr)
	{
		if (std::find(textures_list.begin(), textures_list.end(), texture) == textures_list.end())
		{
			textures_list.push_back(texture);
		}
	}
}

void ModuleResources::RemoveTexture(Texture * texture)
{
	std::vector<Texture*>::iterator it = std::find(textures_list.begin(), textures_list.end(), texture);
	if(it != textures_list.end()) textures_list.erase(it);
}

Mesh * ModuleResources::GetMesh(std::string name) const
{
	for (std::vector<Mesh*>::const_iterator it = meshes_list.begin(); it != meshes_list.end(); it++)
	{
		if ((*it) != nullptr && (*it)->GetName() == name) return (*it);
	}
	return nullptr;
}

Mesh * ModuleResources::GetMesh(UID uid) const
{
	for (std::vector<Mesh*>::const_iterator it = meshes_list.begin(); it != meshes_list.end(); it++)
	{
		if ((*it) != nullptr && (*it)->GetUID() == uid) return (*it);
	}
	return nullptr;
}

void ModuleResources::AddMesh(Mesh * mesh)
{
	if (mesh != nullptr)
	{
		if (std::find(meshes_list.begin(), meshes_list.end(), mesh) == meshes_list.end())
		{
			meshes_list.push_back(mesh);
		}
	}
}

void ModuleResources::RemoveMesh(Mesh * mesh)
{
	std::vector<Mesh*>::iterator it = std::find(meshes_list.begin(), meshes_list.end(), mesh);
	if (it != meshes_list.end()) meshes_list.erase(it);
}

Prefab * ModuleResources::GetPrefab(std::string name) const
{
	for (std::vector<Prefab*>::const_iterator it = prefabs_list.begin(); it != prefabs_list.end(); it++)
	{
		if ((*it) != nullptr && (*it)->GetName() == name) return (*it);
	}
	return nullptr;
}

Prefab * ModuleResources::GetPrefab(UID uid) const
{
	for (std::vector<Prefab*>::const_iterator it = prefabs_list.begin(); it != prefabs_list.end(); it++)
	{
		if ((*it) != nullptr && (*it)->GetUID() == uid) return (*it);
	}
	return nullptr;
}

void ModuleResources::AddPrefab(Prefab * prefab)
{
	if (prefab != nullptr)
	{
		if (std::find(prefabs_list.begin(), prefabs_list.end(), prefab) == prefabs_list.end())
		{
			prefabs_list.push_back(prefab);
		}
	}
}

void ModuleResources::RemovePrefab(Prefab * prefab)
{
	std::vector<Prefab*>::iterator it = std::find(prefabs_list.begin(), prefabs_list.end(), prefab);
	if (it != prefabs_list.end()) prefabs_list.erase(it);
}

GameObject * ModuleResources::GetGameObject(std::string name) const
{
	for (std::vector<GameObject*>::const_iterator it = gameobjects_list.begin(); it != gameobjects_list.end(); it++)
	{
		if ((*it) != nullptr && (*it)->GetName() == name) return (*it);
	}
	return nullptr;
}

GameObject * ModuleResources::GetGameObject(UID uid) const
{
	for (std::vector<GameObject*>::const_iterator it = gameobjects_list.begin(); it != gameobjects_list.end(); it++)
	{
		if ((*it) != nullptr && (*it)->GetUID() == uid) return (*it);
	}
	return nullptr;
}

void ModuleResources::AddGameObject(GameObject * gameobject)
{
	if (gameobject != nullptr)
	{
		if (std::find(gameobjects_list.begin(), gameobjects_list.end(), gameobject) == gameobjects_list.end())
		{
			gameobjects_list.push_back(gameobject);
		}
	}
}

void ModuleResources::RemoveGameObject(GameObject * gameobject)
{
	std::vector<GameObject*>::iterator it = std::find(gameobjects_list.begin(), gameobjects_list.end(), gameobject);
	if (it != gameobjects_list.end()) gameobjects_list.erase(it);
}

Material * ModuleResources::GetMaterial(std::string name) const
{
	for (std::vector<Material*>::const_iterator it = materials_list.begin(); it != materials_list.end(); it++)
	{
		if ((*it) != nullptr && (*it)->GetName() == name) return (*it);
	}
	return nullptr;
}

Material * ModuleResources::GetMaterial(UID uid) const
{
	for (std::vector<Material*>::const_iterator it = materials_list.begin(); it != materials_list.end(); it++)
	{
		if ((*it) != nullptr && (*it)->GetUID() == uid) return (*it);
	}
	return nullptr;
}

void ModuleResources::AddMaterial(Material * material)
{
	if (material != nullptr)
	{
		if (std::find(materials_list.begin(), materials_list.end(), material) == materials_list.end())
		{
			materials_list.push_back(material);
		}
	}
}

void ModuleResources::RemoveMaterial(Material * material)
{
	std::vector<Material*>::iterator it = std::find(materials_list.begin(), materials_list.end(), material);
	if (it != materials_list.end()) materials_list.erase(it);
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

	switch (type)
	{
	case Resource::TextureResource:
		ret = App->texture_importer->ImportTexture(file_path);
		break;
	case Resource::MeshResource:
		ret = App->mesh_importer->ImportMesh(file_path);
		break;
	case Resource::SceneResource:
		break;
	case Resource::AnimationResource:
		break;
	case Resource::PrefabResource:
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
	Resource::ResourceType type = LibraryExtensionToResourceType(extension);
	Resource* resource = nullptr;

	switch (type)
	{
	case Resource::TextureResource:
		resource = (Resource*)App->texture_importer->LoadTextureFromLibrary(library_path);
		break;
	case Resource::MeshResource:
		resource = (Resource*)App->mesh_importer->LoadMeshFromLibrary(library_path);
		break;
	case Resource::SceneResource:
		break;
	case Resource::AnimationResource:
		break;
	case Resource::PrefabResource:
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
		std::string extension = App->file_system->GetFileExtension(file_path);
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
