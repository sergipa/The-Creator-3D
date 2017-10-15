#include "Resource.h"



Resource::Resource()
{
	name = "";
	path = "";
	uid = 0;
}


Resource::~Resource()
{
}

std::string Resource::GetName() const
{
	return name;
}

std::string Resource::GetPath() const
{
	return path;
}

UID Resource::GetUID() const
{
	return uid;
}

void Resource::SetName(std::string name)
{
	this->name = name;
}

void Resource::SetPath(std::string path)
{
	this->path = path;
}

void Resource::SetUID(UID uid)
{
	this->uid = uid;
}
