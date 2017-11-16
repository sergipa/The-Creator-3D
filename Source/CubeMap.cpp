#include "CubeMap.h"
#include "Application.h"
#include "ModuleResources.h"
#include "ModuleFileSystem.h"
#include "OpenGL.h"

CubeMap::CubeMap()
{
	texture[0] = texture[1] = texture[2] = texture[3] = texture[4] = texture[5] = nullptr;
	cube_map_id = 0;
}

CubeMap::~CubeMap()
{
	DeleteCubeMap();
}

void CubeMap::CreateCubeMap()
{
	glGenTextures(1, &cube_map_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map_id);

	for (int i = 0; i < 6; i++)
	{
		texture[i] = App->resources->GetTexture(App->file_system->GetFileNameWithoutExtension(textures_path[i]));
		
		if(texture[i] == nullptr)
		{
			texture[i] = (Texture*)App->resources->CreateResource(textures_path[i]);
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, texture[i]->GetWidth(), texture[i]->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture[i]->GetImageData());
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}

void CubeMap::RenderCubeMap()
{
}

void CubeMap::DeleteCubeMap()
{
}

void CubeMap::SetCubeMapTopTexture(std::string texture_path)
{
	textures_path[0] = texture_path;
}

void CubeMap::SetCubeMapLeftTexture(std::string texture_path)
{
	textures_path[1] = texture_path;
}

void CubeMap::SetCubeMapFrontTexture(std::string texture_path)
{
	textures_path[2] = texture_path;
}

void CubeMap::SetCubeMapRightTexture(std::string texture_path)
{
	textures_path[3] = texture_path;
}

void CubeMap::SetCubeMapBackTexture(std::string texture_path)
{
	textures_path[4] = texture_path;
}

void CubeMap::SetCubeMapBottomTexture(std::string texture_path)
{
	textures_path[5] = texture_path;
}
