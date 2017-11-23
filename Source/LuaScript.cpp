#include "LuaScript.h"
#include "GameObject.h"
#include "Texture.h"

#pragma comment (lib, "lua/lib/lua53.lib")

bool LuaScript::insideFunction = false;

LuaScript::LuaScript()
{
}

LuaScript::~LuaScript()
{
	CloseLua();
}

bool LuaScript::InitScript(const char * code, GameObject * container)
{
	CloseLua();

	insideFunction = false;
	bool ret = false;
	luaState = LuaNewState();
	luaL_openlibs(luaState);
	GlobalFunctions(luaState);
	RegisterAPI(luaState);

	if (luaL_loadstring(luaState, code) == 0) {
		lua_newtable(luaState);
		lua_pushstring(luaState, "Type");
		lua_pushstring(luaState, "GameObject");
		lua_settable(luaState, -3);
		lua_pushstring(luaState, "Data");
		lua_pushlightuserdata(luaState, container);
		lua_settable(luaState, -3);
		lua_setglobal(luaState, "this");
		ret = CallFunction(luaState, NULL);
		if (ret) {
			hasStartFunction = FunctionExist(luaState, "Start");
			hasUpdateFunction = FunctionExist(luaState, "Update");
			hasOnCollisionEnter = FunctionExist(luaState, "OnCollisionEnter");
			hasOnCollisionStay = FunctionExist(luaState, "OnCollisionStay");
			hasOnCollisionExit = FunctionExist(luaState, "OnCollisionExit");
			hasOnEnable = FunctionExist(luaState, "OnEnable");
			hasOnDisable = FunctionExist(luaState, "OnDisable");
			insideFunction = true;
		}
	}
	else {
		CONSOLE_WARNING("Cannot load lua script of '%s': %s", container->GetName().c_str(), lua_tostring(luaState, -1));
	}
	return ret;
}

void LuaScript::StartScript()
{
	if (luaState && hasStartFunction) {
		CallFunction(luaState, "Start");
	}
}

void LuaScript::UpdateScript(float deltaTime)
{
	if (luaState && hasUpdateFunction) {
		CallFunction(luaState, "Update");
	}
}

void LuaScript::OnCollisionEnter()
{
	if (luaState && hasOnCollisionEnter) {
		CallFunction(luaState, "OnCollisionEnter");
	}
}

void LuaScript::OnCollisionStay()
{
	if (luaState && hasOnCollisionStay) {
		CallFunction(luaState, "OnCollisionStay");
	}
}

void LuaScript::OnCollisionExit()
{
	if (luaState && hasOnCollisionExit) {
		CallFunction(luaState, "OnCollisionExit");
	}
}

void LuaScript::OnEnable()
{
	if (luaState && hasOnEnable) {
		CallFunction(luaState, "OnEnable");
	}
}

void LuaScript::OnDisable()
{
	if (luaState && hasOnDisable) {
		CallFunction(luaState, "OnDisable");
	}
}

void LuaScript::SetIntProperty(const char * propertyName, int value)
{
	SetFloatProperty(propertyName, (float)value);
}

int LuaScript::GetIntProperty(const char * propertyName)
{
	return (int)GetFloatProperty(propertyName);
}

void LuaScript::SetDoubleProperty(const char * propertyName, double value)
{
	SetFloatProperty(propertyName, (float)value);
}

double LuaScript::GetDoubleProperty(const char * propertyName)
{
	return (double)GetFloatProperty(propertyName);
}

void LuaScript::SetFloatProperty(const char * propertyName, float value)
{
	bool exist = false;
	lua_getglobal(luaState, propertyName);

	if (lua_isnumber(luaState, lua_gettop(luaState))) {
		exist = true;
	}
	lua_pop(luaState, 1);

	if (exist) {
		lua_pushnumber(luaState, value);
		lua_setglobal(luaState, propertyName);
	}
	else {
		CONSOLE_WARNING("Global value '%s' is not a number.", propertyName);
	}
}

float LuaScript::GetFloatProperty(const char* propertyName)
{
	float ret = 0;
	lua_getglobal(luaState, propertyName);
	if (lua_isnumber(luaState, lua_gettop(luaState)))
	{
		ret = (float)lua_tonumber(luaState, -1); // -1 = top of stack
	}
	else
	{
		CONSOLE_WARNING("Global value '%s' is not a number.", propertyName);
	}

	lua_pop(luaState, 1);
	return ret;
}

