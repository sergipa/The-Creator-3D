#include "ModuleImport.h"
#include "GameObject.h"
#include "ComponentMeshRenderer.h"
#include "Component.h"
#include "Application.h"
#include "ModuleScene.h"
#include "OpenGL.h"
#include "Mesh.h"

#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "Devil/include/il.h"
#include "Devil/include/ilut.h"
#pragma comment ( lib, "Devil/libx86/DevIL.lib" )
#pragma comment ( lib, "Devil/libx86/ILU.lib" )
#pragma comment ( lib, "Devil/libx86/ILUT.lib" )



ModuleImport::ModuleImport(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Importer";
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

ModuleImport::~ModuleImport()
{
}

bool ModuleImport::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

void ModuleImport::LoadFile(std::string path)
{
	if (path.find(".fbx") != std::string::npos || path.find(".FBX") != std::string::npos)
	{
		LoadMesh(path.c_str());
	}
	else if(path.find(".png") != std::string::npos || path.find(".jpg") != std::string::npos)
	{
		LoadTexture(path.c_str());
	}
}

bool ModuleImport::LoadMesh(const char* path)
{
	bool ret = true;

	if (!App->scene->root_gameobjects.empty())
	{
		App->scene->AddGameObjectToDestroy(App->scene->root_gameobjects.front());
	}

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		bool parent_created = false;
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* ai_mesh = scene->mMeshes[i];
			Mesh* mesh = new Mesh();
			mesh->num_vertices = ai_mesh->mNumVertices;
			mesh->vertices = new float[mesh->num_vertices * 3];
			memcpy(mesh->vertices, ai_mesh->mVertices, sizeof(float) * mesh->num_vertices * 3);
			CONSOLE_LOG("New mesh with %d vertices", mesh->num_vertices);

			if (ai_mesh->HasFaces())
			{
				mesh->num_indices = ai_mesh->mNumFaces * 3;
				mesh->indices = new uint[mesh->num_indices]; // assume each face is a triangle
				for (uint i = 0; i < ai_mesh->mNumFaces; ++i)
				{
					if (ai_mesh->mFaces[i].mNumIndices != 3)
					{
						CONSOLE_LOG("WARNING, geometry face with != 3 indices!");
						ret = false;
					}
					else
					{
						memcpy(&mesh->indices[i * 3], ai_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}
			}

			if (ai_mesh->HasNormals())
			{
				mesh->normals = new float[mesh->num_vertices * 3];
				memcpy(mesh->normals, ai_mesh->mNormals, sizeof(float) * mesh->num_vertices * 3);
			}

			if (ai_mesh->HasVertexColors(0))
			{
				mesh->colors = new float[mesh->num_vertices * 3];
				memcpy(mesh->colors, ai_mesh->mColors, sizeof(float) * mesh->num_vertices * 3);
			}

			if (ai_mesh->HasTextureCoords(0))
			{
				mesh->texture_coords = new float[mesh->num_vertices * 3];
				memcpy(mesh->texture_coords, ai_mesh->mTextureCoords[0], sizeof(float) * mesh->num_vertices * 3);
			}

			glGenBuffers(1, &mesh->id_vertices);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*mesh->num_vertices * 3, mesh->vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, &mesh->id_indices);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_indices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(uint)*mesh->num_indices, mesh->indices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			if(mesh->normals != nullptr)
			{
				glGenBuffers(1, &(mesh->id_normals));
				glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, mesh->normals, GL_STATIC_DRAW);
			}

			if (mesh->colors != nullptr)
			{
				glGenBuffers(1, &(mesh->id_colors));
				glBindBuffer(GL_ARRAY_BUFFER, mesh->id_colors);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, mesh->colors, GL_STATIC_DRAW);
			}

			if (mesh->texture_coords != nullptr)
			{
				glGenBuffers(1, &(mesh->id_texture_coords));
				glBindBuffer(GL_ARRAY_BUFFER, mesh->id_texture_coords);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, mesh->texture_coords, GL_STATIC_DRAW);
			}
			
			std::string name;
			if (!parent_created) parent = nullptr;
			if (scene->mNumMeshes > 1 && !parent_created)
			{
				parent = new GameObject();
				parent->SetName(GetFileName(path).c_str());
				App->scene->AddGameObjectToScene(parent);
				parent_created = true;
			}
			GameObject* go = new GameObject(parent);
			ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)go->AddComponent(Component::MeshRenderer);
			mesh_renderer->LoadMesh(mesh);
			if (parent_created)
			{
				name = parent->GetName() + "_Mesh_" + std::to_string(i);
			}
			else
			{
				name = GetFileName(path);
			}
			go->SetName(name);
			App->scene->AddGameObjectToScene(go);
		}

		aiReleaseImport(scene);
	}
	else
		CONSOLE_LOG("Error loading scene %s", path);

	return ret;
}

int ModuleImport::LoadTexture(const char * path)
{
	int ret = -1;

	ILuint image_id;
	ilGenImages(1, &image_id);
	ilBindImage(image_id);

	if (ilLoadImage(path))
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		ret = ilutGLBindTexImage();
		ilDeleteImages(1, &image_id);
	}
	else 
	{
		CONSOLE_LOG("Cannot load image %s. Error: %s", path, iluErrorString(ilGetError()));
	}

	GameObject* go = App->scene->scene_gameobjects.front();
	if (go->childs.empty())
	{
		ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)go->GetComponent(Component::MeshRenderer);

		if (mesh_renderer != nullptr)
		{
			mesh_renderer->GetMesh()->texture = ret;
			mesh_renderer->GetMesh()->texture_name = GetFileName(path);
		}
	}
	else 
	{
		for (std::list<GameObject*>::iterator it = go->childs.begin(); it != go->childs.end(); it++)
		{
			ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)(*it)->GetComponent(Component::MeshRenderer);

			if (mesh_renderer != nullptr)
			{
				mesh_renderer->GetMesh()->texture = ret;
				mesh_renderer->GetMesh()->texture_name = GetFileName(path);
			}
		}
	}

	return ret;
}

std::string ModuleImport::GetFileName(const char * path)
{
	std::string name;
	std::string temp_path;
	temp_path = path;
	uint pos_bar = temp_path.find_last_of('\\');
	uint pos_point = temp_path.find_last_of('.');
	for (int i = pos_bar + 1; i < pos_point; i++)
	{
		name.push_back(temp_path[i]);
	}
	return name;
}
