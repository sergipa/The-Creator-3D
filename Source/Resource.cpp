#include "Resource.h"
#include "Application.h"

Resource::Resource()
{
	name = "";
	assets_path = "";
	library_path = "";
	creation_date = "";
	uuid = App->RandomNumber().Int();
	used_count = 0;
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

void Resource::IncreaseUsedCount()
{
	used_count++;
}

void Resource::DecreaseUsedCount()
{
	if(used_count > 0) used_count--;
}

int Resource::GetUsedCount() const
{
	return used_count;
}
