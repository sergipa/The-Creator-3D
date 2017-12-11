#include "ComponentScript.h"
#include "CSScript.h"
#include "ModuleResources.h"
#include "Application.h"
#include "ModuleScriptImporter.h"

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

void ComponentScript::InitScript()
{
	App->script_importer->SetCurrentScript((CSScript*)script);
	script->InitScript();
}

void ComponentScript::StartScript()
{
	App->script_importer->SetCurrentScript((CSScript*)script);
	script->StartScript();
}

void ComponentScript::UpdateScript()
{
	App->script_importer->SetCurrentScript((CSScript*)script);
	script->UpdateScript();
}

void ComponentScript::SetScript(Script * script)
{
	this->script = script;
}

std::string ComponentScript::GetScriptName() const
{
	return script->GetName();
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
	data.AddInt("Type", GetType());
	data.AddBool("Active", IsActive());
	data.AddUInt("UUID", GetUID());
	data.CreateSection("Script");
	if(script)script->Save(data);
	data.CloseSection();
}

void ComponentScript::Load(Data & data)
{
	SetType((Component::ComponentType)data.GetInt("Type"));
	SetActive(data.GetBool("Active"));
	SetUID(data.GetUInt("UUID"));
	data.EnterSection("Script");
	uint script_uid = data.GetUInt("UUID");
	if (script_uid != 0)
	{
		script = App->resources->GetScript(script_uid);
		if (!script)
		{
			script = new CSScript();
			script->Load(data);
		}
	}
	data.LeaveSection();
}
