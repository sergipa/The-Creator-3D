#pragma once

#include "Script.h"
#include "lua/include/lua.hpp"

class Data;

class LuaScript :
	public Script
{
public:
	LuaScript();
	~LuaScript();

	bool LoadScript(std::string script_path);
	void InitScript();
	void StartScript();
	void UpdateScript(float deltaTime);
	void OnCollisionEnter();
	void OnCollisionStay();
	void OnCollisionExit();
	void OnEnable();
	void OnDisable();

	void SetIntProperty(const char* propertyName, int value);
	int GetIntProperty(const char* propertyName);
	void SetDoubleProperty(const char* propertyName, double value);
	double GetDoubleProperty(const char* propertyName);
	void SetFloatProperty(const char* propertyName, float value);
	float GetFloatProperty(const char* propertyName);
	void SetBoolProperty(const char* propertyName, bool value);
	bool GetBoolProperty(const char* propertyName);
	void SetStringProperty(const char* propertyName, const char* value);
	std::string GetStringProperty(const char* propertyName);
	void SetGameObjectProperty(const char* propertyName, GameObject* value);
	GameObject* GetGameObjectProperty(const char* propertyName);
	void SetVec2Property(const char* propertyName, float2 value);
	float2 GetVec2Property(const char* propertyName);
	void SetVec3Property(const char* propertyName, float3 value);
	float3 GetVec3Property(const char* propertyName);
	void SetVec4Property(const char* propertyName, float4 value);
	float4 GetVec4Property(const char* propertyName);
	void SetTextureProperty(const char* propertyName, Texture* value);
	Texture* GetTextureProperty(const char* propertyName);

	std::vector<ScriptField*> GetScriptFields();

private:
	void CloseLua();
	static lua_State* LuaNewState();
	static void* LuaAlloc(void *Ud, void *Ptr, size_t OldSize, size_t NewSize);
	static int LuaPanic(lua_State *luaState);
	void GlobalFunctions(lua_State* luaState);
	static int PrintToLog(lua_State* luaState);
	void RegisterAPI(lua_State* luaState);

	static void RegisterLibrary(lua_State* luaState, luaL_Reg lib[], const char* libName);
	static bool CallFunction(lua_State* luaState, const char* functionName);
	static bool FunctionExist(lua_State* luaState, const char* functionName);

	//GameObject library
	static int IsGameObjectActive(lua_State* luaState);
	static int SetGameObjectActive(lua_State* luaState);
	static int GetGameObjectPosition(lua_State* luaState);
	static int SetGameObjectPosition(lua_State* luaState);
	static int GetGameObjectGlobalPosition(lua_State* luaState);
	static int GetGameObjectComponent(lua_State* luaState);
	static int AddGameObjectComponent(lua_State* luaState);
	static int GetGameObjectLayer(lua_State* luaState);
	static int SetGameObjectLayer(lua_State* luaState);
	static int GetGameObjectName(lua_State* luaState);
	static int SetGameObjectName(lua_State* luaState);
	static int GetGameObjectTag(lua_State* luaState);
	static int SetGameObjectTag(lua_State* luaState);
	static int GetGameObjectParent(lua_State* luaState);
	static int SetGameObjectParent(lua_State* luaState);
	static int GetGameObjectRoot(lua_State* luaState);
	static int GetGameObjectChild(lua_State* luaState);
	static int GetGameObjectChildByIndex(lua_State* luaState);
	static int GetGameObjectChildsCount(lua_State* luaState);
	static int FindGameObject(lua_State* luaState);
	static int DestroyGameObject(lua_State* luaState);

	//Globals
	static int Invoke(lua_State* luaState);
	static int Instantiate(lua_State* luaState);

	//Math library
	static int Vector4(lua_State* luaState);
	static int Vector3(lua_State* luaState);
	static int Vector2(lua_State* luaState);

	//Input library
	static int IsKeyPressed(lua_State* luaState);
	static int IsKeyReleased(lua_State* luaState);
	static int IsKeyRepeated(lua_State* luaState);
	static int IsMouseButtonPressed(lua_State* luaState);
	static int IsMouseButtonReleased(lua_State* luaState);
	static int IsMouseButtonRepeated(lua_State* luaState);

	static void StackDump(lua_State* luaState);

	void Save(Data& data) const;
	bool Load(Data& data);
	void CreateMeta() const;
	void LoadToMemory();
	void UnloadFromMemory();

private:
	bool hasStartFunction;
	bool hasUpdateFunction;
	bool hasOnCollisionEnter;
	bool hasOnCollisionStay;
	bool hasOnCollisionExit;
	bool hasOnEnable;
	bool hasOnDisable;
	static bool insideFunction;
	lua_State* luaState;
	std::vector<ScriptField*> fields;
};

