#pragma once
#include "Component.h"

class Script;
struct ScriptField;

class ComponentScript :
	public Component
{
public:
	ComponentScript(GameObject* attached_gameobject);
	~ComponentScript();

	void InitScript(std::string script_path);

	void SetScriptName(std::string name);
	void SetScriptPath(std::string path);
	void SetScript(Script* script);
	std::string GetScriptName() const;
	std::string GetScriptPath() const;
	Script* GetScript() const;
	std::vector<ScriptField*> GetScriptFields() const;
	void UpdateScriptFields();

	void Save(Data& data) const;
	void Load(Data& data);

private:
	std::string script_name;
	Script* script;
	std::string script_path;
	std::vector<ScriptField*> script_fields;
};

