#pragma once

#include <vector>
#include <string>

class Data;

#define MAX_TAGS 1000
#define MAX_LAYERS 30

class TagsAndLayers
{
public:
	TagsAndLayers();
	~TagsAndLayers();

	void AddTag(int index, std::string tag);
	void RemoveTag(int index);
	int GetTagIndex(std::string tag_name) const;
	std::string GetTagNameByIndex(int index) const;
	void CreateEmptyTag();

	void AddLayer(int index, std::string layer);
	void RemoveLayer(int index);
	int GetLayerIndex(std::string layer_name) const;
	std::string GetLayerNameByIndex(int index) const;
	void CreateEmptyLayer();

	void Load(Data* data);
	void Save(Data* data)const;

public:
	std::vector<std::string> tags_list;
	std::vector<std::string> layers_list;
};

