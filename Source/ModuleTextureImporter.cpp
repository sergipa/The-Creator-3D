#include "ModuleTextureImporter.h"
#include "Texture.h"
#include "Application.h"
#include "ModuleFileSystem.h"

#include "Devil/include/il.h"
#include "Devil/include/ilut.h"
#pragma comment ( lib, "Devil/libx86/DevIL.lib" )
#pragma comment ( lib, "Devil/libx86/ILU.lib" )
#pragma comment ( lib, "Devil/libx86/ILUT.lib" )

ModuleTextureImporter::ModuleTextureImporter(Application* app, bool start_enabled, bool is_game) : Module(app, start_enabled, is_game)
{
	name = "Texture_Importer";

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

ModuleTextureImporter::~ModuleTextureImporter()
{
}

bool ModuleTextureImporter::Init(Data * editor_config)
{
	return true;
}

bool ModuleTextureImporter::CleanUp()
{
	ilShutDown();
	return true;
}

std::string ModuleTextureImporter::ImportTexture(std::string path)
{
	std::string ret;

	ILuint image_id;
	ilGenImages(1, &image_id);
	ilBindImage(image_id);

	if (ilLoadImage(path.c_str()))
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		ilEnable(IL_FILE_OVERWRITE);

		//save texture library
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
		if (ilSave(IL_DDS, (LIBRARY_TEXTURES_FOLDER + App->file_system->GetFileNameWithoutExtension(path) + ".dds").c_str()))
		{
			CONSOLE_DEBUG("%s library file created.", App->file_system->GetFileNameWithoutExtension(path).c_str());
			ret = LIBRARY_TEXTURES_FOLDER + App->file_system->GetFileNameWithoutExtension(path) + ".dds";
		}
		else
		{
			CONSOLE_DEBUG("%s library file cannot be created.", App->file_system->GetFileNameWithoutExtension(path).c_str());
		}

		ilDeleteImages(1, &image_id);
		CONSOLE_DEBUG("Texture Loaded: %s", path);
	}
	else
	{
		CONSOLE_DEBUG("Cannot load image %s. Error: %s", path, iluErrorString(ilGetError()));
	}

	return ret;
}

Texture * ModuleTextureImporter::LoadTextureFromLibrary(std::string path)
{
	ILuint image_id;
	ilGenImages(1, &image_id);
	ilBindImage(image_id);

	Texture* tmp_texture = nullptr;

	if (ilLoadImage(path.c_str()))
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		tmp_texture = new Texture();
		tmp_texture->SetID(ilutGLBindTexImage());
		tmp_texture->SetWidth(ImageInfo.Width);
		tmp_texture->SetHeight(ImageInfo.Height);
		tmp_texture->SetLibraryPath(path);
		tmp_texture->SetName(App->file_system->GetFileNameWithoutExtension(path).c_str());
		tmp_texture->SetCompression(ilGetInteger(IL_DXTC_FORMAT));
		//tmp_texture->SetAssetsPath(App->file_system->GetFilePathInDirectory(App->file_system->GetFileName()))

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
		case IL_BMP: tmp_texture->SetTextureType(Texture::TextureType::bmp); break;
		case IL_JPG: tmp_texture->SetTextureType(Texture::TextureType::jpg); break;
		case IL_PNG: tmp_texture->SetTextureType(Texture::TextureType::png); break;
		case IL_TGA: tmp_texture->SetTextureType(Texture::TextureType::tga); break;
		case IL_DDS: tmp_texture->SetTextureType(Texture::TextureType::dds); break;
		default: tmp_texture->SetTextureType(Texture::TextureType::UnknownType); break;
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

