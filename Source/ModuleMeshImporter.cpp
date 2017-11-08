#include "ModuleMeshImporter.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Mesh.h"
#include "ModuleFileSystem.h"
#include "ModuleCamera3D.h"
#include "ModuleTextureImporter.h"

#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/cimport.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleMeshImporter::ModuleMeshImporter(Application* app, bool start_enabled, bool is_game) : Module(app, start_enabled, is_game)
{
}

ModuleMeshImporter::~ModuleMeshImporter()
{
}

bool ModuleMeshImporter::Init(Data * editor_config)
{
	name = "Mesh_Importer";

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = &Callback;
	aiAttachLogStream(&stream);

	return true;
}

bool ModuleMeshImporter::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

bool ModuleMeshImporter::ImportMesh(std::string path)
{
	bool ret = true;

	const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{

		aiNode* root_node = scene->mRootNode;
		LoadMeshNode(nullptr, root_node, scene, path.c_str());
		CONSOLE_DEBUG("Object succesfully loaded from, %s", path);
		aiReleaseImport(scene);
	}
	else
	{
		CONSOLE_ERROR("Cannot load object from %s", path);
	}
	return ret;
}

Mesh * ModuleMeshImporter::LoadMeshFromLibrary(std::string path)
{
	Mesh* mesh = nullptr;

	std::ifstream file(path, std::ifstream::binary);
	if (file.is_open())
	{
		// get length of file:
		file.seekg(0, file.end);
		int length = file.tellg();
		file.seekg(0, file.beg);

		char * buffer = new char[length];
		file.read(buffer, length);

		if (file)
		{
			CONSOLE_DEBUG("Mesh library file %s read successfully", App->file_system->GetFileNameWithoutExtension(path.c_str()).c_str());

			char* cursor = buffer;
			mesh = new Mesh();
			// amount of indices / vertices / colors / normals / texture_coords
			uint ranges[5];
			uint bytes = sizeof(ranges);
			memcpy(ranges, cursor, bytes);
			mesh->num_indices = ranges[0];
			mesh->num_vertices = ranges[1];
			uint color_size = ranges[2];
			uint normal_size = ranges[3];
			uint uvs_size = ranges[4];
			cursor += bytes;
			// Load indices
			bytes = sizeof(uint) * mesh->num_indices;
			mesh->indices = new uint[mesh->num_indices];
			memcpy(mesh->indices, cursor, bytes);
			cursor += bytes;
			// Load vertices
			bytes = sizeof(float) * mesh->num_vertices;
			mesh->vertices = new float[mesh->num_vertices];
			memcpy(mesh->vertices, cursor, bytes);
			cursor += bytes;
			// Load colors
			if (ranges[2] != 0)
			{
				bytes = sizeof(float) * color_size;
				mesh->colors = new float[color_size];
				memcpy(mesh->colors, cursor, bytes);
				cursor += bytes;
			}
			if (ranges[3] != 0)
			{
				bytes = sizeof(float) * normal_size;
				mesh->normals = new float[normal_size];
				memcpy(mesh->normals, cursor, bytes);
				cursor += bytes;
			}
			if (ranges[4] != 0)
			{
				bytes = sizeof(float) * uvs_size;
				mesh->texture_coords = new float[uvs_size];
				memcpy(mesh->texture_coords, cursor, bytes);
				cursor += bytes;
			}
		}
		else
		{
			CONSOLE_DEBUG("Mesh library file %s read failed", App->file_system->GetFileNameWithoutExtension(path.c_str()).c_str());
		}
		file.close();
	}

	return mesh;
}

