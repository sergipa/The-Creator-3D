#pragma once

#include <string>
#include "Globals.h"

class Data;

class Resource
{
public:

	enum ResourceType {
		TextureResource, MeshResource, SceneResource, AnimationResource, PrefabResource, ScriptResource, 
		AudioResource, ParticleFXResource, FontResource, RenderTexture, Unknown
	};

	Resource();
	virtual ~Resource();

	UID GetUID() const;
	void SetUID(UID uid);
	std::string GetName() const;
	void SetName(std::string name);
	std::string GetAssetsPath() const;
	void SetAssetsPath(std::string path);
	std::string GetLibraryPath() const;
	void SetLibraryPath(std::string path);
	ResourceType GetType() const;
	void SetType(ResourceType type);
	std::string GetTimeCreated() const;
	void SetCreatedTime(std::string time);

	virtual void Save(Data& data) const = 0;
	virtual void Load(Data& data) = 0;
	virtual void CreateMeta() const = 0;

private:
	std::string name;
	std::string assets_path;
	std::string library_path;
	UID uuid;
	ResourceType type;
	std::string creation_date;
};

