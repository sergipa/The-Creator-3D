#pragma once

#include <string>
#include "Globals.h"

class Resource
{
public:
	Resource();
	~Resource();

	std::string GetName() const;
	std::string GetPath() const;
	UID GetUID() const;
	void SetName(std::string name);
	void SetPath(std::string path);
	void SetUID(UID uid);

private:
	std::string name;
	std::string path;
	UID uid;
};

