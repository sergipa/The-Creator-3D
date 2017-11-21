#pragma once
#include "Resource.h"
#include "MathGeoLib\Math\float3.h"
#include "MathGeoLib\Math\float2.h"
#include "MathGeoLib\Math\float4.h"
#include <vector>

class GameObject;
class Texture;

struct ScriptField {
	enum PropertyType {
		Integer, Decimal, Float, Bool, String, GameObject, Animation, Vector2, Vector3, Vector4, Texture, Audio, None
	};
	std::string fieldName;
	PropertyType propertyType = None;
};

class Script :
	public Resource
{
public:
	Script();
	~Script();

	virtual bool InitScript(const char* code, GameObject* container) = 0;
	virtual void StartScript() = 0;
	virtual void UpdateScript(float deltaTime) = 0;
	virtual void OnCollisionEnter() = 0;
	virtual void OnCollisionStay() = 0;
	virtual void OnCollisionExit() = 0;
	virtual void OnEnable() = 0;
	virtual void OnDisable() = 0;

	virtual void CallFunction(const char* function) = 0;

	virtual void SetIntProperty(const char* propertyName, int value) = 0;
	virtual int GetIntProperty(const char* propertyName) = 0;
	virtual void SetDoubleProperty(const char* propertyName, double value) = 0;
	virtual double GetDoubleProperty(const char* propertyName) = 0;
	virtual void SetFloatProperty(const char* propertyName, float value) = 0;
	virtual float GetFloatProperty(const char* propertyName) = 0;
	virtual void SetBoolProperty(const char* propertyName, bool value) = 0;
	virtual bool GetBoolProperty(const char* propertyName) = 0;
	virtual void SetStringProperty(const char* propertyName, const char* value) = 0;
	virtual std::string GetStringProperty(const char* propertyName) = 0;
	virtual void SetGameObjectProperty(const char* propertyName, GameObject* value) = 0;
	virtual GameObject* GetGameObjectProperty(const char* propertyName) = 0;
	virtual void SetVec2Property(const char* propertyName, float2 value) = 0;
	virtual float2 GetVec2Property(const char* propertyName) = 0;
	virtual void SetVec3Property(const char* propertyName, float3 value) = 0;
	virtual float3 GetVec3Property(const char* propertyName) = 0;
	virtual void SetVec4Property(const char* propertyName, float4 value) = 0;
	virtual float4 GetVec4Property(const char* propertyName) = 0;
	virtual void SetTextureProperty(const char* propertyName, Texture* value) = 0;
	virtual Texture* GetSpriteProperty(const char* propertyName) = 0;

	virtual std::vector<ScriptField*> GetScriptFields() = 0;
};

