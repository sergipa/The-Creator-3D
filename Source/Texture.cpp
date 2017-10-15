#include "Texture.h"


Texture::Texture()
{
	texture_id = 0;
	width = 0;
	height = 0;
	format = UnknownFormat;
	type = UnknownType;
}

Texture::~Texture()
{
}

void Texture::SetID(uint id)
{
	texture_id = id;
}

uint Texture::GetID() const
{
	return texture_id;
}

void Texture::SetWidth(uint width)
{
	this->width = width;
}

uint Texture::GetWidth() const
{
	return width;
}

void Texture::SetHeight(uint height)
{
	this->height = height;
}

uint Texture::GetHeight() const
{
	return height;
}

void Texture::SetSize(uint width, uint height)
{
	this->width = width;
	this->height = height;
}

void Texture::GetSize(uint & width, uint & height) const
{
	width = this->width;
	height = this->height;
}

void Texture::SetType(TextureType type)
{
	this->type = type;
}

Texture::TextureType Texture::GetType() const
{
	return type;
}

std::string Texture::GetTypeString() const
{
	const char* types[] = {"Color Index", "Alpha", "rgb", "rgba", "bgr", "bgra", "Luminance", "Luminance Alpha", "Unknown" };
	return types[type];
}

void Texture::SetFormat(TextureFormat format)
{
	this->format = format;
}

Texture::TextureFormat Texture::GetFormat() const
{
	return format;
}

std::string Texture::GetFormatString() const
{
	const char* formats[] = {"bmp", "jpg", "png", "tga", "dds", "Unknown"};
	return formats[format];
}
