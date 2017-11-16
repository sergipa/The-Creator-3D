#pragma once

#include "Texture.h"
#include <string>

class CubeMap
{
public:
	CubeMap();
	~CubeMap();

	void CreateCubeMap();
	void RenderCubeMap();
	void DeleteCubeMap();

	void SetCubeMapTopTexture(std::string texture_path);
	void SetCubeMapLeftTexture(std::string texture_path);
	void SetCubeMapFrontTexture(std::string texture_path);
	void SetCubeMapRightTexture(std::string texture_path);
	void SetCubeMapBackTexture(std::string texture_path);
	void SetCubeMapBottomTexture(std::string texture_path);

private:
	Texture* texture[6];
	std::string textures_path[6];
	uint cube_map_id;
	/*top_texture_path;
	std::string left_texture_path;
	std::string front_texture_path;
	std::string right_texture_path;
	std::string back_texture_path;
	std::string bottom_texture_path;*/
};