void LuaScript::SetBoolProperty(const char* propertyName, bool value)
{
	bool exists = false;

	lua_getglobal(luaState, propertyName);
	if (lua_isboolean(luaState, lua_gettop(luaState)))
	{
		exists = true;
	}

	// Pop after getglobal, since no other lua function is popping the stack
	lua_pop(luaState, 1);

	if (exists)
	{
		lua_pushboolean(luaState, (int)value);
		lua_setglobal(luaState, propertyName); // setglobal pops the stack for us
	}
	else
	{
		CONSOLE_WARNING("Global value '%s' is not a boolean.", propertyName);
	}
}

bool LuaScript::GetBoolProperty(const char* propertyName)
{
	bool ret = false;
	lua_getglobal(luaState, propertyName);

	if (lua_isboolean(luaState, lua_gettop(luaState)))
	{
		ret = lua_toboolean(luaState, -1); // -1 = top of stack
	}
	else
	{
		CONSOLE_WARNING("Global value '%s' is not a boolean.", propertyName);
	}

	lua_pop(luaState, 1);
	return ret;
}

void LuaScript::SetStringProperty(const char* propertyName, const char* value)
{
	bool exists = false;

	lua_getglobal(luaState, propertyName);
	if (lua_isstring(luaState, lua_gettop(luaState)))
	{
		exists = true;
	}

	lua_pop(luaState, 1);

	if (exists)
	{
		lua_pushstring(luaState, value);
		lua_setglobal(luaState, propertyName);
	}
	else {
		CONSOLE_WARNING("Global value '%s' is not a String.", propertyName);
	}
}

std::string LuaScript::GetStringProperty(const char* propertyName)
{
	std::string ret = "";
	lua_getglobal(luaState, propertyName);

	if (lua_isstring(luaState, lua_gettop(luaState)))
	{
		ret = (std::string)lua_tostring(luaState, -1);
	}
	else
	{
		CONSOLE_WARNING("Global value '%s' is not a String.", propertyName);
	}

	lua_pop(luaState, 1);
	return ret;
}

void LuaScript::SetGameObjectProperty(const char* propertyName, GameObject * value)
{
	bool exists = false;

	lua_getglobal(luaState, propertyName);

	if (lua_istable(luaState, lua_gettop(luaState)))
	{
		lua_pushstring(luaState, "Type");
		lua_gettable(luaState, -2);
		if (lua_isstring(luaState, -1)) {
			std::string str = lua_tostring(luaState, -1);
			if (str == "GameObject")
			{
				exists = true;
			}
		}
	}

	lua_pop(luaState, 1);

	if (exists)
	{
		lua_newtable(luaState);
		lua_pushstring(luaState, "Type");
		lua_pushstring(luaState, "GameObject");
		lua_settable(luaState, -3);
		lua_pushstring(luaState, "Data");
		lua_pushlightuserdata(luaState, value);
		lua_settable(luaState, -3);
		lua_setglobal(luaState, propertyName); // setglobal pops the stack for us
	}
	else
	{
		CONSOLE_WARNING("Global value '%s' is nil.", propertyName);
	}
}

GameObject * LuaScript::GetGameObjectProperty(const char* propertyName)
{
	GameObject* ret = nullptr;
	lua_getglobal(luaState, propertyName);

	if (lua_istable(luaState, lua_gettop(luaState)))
	{
		lua_pushstring(luaState, "Type");
		lua_gettable(luaState, -2);
		if (lua_isstring(luaState, -1)) {
			std::string str = lua_tostring(luaState, -1);
			if (str == "GameObject")
			{
				lua_pop(luaState, 1);
				lua_pushstring(luaState, "Data");
				lua_gettable(luaState, -2);
				ret = (GameObject*)lua_touserdata(luaState, -1);
			}
		}
	}
	else
	{
		CONSOLE_WARNING("Global value '%s' is nil.", propertyName);
	}

	lua_pop(luaState, 1);
	return ret;
}

void LuaScript::SetVec2Property(const char* propertyName, float2 value)
{
	bool exist = false;
	lua_getglobal(luaState, propertyName);
	if (lua_istable(luaState, lua_gettop(luaState))) {
		exist = true;
	}
	lua_pop(luaState, 1);

	if (exist) {
		lua_newtable(luaState);

		lua_pushnumber(luaState, value.x);
		lua_rawseti(luaState, -2, 1);

		lua_pushnumber(luaState, value.y);
		lua_rawseti(luaState, -2, 2);

		lua_setglobal(luaState, propertyName);
	}
	else {
		CONSOLE_WARNING("Global value '%s' is nil.", propertyName);
	}
}

