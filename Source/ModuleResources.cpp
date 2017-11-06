#include "ModuleResources.h"
#include "Texture.h"
#include "Mesh.h"
#include "Data.h"
#include "Resource.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleImport.h"

ModuleResources::ModuleResources(Application* app, bool start_enabled, bool is_game) : Module(app, start_enabled, is_game)
{
	name = "Resources";
}

ModuleResources::~ModuleResources()
{
	for (std::list<Texture*>::iterator it = textures_list.begin(); it != textures_list.end(); ++it) {
		RELEASE(*it);
	}
	for (std::list<Mesh*>::iterator it = meshes_list.begin(); it != meshes_list.end(); ++it) {
		RELEASE(*it);
	}
}

bool ModuleResources::Init(Data * editor_config)
{
	//FillResourcesLists();
	return true;
}

void ModuleResources::FillResourcesLists()
{
	std::string assets_folder_path = App->file_system->StringToPathFormat(ASSETS_FOLDER_PATH);
	std::vector<std::string> files_in_assets = App->file_system->GetAllFilesRecursively(assets_folder_path);
	
	for (std::vector<std::string>::iterator it = files_in_assets.begin(); it != files_in_assets.end(); it++)
	{
		std::string extension = App->file_system->GetFileExtension(*it);
		if (AssetExtensionToResourceType(extension) == Resource::ResourceType::Unknown) continue;
		Resource* resource = nullptr;
		if (HasMetaFile(*it))
		{
			Data data;
			if (data.LoadJSON(*it + ".meta"))
			{
				std::string library_path = data.GetString("Library_path");
				Resource::ResourceType type = (Resource::ResourceType)data.GetInt("Type");
				if (!App->file_system->FileExist(library_path))
				{
					CreateLibraryFile(type, *it);
				}
				resource = CreateResourceFromLibrary(library_path);
			}
		}
		else if(HasLibraryFile(*it))
		{
			std::string path = GetLibraryFile(*it);
			resource = CreateResourceFromLibrary(path);
			resource->CreateMeta();
		}
		else
		{
			std::string extension = App->file_system->GetFileExtension(*it);
			Resource::ResourceType type = AssetExtensionToResourceType(extension);
			CreateLibraryFile(type, *it);
			std::string path = GetLibraryFile(*it);
			//std::string path = App->get
			resource = CreateResourceFromLibrary(path);
			if(resource != nullptr) resource->CreateMeta();
		}

		AddResource(resource);
	}
}

Resource* ModuleResources::CreateResource(Resource::ResourceType type)
{
	Resource* resource = nullptr;

	switch (type)
	{
	case Resource::TextureResource:
		resource = (Resource*) new Texture();
		break;
	case Resource::MeshResource:
		resource = (Resource*) new Mesh();
		break;
	case Resource::SceneResource:
		break;
	case Resource::AnimationResource:
		break;
	case Resource::PrefabResource:
		break;
	case Resource::ScriptResource:
		break;
	case Resource::AudioResource:
		break;
	case Resource::ParticleFXResource:
		break;
	case Resource::FontResource:
		break;
	case Resource::Unknown:
		break;
	}

	return resource;
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
		break;
	case Resource::ScriptResource:
		break;
	case Resource::AudioResource:
		break;
	case Resource::ParticleFXResource:
		break;
	case Resource::FontResource:
		break;
	case Resource::Unknown:
		break;
	}
}

Texture * ModuleResources::GetTexture(std::string name) const
{
	for (std::list<Texture*>::const_iterator it = textures_list.begin(); it != textures_list.end(); it++)
	{
		if ((*it) != nullptr && (*it)->GetName() == name) return (*it);
	}
	return nullptr;
}

Texture * ModuleResources::GetTexture(UID uid) const
{
	for (std::list<Texture*>::const_iterator it = textures_list.begin(); it != textures_list.end(); it++)
	{
		if ((*it) != nullptr && (*it)->GetUID() == uid) return (*it);
	}
	return nullptr;
}

