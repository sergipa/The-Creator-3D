#pragma once

#include <string>
#include "Resource.h"

class Data;

class Texture : public Resource
{
public:

	enum TextureType
	{
		bmp, jpg, png, tga, dds, UnknownType,
	};
	enum TextureFormat
	{
		ColorIndex, alpha, rgb, rgba, bgr, bgra, luminance, luminance_alpha, UnknownFormat
	};

	Texture();
	virtual ~Texture();

	void SetID(uint id);
	uint GetID() const;
	void SetWidth(uint width);
	uint GetWidth() const;
	void SetHeight(uint height);
	uint GetHeight() const;
	void SetSize(uint width, uint height);
	void GetSize(uint& width, uint& height) const;
	void SetTextureType(TextureType type);
	TextureType GetTextureType() const;
	std::string GetTypeString() const;
	void SetFormat(TextureFormat format);
	TextureFormat GetFormat() const;
	std::string GetFormatString() const;

	void Save(Data& data) const;
	void Load(Data& data);
	void CreateMeta() const;

private:
	uint texture_id;
	uint width;
	uint height;
	TextureFormat format;
	TextureType type;
};

