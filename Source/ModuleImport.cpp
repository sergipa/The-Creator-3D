#include "ModuleImport.h"
#include "GameObject.h"
#include "ComponentMeshRenderer.h"
#include "ComponentTransform.h"
#include "Component.h"
#include "Application.h"
#include "ModuleScene.h"
#include "OpenGL.h"
#include "Mesh.h"
#include "ModuleCamera3D.h"
#include "ModuleResources.h"
#include "Data.h"
#include "ImportWindow.h"

#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/cimport.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "Devil/include/il.h"
#include "Devil/include/ilut.h"
#pragma comment ( lib, "Devil/libx86/DevIL.lib" )
#pragma comment ( lib, "Devil/libx86/ILU.lib" )
#pragma comment ( lib, "Devil/libx86/ILUT.lib" )



ModuleImport::ModuleImport(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Importer";

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = &Callback;
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
	ilShutDown();
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
		LoadTexture(path.c_str(), true); //<-- Right now, we will use the textures only to attach them to gameobjects (Assigment1)
	}
}

bool ModuleImport::LoadMesh(const char* path)
{
	bool ret = true;

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		aiNode* root_node = scene->mRootNode;
		LoadMeshNode(nullptr, root_node, scene, path);
		CONSOLE_DEBUG("Object succesfully loaded from, %s", path);
		aiReleaseImport(scene);
	}
	else
	{
		CONSOLE_ERROR("Cannot load object from %s", path);
	}
	return ret;
}

bool ModuleImport::LoadMeshNode(GameObject * parent, aiNode * node, const aiScene * scene, const char* path)
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
		}

		std::string root_go_name;
		(s_node_name.find("RootNode") != std::string::npos) ? root_go_name = GetFileName(path) : root_go_name = node->mName.C_Str();
		root = new GameObject(parent);
		if (!node->mParent) root->SetRoot(true);
		root->SetName(root_go_name);
		ComponentTransform* transform = (ComponentTransform*)root->GetComponent(Component::Transform);
		aiVector3D rotation = quat.GetEuler();
		rotation *= RADTODEG;
		transform->SetPosition({ pos.x, pos.y, pos.z });
		transform->SetRotation({ rotation.z, rotation.y, rotation.x });
		transform->SetScale({ scale.x, scale.y, scale.z });
		App->scene->AddGameObjectToScene(root);
			
	}
	else
	{
		for (int i = 0; i < node->mNumMeshes; i++)
		{
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
				CONSOLE_DEBUG("New mesh ""%s"" with %d triangles.", node->mName.C_Str(), mesh->num_indices /3);
			}
			
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

			glGenBuffers(1, &mesh->id_vertices);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*mesh->num_vertices * 3, mesh->vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, &mesh->id_indices);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_indices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(uint)*mesh->num_indices, mesh->indices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			if (mesh->normals != nullptr)
			{
				glGenBuffers(1, &(mesh->id_normals));
				glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, mesh->normals, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			if (mesh->colors != nullptr)
			{
				glGenBuffers(1, &(mesh->id_colors));
				glBindBuffer(GL_ARRAY_BUFFER, mesh->id_colors);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 4, mesh->colors, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			if (mesh->texture_coords != nullptr)
			{
				glGenBuffers(1, &(mesh->id_texture_coords));
				glBindBuffer(GL_ARRAY_BUFFER, mesh->id_texture_coords);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, mesh->texture_coords, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
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
						full_texture_path = ASSETS_TEXTURES_FOLDER + GetFileName(mat_texture_path.C_Str()) + GetFileExtension(mat_texture_path.C_Str());
						material_texture = LoadTexture(full_texture_path.c_str(), false);
					}
				}
			}

			uint ranges[5] = { mesh->num_indices, mesh->num_vertices * 3, (mesh->colors) ? mesh->num_vertices * 4 : 0, (mesh->normals) ? mesh->num_vertices * 3 : 0, (mesh->texture_coords) ? mesh->num_vertices * 3 : 0 };
			uint size = sizeof(ranges) + sizeof(uint) * ranges[0] + sizeof(float) * ranges[1];
			if (mesh->colors) size += sizeof(float) * ranges[2];
			if (mesh->normals) size += sizeof(float) * ranges[3];
			if (mesh->texture_coords) size += sizeof(float) * ranges[4];
			char* data = new char[size]; // Allocate
			char* cursor = data;
			uint bytes = sizeof(ranges); // First store ranges
			memcpy(cursor, ranges, bytes);
			cursor += bytes; // Store indices
			bytes = sizeof(uint) * ranges[0];
			memcpy(cursor, mesh->indices, bytes);
			cursor += bytes; // Store vertex
			bytes = sizeof(float) * ranges[1];
			memcpy(cursor, mesh->vertices, bytes);
			cursor += bytes; // Store colors
			if (ranges[2] != 0)
			{
				bytes = sizeof(float) * ranges[2];
				memcpy(cursor, mesh->colors, bytes);
				cursor += bytes;
			}
			if (ranges[3] != 0) // Store normals
			{
				bytes = sizeof(float) * ranges[3];
				memcpy(cursor, mesh->normals, bytes);
				cursor += bytes;
			}
			if (ranges[4] != 0) // Store uvs
			{
				bytes = sizeof(float) * ranges[4];
				memcpy(cursor, mesh->texture_coords, bytes);
				cursor += bytes;
			}
			
			std::ofstream outfile((LIBRARY_MESHES_FOLDER + mesh->GetName() + ".mesh").c_str(), std::ofstream::binary);
			outfile.write(data, size);
			outfile.close();

			delete[] data;
			data = nullptr;

			LoadMeshFromLibrary((LIBRARY_MESHES_FOLDER + mesh->GetName() + ".mesh"));

			App->resources->AddMesh(mesh);

			GameObject* go = new GameObject(parent);
			ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)go->AddComponent(Component::MeshRenderer);
			mesh_renderer->LoadMesh(mesh);
			mesh_renderer->LoadTexture(material_texture);
			ComponentTransform* transform = (ComponentTransform*)go->GetComponent(Component::Transform);
			aiVector3D position;
			aiQuaternion rotation_quat;
			aiVector3D scale;
			node->mTransformation.Decompose(scale, rotation_quat, position);
			aiVector3D rotation = rotation_quat.GetEuler();
			rotation *= RADTODEG;
			transform->SetPosition({ position.x, position.y, position.z });
			transform->SetRotation({ rotation.x, rotation.z, rotation.y });
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

