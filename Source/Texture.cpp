#include "Texture.h"
#include "OpenGL.h"
#include "Data.h"
#include "Application.h"
#include "ModuleTextureImporter.h"
#include "ModuleResources.h"
#include "ModuleFileSystem.h"
#include <ctime>

Texture::Texture()
{
	texture_id = 0;
	width = 0;
	height = 0;
	format = UnknownFormat;
	type = UnknownType;
	SetType(Resource::TextureResource);
	SetUsedCount(0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &texture_id);
	texture_id = 0;
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

void Texture::SetTextureType(TextureType type)
{
	this->type = type;
}

Texture::TextureType Texture::GetTextureType() const
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

void Texture::SetCompression(int compression_format)
{
	compression = (CompressionFormat)compression_format;
}

Texture::CompressionFormat Texture::GetCompression() const
{
	return compression;
}

std::string Texture::CompressionToString() const
{
	const char* compressions[] = { "IL_DXTC_FORMAT, IL_DXT1, IL_DXT2, IL_DXT3, IL_DXT4, IL_DXT5, IL_DXT_NO_COMP, IL_KEEP_DXTC_DATA, IL_DXTC_DATA_FORMAT, IL_3DC, IL_RXGB, IL_ATI1N, IL_DXT1A" };
	return compressions[compression];
}

std::string Texture::GetFormatString() const
{
	const char* formats[] = {"bmp", "jpg", "png", "tga", "dds", "Unknown"};
	return formats[format];
}

void Texture::Save(Data & data) const
{
	data.AddString("library_path", GetLibraryPath());
	data.AddString("assets_path", GetAssetsPath());
	data.AddString("texture_name", GetName());
	data.AddUInt("UUID", GetUID());
}

bool Texture::Load(Data & data)
{
	bool ret = true;

	std::string library_path = data.GetString("library_path");
	Texture* text = App->texture_importer->LoadTextureFromLibrary(GetLibraryPath());
	if (!text)
	{
		std::string assets_path = data.GetString("assets_path");
		if (App->file_system->FileExist(assets_path))
		{
			App->resources->CreateLibraryFile(Resource::TextureResource, assets_path);
			Load(data);
		}
		else
		{
			std::string name = data.GetString("texture_name");
			CONSOLE_ERROR("Texture %s not found!", name.c_str());
			ret = false;
		}
	}
	else
	{
		width = text->width;
		height = text->height;
		format = text->format;
		type = text->type;
		SetAssetsPath(data.GetString("assets_path"));
		SetLibraryPath(data.GetString("library_path"));
		SetName(data.GetString("texture_name"));
		SetUID(data.GetUInt("UUID"));
		SetCompression(text->compression);
	}

	return ret;
}

void Texture::CreateMeta() const
{
	time_t now = time(0);
	char* dt = ctime(&now);

	Data data;
	data.AddUInt("UUID", GetUID());
	data.AddString("Time_Created", dt);
	data.AddString("library_path", GetLibraryPath());
	data.AddString("Name", GetName());
	data.AddUInt("Width", width);
	data.AddUInt("Height", width);
	data.AddString("Type", GetTypeString());
	data.AddString("Format", GetFormatString());
	//data.AddString("Compression", CompressionToString());
	
	data.SaveAsJSON(GetAssetsPath() + ".meta");
}

void Texture::LoadToMemory()
{
	GLenum gl_format;

	switch (format)
	{
	case Texture::ColorIndex:
		gl_format = GL_COLOR_INDEX;
		break;
	case Texture::alpha:
		gl_format = GL_ALPHA;
		break;
	case Texture::rgb:
		gl_format = GL_RGB;
		break;
	case Texture::rgba:
		gl_format = GL_RGBA;
		break;
	case Texture::bgr:
		gl_format = GL_BGR;
		break;
	case Texture::bgra:
		gl_format = GL_BGRA;
		break;
	case Texture::luminance:
		gl_format = GL_LUMINANCE;
		break;
	case Texture::luminance_alpha:
		gl_format = GL_LUMINANCE_ALPHA;
		break;
	case Texture::UnknownFormat:
		gl_format = GL_ZERO;
		break;
	}

	// create a texture object
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, gl_format, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::UnloadFromMemory()
{
	glDeleteTextures(1, &texture_id);
	texture_id = 0;
}
