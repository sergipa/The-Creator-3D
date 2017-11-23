#pragma once

#include "Script.h"
#include "mono/include/jit/jit.h"
#include "mono/include/metadata/assembly.h"

class CSScript :
	public Script
{
public:
	CSScript();
	~CSScript();

	bool InitScript(const char* code, GameObject* container);
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
	MonoMethod* GetFunction(const char* functionName, int parameters);
	void CallFunction(MonoMethod* function, void** parameter);

private:
	MonoDomain* domain;
	MonoAssembly* assembly;
	MonoClass* mono_class;
	MonoImage* image;
	MonoObject* object;
};