Texture* ModuleImport::LoadTexture(const char * path, bool attach_to_gameobject)
{
	//If gameobjects are not selected, don't load a texture. This is just for the assigment 1.
	//In the future if we drag a image without selected gameobjects, a texture should be created and stored in assets/library
	if (App->scene->selected_gameobjects.empty() && attach_to_gameobject) return nullptr;

	//If texture exist. Don't load a new one
	if (App->resources->TextureExist(GetFileName(path)))
	{
		Texture* existing_texture = App->resources->GetTexture(GetFileName(path));
		if(attach_to_gameobject) App->scene->ApplyTextureToSelectedGameObjects(existing_texture);
		return existing_texture;
	}

	ILuint image_id;
	ilGenImages(1, &image_id);
	ilBindImage(image_id);

	Texture* tmp_texture = nullptr;

	if (ilLoadImage(path))
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		tmp_texture = new Texture();
		tmp_texture->SetID(ilutGLBindTexImage());
		tmp_texture->SetWidth(ImageInfo.Width);
		tmp_texture->SetHeight(ImageInfo.Height);
		tmp_texture->SetPath(path);
		tmp_texture->SetName(GetFileName(path).c_str());

		switch (ImageInfo.Format)
		{
			case IL_COLOUR_INDEX: tmp_texture->SetFormat(Texture::ColorIndex); break;
			case IL_ALPHA: tmp_texture->SetFormat(Texture::alpha); break;
			case IL_RGB: tmp_texture->SetFormat(Texture::rgb); break;
			case IL_RGBA: tmp_texture->SetFormat(Texture::rgba); break;
			case IL_BGR: tmp_texture->SetFormat(Texture::bgr); break;
			case IL_BGRA: tmp_texture->SetFormat(Texture::bgra); break;
			case IL_LUMINANCE: tmp_texture->SetFormat(Texture::luminance); break;
			case IL_LUMINANCE_ALPHA: tmp_texture->SetFormat(Texture::luminance_alpha); break;
			default: tmp_texture->SetFormat(Texture::UnknownFormat); break;
		}
		switch (ImageInfo.Type)
		{
			case IL_BMP: tmp_texture->SetType(Texture::TextureType::bmp); break;
			case IL_JPG: tmp_texture->SetType(Texture::TextureType::jpg); break;
			case IL_PNG: tmp_texture->SetType(Texture::TextureType::png); break;
			case IL_TGA: tmp_texture->SetType(Texture::TextureType::tga); break;
			case IL_DDS: tmp_texture->SetType(Texture::TextureType::dds); break;
			default: tmp_texture->SetType(Texture::TextureType::UnknownType); break;
		}

		//save texture library
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
		if (ilSave(IL_DDS, (LIBRARY_TEXTURES_FOLDER + GetFileName(path) + ".dds").c_str()))
		{
			CONSOLE_DEBUG("%s library file created.", GetFileName(path).c_str());
		}
		else 
		{
			CONSOLE_DEBUG("%s library file cannot be created.", GetFileName(path).c_str());
		}

		ilDeleteImages(1, &image_id);
		CONSOLE_DEBUG("Texture Loaded: %s", path);
		App->resources->AddTexture(tmp_texture);
	}
	else 
	{
		CONSOLE_DEBUG("Cannot load image %s. Error: %s", path, iluErrorString(ilGetError()));
	}

	if (attach_to_gameobject) App->scene->ApplyTextureToSelectedGameObjects(tmp_texture);

	return tmp_texture;
}