float2 LuaScript::GetVec2Property(const char* propertyName)
{
	float2 v2;
	lua_getglobal(luaState, propertyName);

	if (lua_istable(luaState, lua_gettop(luaState))) {
		lua_pushinteger(luaState, 1);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v2.x = (float)lua_tonumber(luaState, -1);
		}
		else {
			CONSOLE_WARNING("Value 'x' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);

		lua_pushinteger(luaState, 2);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v2.y = (float)lua_tonumber(luaState, -1);
		}
		else {
			CONSOLE_WARNING("Value 'y' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);
	}

	lua_pop(luaState, 1);
	return v2;
}

void LuaScript::SetVec3Property(const char* propertyName, float3 value)
{
	bool exist = false;
	lua_getglobal(luaState, propertyName);
	if (lua_istable(luaState, lua_gettop(luaState))) {
		exist = true;
	}
	lua_pop(luaState, 1);

	if (exist) {
		lua_newtable(luaState);

		lua_pushnumber(luaState, value.x);
		lua_rawseti(luaState, -2, 1);

		lua_pushnumber(luaState, value.y);
		lua_rawseti(luaState, -2, 2);

		lua_pushnumber(luaState, value.z);
		lua_rawseti(luaState, -2, 3);

		lua_setglobal(luaState, propertyName);
	}
	else {
		CONSOLE_WARNING("Global value '%s' is nil.", propertyName);
	}
}

float3 LuaScript::GetVec3Property(const char* propertyName)
{
	float3 v3;
	lua_getglobal(luaState, propertyName);

	if (lua_istable(luaState, lua_gettop(luaState))) {
		lua_pushinteger(luaState, 1);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v3.x = (float)lua_tonumber(luaState, -1);
		}
		else {
			CONSOLE_WARNING("Value 'x' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);

		lua_pushinteger(luaState, 2);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v3.y = (float)lua_tonumber(luaState, -1);
		}
		else {
			CONSOLE_WARNING("Value 'y' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);

		lua_pushinteger(luaState, 3);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v3.z = (float)lua_tonumber(luaState, -1);
		}
		else {
			CONSOLE_WARNING("Value 'z' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);
	}

	lua_pop(luaState, 1);
	return v3;
}

void LuaScript::SetVec4Property(const char * propertyName, float4 value)
{
	bool exist = false;
	lua_getglobal(luaState, propertyName);
	if (lua_istable(luaState, lua_gettop(luaState))) {
		exist = true;
	}
	lua_pop(luaState, 1);

	if (exist) {
		lua_newtable(luaState);

		lua_pushnumber(luaState, value.x);
		lua_rawseti(luaState, -2, 1);

		lua_pushnumber(luaState, value.y);
		lua_rawseti(luaState, -2, 2);

		lua_pushnumber(luaState, value.z);
		lua_rawseti(luaState, -2, 3);

		lua_pushnumber(luaState, value.w);
		lua_rawseti(luaState, -2, 3);

		lua_setglobal(luaState, propertyName);
	}
	else {
		CONSOLE_WARNING("Global value '%s' is nil.", propertyName);
	}
}

float4 LuaScript::GetVec4Property(const char * propertyName)
{
	float4 v4;
	lua_getglobal(luaState, propertyName);

	if (lua_istable(luaState, lua_gettop(luaState))) {
		lua_pushinteger(luaState, 1);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v4.x = (float)lua_tonumber(luaState, -1);
		}
		else {
			CONSOLE_WARNING("Value 'x' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);

		lua_pushinteger(luaState, 2);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v4.y = (float)lua_tonumber(luaState, -1);
		}
		else {
			CONSOLE_WARNING("Value 'y' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);

		lua_pushinteger(luaState, 3);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v4.z = (float)lua_tonumber(luaState, -1);
		}
		else {
			CONSOLE_WARNING("Value 'z' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);

		lua_pushinteger(luaState, 4);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v4.w = (float)lua_tonumber(luaState, -1);
		}
		else {
			CONSOLE_WARNING("Value 'w' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);
	}

	lua_pop(luaState, 1);
	return v4;
}

void LuaScript::SetTextureProperty(const char * propertyName, Texture * value)
{
}

Texture * LuaScript::GetTextureProperty(const char * propertyName)
{
	return nullptr;
}

std::vector<ScriptField*> LuaScript::GetScriptFields()
{
	fields.clear();

	lua_pushglobaltable(luaState);
	lua_pushnil(luaState);

	while (lua_next(luaState, -2) != 0) {
		int keyType = lua_type(luaState, -2);
		if (keyType != LUA_TSTRING) {
			lua_pop(luaState, 1);
			continue;
		}

		int valueType = lua_type(luaState, -1);
		std::string keyName(lua_tostring(luaState, -2));
		if (keyName.length() == 0 || keyName[0] == '_' || keyName == "this") {
			lua_pop(luaState, 1);
			continue;
		}

		ScriptField* propertyField = new ScriptField();
		propertyField->fieldName = keyName;

		switch (valueType) {
		case LUA_TNUMBER:
		{
			double n = lua_tonumber(luaState, -1);
			if (n == (int)n) {
				propertyField->propertyType = propertyField->Integer;
			}
			else {
				propertyField->propertyType = propertyField->Float;
			}
		}
		break;
		case LUA_TSTRING:
			propertyField->propertyType = propertyField->String;
			break;
		case LUA_TBOOLEAN:
			propertyField->propertyType = propertyField->Bool;
			break;
		case LUA_TLIGHTUSERDATA:
			propertyField->propertyType = propertyField->GameObject;
			break;
		case LUA_TTABLE:
		{
			if (lua_rawlen(luaState, -1) == 3) {
				propertyField->propertyType = propertyField->Vector3;
			}
			else if (lua_rawlen(luaState, -1) == 2) {
				propertyField->propertyType = propertyField->Vector2;
			}
			else if (lua_rawlen(luaState, -1) == 4) {
				propertyField->propertyType = propertyField->Vector4;
			}
		}
		break;
		default:
			propertyField->propertyType = propertyField->None;
			break;
		}
		if (propertyField->propertyType != propertyField->None) {
			fields.push_back(propertyField);
		}

		lua_pop(luaState, 1);
	}

	return fields;
}

void LuaScript::CloseLua()
{
	if (luaState)
	{
		lua_close(luaState);
		luaState = NULL;
	}
}

lua_State * LuaScript::LuaNewState()
{
	lua_State* luaState = lua_newstate(LuaAlloc, NULL);
	if (luaState)
	{
		lua_atpanic(luaState, &LuaPanic);
	}
	return luaState;
}

void * LuaScript::LuaAlloc(void * Ud, void * Ptr, size_t OldSize, size_t NewSize)
{
	if (NewSize != 0)
	{
		return realloc(Ptr, NewSize);
	}
	else
	{
		free(Ptr);
		return NULL;
	}
}

int LuaScript::LuaPanic(lua_State * lua_State)
{
	CONSOLE_ERROR("PANIC: unprotected error in call to Lua API: %s", lua_tostring(lua_State, -1));
	return 0;
}

void LuaScript::GlobalFunctions(lua_State * luaState)
{
	static const luaL_Reg _GFunctions[] =
	{
		{ "print", PrintToLog },
		{ "Invoke", Invoke },
		{ "Instantiate", Instantiate },
		{ NULL, NULL }
	};

	lua_getglobal(luaState, "_G");
	luaL_setfuncs(luaState, _GFunctions, 0);
	lua_pop(luaState, 1);
}

int LuaScript::PrintToLog(lua_State * luaState)
{
	int ArgCount = lua_gettop(luaState);
	std::string message;

	for (int ArgIndex = 1; ArgIndex <= ArgCount; ++ArgIndex)
	{
		if (lua_isstring(luaState, ArgIndex))
		{
			message += lua_tostring(luaState, ArgIndex);
		}
		else if (lua_isboolean(luaState, ArgIndex)) {
			bool boolArg = lua_toboolean(luaState, ArgIndex);
			if (boolArg) {
				message += "true";
			}
			else {
				message += "false";
			}
		}
		else if (lua_isnumber(luaState, ArgIndex)) {
			message += std::to_string(lua_tonumber(luaState, ArgIndex));
		}
		else if (lua_istable(luaState, ArgIndex)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -2);
			if (lua_isstring(luaState, -1)) {
				std::string str = lua_tostring(luaState, -1);
				if (str == "GameObject") {
					message += "GameObject!";
				}
			}
		}
		else {
			CONSOLE_ERROR("Don't recognise print function argument type");
			return 0;
		}
	}
	CONSOLE_LOG("%s", message.c_str());
	return 0;
}

void LuaScript::RegisterAPI(lua_State * luaState)
{
}

int LuaScript::IsGameObjectActive(lua_State * luaState)
{
	return 0;
}

int LuaScript::SetGameObjectActive(lua_State * luaState)
{
	return 0;
}

int LuaScript::GetGameObjectPosition(lua_State * luaState)
{
	return 0;
}

int LuaScript::SetGameObjectPosition(lua_State * luaState)
{
	return 0;
}

int LuaScript::GetGameObjectGlobalPosition(lua_State * luaState)
{
	return 0;
}

int LuaScript::GetGameObjectComponent(lua_State * luaState)
{
	return 0;
}

int LuaScript::AddGameObjectComponent(lua_State * luaState)
{
	return 0;
}

int LuaScript::GetGameObjectLayer(lua_State * luaState)
{
	return 0;
}

int LuaScript::SetGameObjectLayer(lua_State * luaState)
{
	return 0;
}

int LuaScript::GetGameObjectName(lua_State * luaState)
{
	return 0;
}

int LuaScript::SetGameObjectName(lua_State * luaState)
{
	return 0;
}

int LuaScript::GetGameObjectTag(lua_State * luaState)
{
	return 0;
}

int LuaScript::SetGameObjectTag(lua_State * luaState)
{
	return 0;
}

int LuaScript::GetGameObjectParent(lua_State * luaState)
{
	return 0;
}

int LuaScript::SetGameObjectParent(lua_State * luaState)
{
	return 0;
}

int LuaScript::GetGameObjectRoot(lua_State * luaState)
{
	return 0;
}

int LuaScript::GetGameObjectChild(lua_State * luaState)
{
	return 0;
}

int LuaScript::GetGameObjectChildByIndex(lua_State * luaState)
{
	return 0;
}

int LuaScript::GetGameObjectChildsCount(lua_State * luaState)
{
	return 0;
}

int LuaScript::FindGameObject(lua_State * luaState)
{
	return 0;
}

int LuaScript::DestroyGameObject(lua_State * luaState)
{
	return 0;
}

int LuaScript::Invoke(lua_State * luaState)
{
	return 0;
}

int LuaScript::Instantiate(lua_State * luaState)
{
	return 0;
}

int LuaScript::Vector4(lua_State * luaState)
{
	return 0;
}

int LuaScript::Vector3(lua_State * luaState)
{
	return 0;
}

int LuaScript::Vector2(lua_State * luaState)
{
	return 0;
}

int LuaScript::IsKeyPressed(lua_State * luaState)
{
	return 0;
}

int LuaScript::IsKeyReleased(lua_State * luaState)
{
	return 0;
}

int LuaScript::IsKeyRepeated(lua_State * luaState)
{
	return 0;
}

int LuaScript::IsMouseButtonPressed(lua_State * luaState)
{
	return 0;
}

int LuaScript::IsMouseButtonReleased(lua_State * luaState)
{
	return 0;
}

int LuaScript::IsMouseButtonRepeated(lua_State * luaState)
{
	return 0;
}

void LuaScript::StackDump(lua_State * luaState)
{
	int i;
	int top = lua_gettop(luaState);

	printf("total in stack %d\n", top);

	for (i = 1; i <= top; i++)
	{  /* repeat for each level */
		int t = lua_type(luaState, i);
		switch (t) {
		case LUA_TSTRING:  /* strings */
			printf("string: '%s'\n", lua_tostring(luaState, i));
			break;
		case LUA_TBOOLEAN:  /* booleans */
			printf("boolean %s\n", lua_toboolean(luaState, i) ? "true" : "false");
			break;
		case LUA_TNUMBER:  /* numbers */
			printf("number: %g\n", lua_tonumber(luaState, i));
			break;
		case LUA_TTABLE:
			printf("table\n");
			/*while (lua_next(luaState, -2)) {

			}*/
			break;
		default:  /* other values */
			printf("%s\n", lua_typename(luaState, t));
			break;
		}
	}
	printf("\n");  /* end the listing */
}

void LuaScript::RegisterLibrary(lua_State * luaState, luaL_Reg lib[], const char * libName)
{
	lua_getglobal(luaState, libName);
	if (lua_isnil(luaState, -1))
	{
		lua_pop(luaState, 1);
		lua_newtable(luaState);
	}
	luaL_setfuncs(luaState, lib, 0);
	lua_setglobal(luaState, libName);
}

bool LuaScript::CallFunction(lua_State * luaState, const char * functionName)
{
	bool ret = true;

	if (functionName != NULL) {
		lua_getglobal(luaState, functionName);
	}

	int numArgs = 0;
	int numResults = 0;
	if (lua_pcall(luaState, numArgs, numResults, 0) != 0) {
		CONSOLE_WARNING("Cannot call Lua function %s: %s", functionName, lua_tostring(luaState, -1));
		ret = false;
	}

	return ret;
}

bool LuaScript::FunctionExist(lua_State * luaState, const char * functionName)
{
	bool ret = true;

	lua_getglobal(luaState, functionName);
	if (!lua_isfunction(luaState, lua_gettop(luaState))) {
		ret = false;
	}
	lua_pop(luaState, 1);

	return ret;
}
