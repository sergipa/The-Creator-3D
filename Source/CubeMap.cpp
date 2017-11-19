#include "CubeMap.h"
#include "Application.h"
#include "ModuleTextureImporter.h"
#include "OpenGL.h"
#include "Primitive.h"
#include "Texture.h"

CubeMap::CubeMap(float size)
{
	textures[0] = textures[1] = textures[2] = textures[3] = textures[4] = textures[5] = nullptr;
	cube_map_id = 0;

	cube = new pTexturedCube(size, size, size);
}

CubeMap::~CubeMap()
{
	DeleteCubeMap();
}

void CubeMap::CreateCubeMap()
{
	/*glGenTextures(1, &cube_map_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map_id);*/

	for (int i = 0; i < 6; i++)
	{
		textures[i] = App->texture_importer->LoadTextureFromLibrary(textures_path[i]);

		/*glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, textures[i]->GetWidth(), textures[i]->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, textures[i]->GetImageData());
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);*/
	}

	cube->SetTextures(textures);
}

void CubeMap::RenderCubeMap(float3 position)
{
	//Immediate draw until shaders are done
	cube->SetPos(position);
	cube->Render();
}

void CubeMap::DeleteCubeMap()
{
	/*for (int i = 0; i < 6; i++)
	{
		glDeleteTextures(6, )
	}*/
	delete cube;
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