Texture * ModuleImport::LoadEngineImages(const char * path)
{
	ILuint image_id;
	ilGenImages(1, &image_id);
	ilBindImage(image_id);

	Texture* tmp_texture = nullptr;

	if (ilLoadImage(path))
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		tmp_texture = new Texture();
		tmp_texture->SetID(ilutGLBindTexImage());
		tmp_texture->SetWidth(ImageInfo.Width);
		tmp_texture->SetHeight(ImageInfo.Height);
		tmp_texture->SetPath(path);
		tmp_texture->SetName(GetFileName(path).c_str());

		switch (ImageInfo.Format)
		{
		case IL_COLOUR_INDEX: tmp_texture->SetFormat(Texture::ColorIndex); break;
		case IL_ALPHA: tmp_texture->SetFormat(Texture::alpha); break;
		case IL_RGB: tmp_texture->SetFormat(Texture::rgb); break;
		case IL_RGBA: tmp_texture->SetFormat(Texture::rgba); break;
		case IL_BGR: tmp_texture->SetFormat(Texture::bgr); break;
		case IL_BGRA: tmp_texture->SetFormat(Texture::bgra); break;
		case IL_LUMINANCE: tmp_texture->SetFormat(Texture::luminance); break;
		case IL_LUMINANCE_ALPHA: tmp_texture->SetFormat(Texture::luminance_alpha); break;
		default: tmp_texture->SetFormat(Texture::UnknownFormat); break;
		}
		switch (ImageInfo.Type)
		{
		case IL_BMP: tmp_texture->SetType(Texture::TextureType::bmp); break;
		case IL_JPG: tmp_texture->SetType(Texture::TextureType::jpg); break;
		case IL_PNG: tmp_texture->SetType(Texture::TextureType::png); break;
		case IL_TGA: tmp_texture->SetType(Texture::TextureType::tga); break;
		case IL_DDS: tmp_texture->SetType(Texture::TextureType::dds); break;
		default: tmp_texture->SetType(Texture::TextureType::UnknownType); break;
		}
		ilDeleteImages(1, &image_id);
		CONSOLE_DEBUG("Engine image Loaded: %s", path);
	}
	else
	{
		CONSOLE_DEBUG("Cannot load image %s. Error: %s", path, iluErrorString(ilGetError()));
	}

	return tmp_texture;
}

Mesh * ModuleImport::LoadMeshFromLibrary(std::string path)
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
			CONSOLE_DEBUG("Mesh library file %s read successfully", GetFileName(path.c_str()).c_str());

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
			CONSOLE_DEBUG("Mesh library file %s read failed", GetFileName(path.c_str()).c_str());
		}
		file.close();
	}

	return mesh;
}

std::string ModuleImport::GetFileName(const char * path)
{
	std::string name;
	std::string temp_path;
	temp_path = path;
	uint pos_bar = (temp_path.find(ASSETS_FOLDER) != std::string::npos) ? temp_path.find_last_of('/') : temp_path.find_last_of('\\');
	uint pos_point = temp_path.find_last_of('.');
	for (int i = pos_bar + 1; i < pos_point; i++)
	{
		name.push_back(temp_path[i]);
	}
	return name;
}

std::string ModuleImport::GetFileExtension(const char * path)
{
	std::string name;
	std::string temp_path;
	temp_path = path;
	uint pos_point = temp_path.find_last_of('.');
	for (int i = pos_point; i < temp_path.length(); i++)
	{
		name.push_back(temp_path[i]);
	}
	return name;
}

void ModuleImport::GetDummyTransform(aiNode& node, aiVector3D& pos, aiQuaternion& rot, aiVector3D& scale)
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