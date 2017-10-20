#pragma once

#include <string>
#include "Globals.h"

class Resource
{
public:

	enum ResourceType {
		TextureResource, MeshResource, SceneResource, AnimationResource, PrefabResource, ScriptResource, 
		AudioResource, ParticleFXResource, FontResource, Unknown
	};

	Resource();
	virtual ~Resource();

	std::string GetName() const;
	std::string GetPath() const;
	UID GetUID() const;
	void SetUID(UID uid);
	void SetName(std::string name);
	void SetPath(std::string path);
	ResourceType GetType() const;
	void SetType(ResourceType type);

private:
	std::string name;
	std::string path;
	UID uuid;
	ResourceType type;
};

