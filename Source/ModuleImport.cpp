#include "ModuleImport.h"
#include "GameObject.h"
#include "ComponentMeshRenderer.h"
#include "Component.h"
#include "Application.h"
#include "ModuleScene.h"
#include "OpenGL.h"
#include <string>

ModuleImport::ModuleImport(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Importer";
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}

ModuleImport::~ModuleImport()
{
}

bool ModuleImport::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

bool ModuleImport::LoadMesh(const char* path)
{
	bool ret = true;
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

			glGenBuffers(1, &mesh->id_vertices);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*mesh->num_vertices * 3, mesh->vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, &mesh->id_indices);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_indices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(uint)*mesh->num_indices, mesh->indices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			
			std::string name;
			if (!parent_created) parent = nullptr;
			if (scene->mNumMeshes > 1 && !parent_created)
			{
				parent = new GameObject();
				std::string temp_path;
				temp_path = path;
				uint pos_bar = temp_path.find_last_of('\\');
				uint pos_point = temp_path.find_last_of('.');
				std::string name;
				for (int i = pos_bar + 1; i < pos_point; i++)
				{
					name.push_back(temp_path[i]);
				}
				parent->SetName(name.c_str());
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
				std::string temp_path;
				temp_path = path;
				uint pos_bar = temp_path.find_last_of('\\');
				uint pos_point = temp_path.find_last_of('.');
				for (int i = pos_bar + 1; i < pos_point; i++)
				{
					name.push_back(temp_path[i]);
				}
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
