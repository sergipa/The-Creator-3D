#include "Resource.h"
#include "Application.h"

Resource::Resource()
{
	name = "";
	assets_path = "";
	library_path = "";
	creation_date = "";
	uuid = App->RandomNumber().Int();
}

Resource::~Resource()
{
}

std::string Resource::GetName() const
{
	return name;
}

std::string Resource::GetAssetsPath() const
{
	return assets_path;
}

UID Resource::GetUID() const
{
	return uuid;
}

void Resource::SetName(std::string name)
{
	this->name = name;
}

void Resource::SetAssetsPath(std::string path)
{
	this->assets_path = path;
}

std::string Resource::GetLibraryPath() const
{
	return library_path;
}

void Resource::SetLibraryPath(std::string path)
{
	library_path = path;
}

void Resource::SetUID(UID uid)
{
	uuid = uid;
}

Resource::ResourceType Resource::GetType() const
{
	return type;
}

void Resource::SetType(ResourceType type)
{
	this->type = type;
}

std::string Resource::GetTimeCreated() const
{
	return creation_date;;
}

void Resource::SetCreatedTime(std::string time)
{
	creation_date = time;
}