void ModuleMeshImporter::SaveMeshToLibrary(Mesh& mesh)
{
	uint ranges[5] = { mesh.num_indices, mesh.num_vertices * 3, (mesh.colors) ? mesh.num_vertices * 4 : 0, (mesh.normals) ? mesh.num_vertices * 3 : 0, (mesh.texture_coords) ? mesh.num_vertices * 3 : 0 };
	uint size = sizeof(ranges) + sizeof(uint) * ranges[0] + sizeof(float) * ranges[1];
	if (mesh.colors) size += sizeof(float) * ranges[2];
	if (mesh.normals) size += sizeof(float) * ranges[3];
	if (mesh.texture_coords) size += sizeof(float) * ranges[4];
	char* data = new char[size]; // Allocate
	char* cursor = data;
	uint bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);
	cursor += bytes; // Store indices
	bytes = sizeof(uint) * ranges[0];
	memcpy(cursor, mesh.indices, bytes);
	cursor += bytes; // Store vertex
	bytes = sizeof(float) * ranges[1];
	memcpy(cursor, mesh.vertices, bytes);
	cursor += bytes; // Store colors
	if (ranges[2] != 0)
	{
		bytes = sizeof(float) * ranges[2];
		memcpy(cursor, mesh.colors, bytes);
		cursor += bytes;
	}
	if (ranges[3] != 0) // Store normals
	{
		bytes = sizeof(float) * ranges[3];
		memcpy(cursor, mesh.normals, bytes);
		cursor += bytes;
	}
	if (ranges[4] != 0) // Store uvs
	{
		bytes = sizeof(float) * ranges[4];
		memcpy(cursor, mesh.texture_coords, bytes);
		cursor += bytes;
	}

	std::ofstream outfile((LIBRARY_MESHES_FOLDER + mesh.GetName() + ".mesh").c_str(), std::ofstream::binary);
	outfile.write(data, size);
	outfile.close();

	delete[] data;
	data = nullptr;
}

