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

	void InitScript();
	void StartScript();
	void UpdateScript();

	void SetScript(Script* script);
	std::string GetScriptName() const;
	Script* GetScript() const;
	std::vector<ScriptField*> GetScriptFields() const;
	void UpdateScriptFields();

	void Save(Data& data) const;
	void Load(Data& data);

private:
	Script* script;
	std::vector<ScriptField*> script_fields;
};

