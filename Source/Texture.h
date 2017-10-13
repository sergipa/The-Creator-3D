#pragma once

typedef unsigned int uint;

#include <string>

class Texture
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
	~Texture();

	void SetID(uint id);
	uint GetID() const;
	void SetWidth(uint width);
	uint GetWidth() const;
	void SetHeight(uint height);
	uint GetHeight() const;
	void SetSize(uint width, uint height);
	void GetSize(uint& width, uint& height) const;
	void SetPath(const char* path);
	std::string GetPath() const;
	void SetName(const char* name);
	std::string GetName() const;
	void SetType(TextureType type);
	TextureType GetType() const;
	std::string GetTypeString() const;
	void SetFormat(TextureFormat format);
	TextureFormat GetFormat() const;
	std::string GetFormatString() const;

private:
	uint texture_id;
	uint width;
	uint height;
	std::string path;
	std::string name;
	TextureFormat format;
	TextureType type;
};

