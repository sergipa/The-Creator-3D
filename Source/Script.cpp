#include "Script.h"

Script::Script()
{
}

Script::~Script()
{
}

void Script::SetScriptType(ScriptType type)
{
	script_type = type;
}

Script::ScriptType Script::GetScriptType()
{
	return script_type;
}
