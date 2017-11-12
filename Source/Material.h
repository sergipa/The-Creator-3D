#pragma once
#include "Resource.h"
#include "Color.h"
#include <vector>

class Texture;

class Material :
	public Resource
{
public:
	Material();
	~Material();

	void Save(Data& data) const;
	bool Load(Data& data);
	void CreateMeta() const;
	void LoadToMemory();
	void UnloadFromMemory();

	void SetDiffuseTexture(Texture* diffuse);
	void SetSpecularTexture(Texture* specular);
	void SetAmbientTexture(Texture* ambient);
	void SetEmissiveTexture(Texture* emissive);
	void SetHeightMapTexture(Texture* heightmap);
	void SetNormalMapTexture(Texture* normalmap);
	void SetShininessTexture(Texture* shininess);
	void SetOpacityTexture(Texture* opacity);
	void SetDisplacementTexture(Texture* displacement);
	void SetLightMapTexture(Texture* ligthmap);
	void SetReflectionTexture(Texture* refection);

	Texture* GetDiffuseTexture(int index) const;
	Texture* GetSpecularTexture(int index) const;
	Texture* GetAmbientTexture(int index) const;
	Texture* GetEmissiveTexture(int index) const;
	Texture* GetHeightMapTexture(int index) const;
	Texture* GetNormalMapTexture(int index) const;
	Texture* GetShininessTexture(int index) const;
	Texture* GetOpacityTexture(int index) const;
	Texture* GetDisplacementTexture(int index) const;
	Texture* GetLightMapTexture(int index) const;
	Texture* GetReflectionTexture(int index) const;

	std::vector<Texture*> GetDiffuseTextureList() const;
	std::vector<Texture*> GetSpecularTextureList() const;
	std::vector<Texture*> GetAmbientTextureList() const;
	std::vector<Texture*> GetEmissiveTextureList() const;
	std::vector<Texture*> GetHeightMapTextureList() const;
	std::vector<Texture*> GetNormalMapTextureList() const;
	std::vector<Texture*> GetShininessTextureList() const;
	std::vector<Texture*> GetOpacityTextureList() const;
	std::vector<Texture*> GetDisplacementTextureList() const;
	std::vector<Texture*> GetLightMapTextureList() const;
	std::vector<Texture*> GetReflectionTextureList() const;

	void SetDiffuseColor(float r, float g, float b);
	void SetSpecularColor(float r, float g, float b);
	void SetAmbientColor(float r, float g, float b);
	void SetEmissiveColor(float r, float g, float b);
	void SetTransparentColor(float r, float g, float b);
	void SetReflectiveColor(float r, float g, float b);

	Color GetDiffuseColor() const;
	Color GetSpecularColor() const;
	Color GetAmbientColor() const;
	Color GetEmissiveColor() const;
	Color GetTransparentColor() const;
	Color GetReflectiveColor() const;

	void SetWireframe(bool wireframe);
	bool IsWireFrame() const;

	void SetTwoSided(bool two_sided);
	bool IsTwoSided() const;
	
	void SetShadingModel(int shading_model);
	int GetShadingModel() const;

	void SetBlendMode(int blend_mode);
	int GetBlendMode() const;

	void SetOpacity(float opacity);
	float GetOpacity() const;

	void SetShininess(float shininess);
	float GetShininess() const;

	void SetShininessStrength(float shininess_strength);
	float GetShininessStrength() const;

	void SetRefraction(float refraction);
	float GetRefraction() const;

	void SetReflectivity(float reflectivity);
	float GetReflectivity() const;

	void SetBumpScaling(float bump_scaling);
	float GetBumpScaling() const;

private:

	std::vector<Texture*> diffuse_texture_list;
	std::vector<Texture*> specular_texture_list;
	std::vector<Texture*> ambient_texture_list;
	std::vector<Texture*> emissive_texture_list;
	std::vector<Texture*> heightmap_texture_list;
	std::vector<Texture*> normalmap_texture_list;
	std::vector<Texture*> shininess_texture_list;
	std::vector<Texture*> opacity_texture_list;
	std::vector<Texture*> displacement_texture_list;
	std::vector<Texture*> lightmap_texture_list;
	std::vector<Texture*> reflection_texture_list;

	Color diffuse_color;
	Color specular_color;
	Color ambient_color;
	Color emissive_color;
	Color transparent_color;
	Color reflective_color;

	//Specifies whether wireframe rendering must be turned on for the material.
	bool wireframe;
	//Specifies whether meshes using this material must be rendered without backface culling.
	bool two_sided;
	//Defines the library shading model to use for (real time) rendering to approximate the original look of the material as closely as possible.
	//i.e: Blinn, Toon, Flat, Phong... 
	int shading_model;
	//Defines how the final color value in the screen buffer is computed from the given color at that position and the newly computed color from the material.
	//Simply said, alpha blending settings.
	int blend_mode;
	//Defines the opacity of the material in a range between 0..1.	
	float opacity;
	//Defines the shininess of a phong-shaded material. This is actually the exponent of the phong specular equation	
	float shininess;
	//Scales the specular color of the material.
	float shininess_strength;
	//Defines the Index Of Refraction for the material. That's not supported by most file formats.
	float refraction;
	float reflectivity;
	float bump_scaling;

};