void ModuleResources::AddTexture(Texture * texture)
{
	if (texture != nullptr) textures_list.push_back(texture);
}

Mesh * ModuleResources::GetMesh(std::string name) const
{
	for (std::list<Mesh*>::const_iterator it = meshes_list.begin(); it != meshes_list.end(); it++)
	{
		if ((*it) != nullptr && (*it)->GetName() == name) return (*it);
	}
	return nullptr;
}

Mesh * ModuleResources::GetMesh(UID uid) const
{
	for (std::list<Mesh*>::const_iterator it = meshes_list.begin(); it != meshes_list.end(); it++)
	{
		if ((*it) != nullptr && (*it)->GetUID() == uid) return (*it);
	}
	return nullptr;
}

void ModuleResources::AddMesh(Mesh * mesh)
{
	if (mesh != nullptr) meshes_list.push_back(mesh);
}

Resource::ResourceType ModuleResources::AssetExtensionToResourceType(std::string str)
{
	if (str == ".jpg" || str == ".png" || str == ".tga" || str == ".dds") return Resource::TextureResource;
	else if (str == ".fbx" || str == ".FBX") return Resource::MeshResource;
	else if (str == ".wav" || str == ".ogg") return Resource::AudioResource;
	else if (str == ".prefab") return Resource::PrefabResource;
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

	return Resource::Unknown;
}

std::string ModuleResources::ResourceTypeToLibraryExtension(Resource::ResourceType type)
{
	if (type == Resource::TextureResource) return ".dds";
	else if (type == Resource::MeshResource) return ".mesh";
	else if (type == Resource::SceneResource) return ".scene";
	
	return "";
}

bool ModuleResources::HasMetaFile(std::string file_path)
{
	return App->file_system->FileExist(file_path + ".meta");
}

bool ModuleResources::HasLibraryFile(std::string file_path)
{
	std::string directory = App->file_system->StringToPathFormat(LIBRARY_FOLDER_PATH);
	return App->file_system->FileExistInDirectory(file_path, directory, true);
}

std::string ModuleResources::GetLibraryFile(std::string file_name)
{
	std::string name = App->file_system->GetFileNameWithoutExtension(file_name);
	std::string extension = App->file_system->GetFileExtension(file_name);
	Resource::ResourceType type = AssetExtensionToResourceType(extension);
	name += ResourceTypeToLibraryExtension(type);
	std::string directory = App->file_system->StringToPathFormat(LIBRARY_FOLDER_PATH);
	return App->file_system->GetFilePathInDirectory(name,directory,true);
}

void ModuleResources::CreateLibraryFile(Resource::ResourceType type, std::string file_path)
{
	switch (type)
	{
	case Resource::TextureResource:
		App->import->LoadTexture(file_path.c_str(), false);
		break;
	case Resource::MeshResource:
		App->import->LoadMesh(file_path.c_str());
		break;
	case Resource::SceneResource:
		break;
	case Resource::AnimationResource:
		break;
	case Resource::PrefabResource:
		break;
	case Resource::ScriptResource:
		break;
	case Resource::AudioResource:
		break;
	case Resource::ParticleFXResource:
		break;
	case Resource::FontResource:
		break;
	case Resource::Unknown:
		break;
	default:
		break;
	}
}

Resource * ModuleResources::CreateResourceFromLibrary(std::string library_path)
{
	std::string extension = App->file_system->GetFileExtension(library_path);
	Resource::ResourceType type = LibraryExtensionToResourceType(extension);
	Resource* resource = nullptr;

	switch (type)
	{
	case Resource::TextureResource:
		resource = (Resource*)App->import->LoadTexture(library_path.c_str());
		break;
	case Resource::MeshResource:
		break;
	case Resource::SceneResource:
		break;
	case Resource::AnimationResource:
		break;
	case Resource::PrefabResource:
		break;
	case Resource::ScriptResource:
		break;
	case Resource::AudioResource:
		break;
	case Resource::ParticleFXResource:
		break;
	case Resource::FontResource:
		break;
	case Resource::Unknown:
		break;
	default:
		break;
	}

	return resource;
}
