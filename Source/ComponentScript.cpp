#include "ComponentScript.h"
#include "CSScript.h"


ComponentScript::ComponentScript(GameObject* attached_gameobject)
{
	SetActive(true);
	SetName("Script");
	SetType(ComponentType::CompScript);
	SetGameObject(attached_gameobject);
	script = nullptr;
}

ComponentScript::~ComponentScript()
{
}

void ComponentScript::InitScript(std::string script_path)
{
	script->LoadScript(script_path.c_str());
	SetScriptPath(script_path);
}

void ComponentScript::SetScriptName(std::string name)
{
	script_name = name;
}

void ComponentScript::SetScriptPath(std::string path)
{
	script_path = path;
}

void ComponentScript::SetScript(Script * script)
{
	this->script = script;
}

std::string ComponentScript::GetScriptName() const
{
	return script_name;
}

std::string ComponentScript::GetScriptPath() const
{
	return script_path;
}

Script * ComponentScript::GetScript() const
{
	return script;
}

std::vector<ScriptField*> ComponentScript::GetScriptFields() const
{
	return script_fields;
}

void ComponentScript::UpdateScriptFields()
{
	script_fields = script->GetScriptFields();
}

void ComponentScript::Save(Data & data) const
{
}

void ComponentScript::Load(Data & data)
{
}