bool ModuleMeshImporter::LoadMeshNode(GameObject * parent, aiNode * node, const aiScene * scene, const char * path)
{
	bool ret = true;

	GameObject* root = nullptr;

	if (node->mNumMeshes < 1)
	{
		std::string s_node_name(node->mName.C_Str());
		aiVector3D pos;
		aiQuaternion quat;
		aiVector3D scale;
		if (s_node_name.find("$AssimpFbx$_PreRotation") != std::string::npos || s_node_name.find("$AssimpFbx$_Rotation") != std::string::npos ||
			s_node_name.find("$AssimpFbx$_PostRotation") != std::string::npos || s_node_name.find("$AssimpFbx$_Scaling") != std::string::npos ||
			s_node_name.find("$AssimpFbx$_Translation") != std::string::npos)
		{
			GetDummyTransform(*node, pos, quat, scale);
			aiVector3D n_pos;
			aiQuaternion n_quat;
			aiVector3D n_scale;
			node->mTransformation.Decompose(n_scale, n_quat, n_pos);
			pos += n_pos;
			quat = quat * n_quat;
			scale = n_scale;
		}
		else
		{
			node->mTransformation.Decompose(scale, quat, pos);
		}

		std::string root_go_name;
		(s_node_name.find("RootNode") != std::string::npos) ? root_go_name = App->file_system->GetFileNameWithoutExtension(path) : root_go_name = node->mName.C_Str();
		root = new GameObject(parent);
		if (!node->mParent) root->SetRoot(true);
		root->SetName(root_go_name);
		ComponentTransform* transform = (ComponentTransform*)root->GetComponent(Component::Transform);
		math::Quat math_quat(quat.x, quat.y, quat.z, quat.w);
		float3 rotation = math::RadToDeg(math_quat.ToEulerXYZ());
		transform->SetPosition({ pos.x, pos.y, pos.z });
		transform->SetRotation(rotation);
		transform->SetScale({ scale.x, scale.y, scale.z });
		App->scene->AddGameObjectToScene(root);

	}
	else
	{
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			if (!ret) ret = true; //If node have more than 1 mesh and last mesh returned false, we need to restart the return to true for the new mesh.

			aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[i]];
			Mesh* mesh = new Mesh();
			mesh->SetName((std::string)node->mName.C_Str());
			mesh->num_vertices = ai_mesh->mNumVertices;
			mesh->vertices = new float[mesh->num_vertices * 3];
			memcpy(mesh->vertices, ai_mesh->mVertices, sizeof(float) * mesh->num_vertices * 3);
			CONSOLE_DEBUG("New mesh ""%s"" with %d vertices", node->mName.C_Str(), mesh->num_vertices);

			if (ai_mesh->HasFaces())
			{
				mesh->num_indices = ai_mesh->mNumFaces * 3;
				mesh->indices = new uint[mesh->num_indices]; // assume each face is a triangle
				CONSOLE_DEBUG("New mesh ""%s"" with %d indices.", node->mName.C_Str(), mesh->num_indices);
				for (uint j = 0; j < ai_mesh->mNumFaces; ++j)
				{
					if (ai_mesh->mFaces[j].mNumIndices != 3)
					{
						CONSOLE_DEBUG("WARNING, geometry face %d with != 3 indices!", j);
						ret = false;
					}
					else
					{
						memcpy(&mesh->indices[j * 3], ai_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
					}
				}
				CONSOLE_DEBUG("New mesh ""%s"" with %d triangles.", node->mName.C_Str(), mesh->num_indices / 3);
			}

			if (!ret) continue;
			//Focus the camera on the mesh
			App->camera->can_update = true;
			App->camera->FocusOnObject(float3(0, 0, 0), mesh->box.Size().Length());
			App->camera->can_update = false;

			if (ai_mesh->HasNormals())
			{
				mesh->normals = new float[mesh->num_vertices * 3];
				memcpy(mesh->normals, ai_mesh->mNormals, sizeof(float) * mesh->num_vertices * 3);
				CONSOLE_DEBUG("Mesh ""%s"" has Normals", node->mName.C_Str());
			}

			if (ai_mesh->HasVertexColors(0))
			{
				mesh->colors = new float[mesh->num_vertices * 4];
				memcpy(mesh->colors, ai_mesh->mColors[0], sizeof(float) * mesh->num_vertices * 4);
				CONSOLE_DEBUG("Mesh ""%s"" has Colors", node->mName.C_Str());
			}

			if (ai_mesh->HasTextureCoords(0))
			{
				mesh->texture_coords = new float[mesh->num_vertices * 3];
				memcpy(mesh->texture_coords, ai_mesh->mTextureCoords[0], sizeof(float) * mesh->num_vertices * 3);
				CONSOLE_DEBUG("Mesh ""%s"" has UVs", node->mName.C_Str());
			}

			Texture* material_texture = nullptr;
			if (scene->HasMaterials())
			{
				aiMaterial* mat = scene->mMaterials[ai_mesh->mMaterialIndex];

				if (mat != nullptr)
				{
					aiString mat_texture_path;
					mat->GetTexture(aiTextureType_DIFFUSE, 0, &mat_texture_path);

					if (mat_texture_path.length > 0)
					{
						std::string full_texture_path;
						full_texture_path = ASSETS_TEXTURES_FOLDER + App->file_system->GetFileName(mat_texture_path.C_Str());
						if (App->texture_importer->ImportTexture(full_texture_path))
						{
							material_texture = App->texture_importer->LoadTextureFromLibrary()
						}
					}
				}
			}

			GameObject* go = new GameObject(parent);
			ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)go->AddComponent(Component::MeshRenderer);
			mesh_renderer->LoadMesh(mesh);
			mesh_renderer->LoadTexture(material_texture);
			ComponentTransform* transform = (ComponentTransform*)go->GetComponent(Component::Transform);
			aiVector3D position;
			aiQuaternion rotation_quat;
			aiVector3D scale;
			node->mTransformation.Decompose(scale, rotation_quat, position);
			math::Quat math_quat(rotation_quat.x, rotation_quat.y, rotation_quat.z, rotation_quat.w);
			float3 rotation = math::RadToDeg(math_quat.ToEulerXYZ());
			transform->SetPosition({ position.x, position.y, position.z });
			transform->SetRotation(rotation);
			transform->SetScale({ scale.x, scale.y, scale.z });
			go->SetName(mesh->GetName());
			App->scene->AddGameObjectToScene(go);
		}
		root = parent;
	}


	for (int i = 0; i < node->mNumChildren; i++)
	{
		LoadMeshNode(root, node->mChildren[i], scene, path);
	}
	return ret;
}

void ModuleMeshImporter::GetDummyTransform(aiNode & node, aiVector3D & pos, aiQuaternion & rot, aiVector3D & scale)
{
	if (node.mChildren)
	{
		std::string s_node_name(node.mName.C_Str());
		if (s_node_name.find("$AssimpFbx$_PreRotation") != std::string::npos || s_node_name.find("$AssimpFbx$_Rotation") != std::string::npos ||
			s_node_name.find("$AssimpFbx$_PostRotation") != std::string::npos || s_node_name.find("$AssimpFbx$_Scaling") != std::string::npos ||
			s_node_name.find("$AssimpFbx$_Translation") != std::string::npos)
		{
			aiVector3D node_pos;
			aiQuaternion node_quat;
			aiVector3D node_scale;
			node.mTransformation.Decompose(node_scale, node_quat, node_pos);
			pos += node_pos;
			rot = rot * node_quat;
			scale = aiVector3D(scale * node_scale);
			node = *node.mChildren[0];
			GetDummyTransform(node, pos, rot, scale);
		}
	}
}

void Callback(const char* message, char* c) {
	CONSOLE_DEBUG("%s", message);
}
