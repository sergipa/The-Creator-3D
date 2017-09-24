#pragma once

#include "Globals.h"
#include "Vector2.h"
#include "Vector3.h"
#include <fstream>
#include <iostream>
#include "cereal-1.2.2\include\cereal\archives\xml.hpp"
#include "cereal-1.2.2\include\cereal\archives\json.hpp"
#include "cereal-1.2.2\include\cereal\archives\binary.hpp"
#include "cereal-1.2.2\include\cereal\types\string.hpp"
#include "cereal-1.2.2\include\cereal\types\vector.hpp"

class Data
{
public:
	Data();
	~Data();

	void SaveAsXML(std::string path);
	bool LoadXML(std::string path);
	void SaveAsJSON(std::string path);
	bool LoadJSON(std::string path);
	void SaveAsBinary(std::string path);
	bool LoadBinary(std::string path);

	void ClearData();

	void CreateSection(std::string sectionName);
	void CloseSection();
	bool EnterSection(std::string sectionName);
	void LeaveSection();

	bool GetBool(std::string valueName);
	int GetInt(std::string valueName);
	uint GetUInt(std::string valueName);
	double GetDouble(std::string valueName);
	float GetFloat(std::string valueName);
	std::string GetString(std::string valueName);
	Vector2f GetVector2Float(std::string valueName);
	Vector3f GetVector3Float(std::string valueName);
	Vector2i GetVector2Int(std::string valueName);
	Vector3i GetVector3Int(std::string valueName);

	void AddBool(std::string valueName, bool value);
	void AddInt(std::string valueName, int value);
	void AddUInt(std::string valueName, uint value);
	void AddDouble(std::string valueName, double value);
	void AddFloat(std::string valueName, float value);
	void AddString(std::string valueName, std::string value);
	void AddVector2Float(std::string valueName, Vector2f value);
	void AddVector3Float(std::string valueName, Vector3f value);
	void AddVector2Int(std::string valueName, Vector2i value);
	void AddVector3Int(std::string valueName, Vector3i);

private:

	friend class cereal::access;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(data_names, data_values);
	}

private:
	std::vector<std::string> data_names;
	std::vector<std::string> data_values;

	std::vector<std::string> in_section_names;
	std::vector<std::string> in_section_values;
	int sections_open = 0;
	int current_index = 0;
	std::list<int> last_index;
	std::list<std::string> last_index_name;

	std::vector<std::string> out_section_names;
	std::vector<std::string> out_section_values;

	bool getting_from_section = false;
};

