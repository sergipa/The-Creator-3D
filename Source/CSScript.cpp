#include "CSScript.h"
#include "GameObject.h"

#pragma comment (lib, "mono/lib/mono-2.0-sgen.lib")


CSScript::CSScript()
{
	domain = nullptr;
	assembly = nullptr;
	mono_class = nullptr;
}

CSScript::~CSScript()
{
}

bool CSScript::InitScript(const char * code, GameObject * container)
{
	//mono_class = mono_class_from_name()
	return false;
}

void CSScript::StartScript()
{
}

void CSScript::UpdateScript(float deltaTime)
{
}

void CSScript::OnCollisionEnter()
{
}

void CSScript::OnCollisionStay()
{
}

void CSScript::OnCollisionExit()
{
}

void CSScript::OnEnable()
{
}

void CSScript::OnDisable()
{
}

void CSScript::SetIntProperty(const char * propertyName, int value)
{
}

int CSScript::GetIntProperty(const char * propertyName)
{
	return 0;
}

void CSScript::SetDoubleProperty(const char * propertyName, double value)
{
}

double CSScript::GetDoubleProperty(const char * propertyName)
{
	return 0.0;
}

void CSScript::SetFloatProperty(const char * propertyName, float value)
{
	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);
	
	if(field)
	{
		void* params = &value;
		mono_field_set_value(object, field, params);
	}
}

float CSScript::GetFloatProperty(const char * propertyName)
{
	float value = 0.0f;

	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		mono_field_get_value(object, field, &value);
	}

	return value;
}

void CSScript::SetBoolProperty(const char * propertyName, bool value)
{
}

bool CSScript::GetBoolProperty(const char * propertyName)
{
	return false;
}

void CSScript::SetStringProperty(const char * propertyName, const char * value)
{
}

std::string CSScript::GetStringProperty(const char * propertyName)
{
	return std::string();
}

void CSScript::SetGameObjectProperty(const char * propertyName, GameObject * value)
{
	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);
	
	if (field)
	{
		MonoClass* gameobject = mono_type_get_class(mono_field_get_type(field));
		MonoObject* new_object = mono_object_new(domain, gameobject);
		MonoClassField* this_field = mono_class_get_field_from_name(gameobject, "this");
		
		if (this_field)
		{
			void* params = value;
			mono_field_set_value(new_object, this_field, params);

			mono_field_set_value(object, field, new_object);
		}
	}
}

GameObject * CSScript::GetGameObjectProperty(const char * propertyName)
{
	GameObject* gameobject = nullptr;

	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		MonoObject* mono_object = nullptr;
		mono_field_get_value(object, field, &mono_object);
		
		if (mono_object != nullptr)
		{
			MonoClass* object_class = mono_object_get_class(mono_object);
			field = mono_class_get_field_from_name(object_class, "this");
			
			if (field)
			{
				mono_field_get_value(mono_object, field, &gameobject);
			}
		}
	}

	return gameobject;
}

void CSScript::SetVec2Property(const char * propertyName, float2 value)
{
}

float2 CSScript::GetVec2Property(const char * propertyName)
{
	return float2();
}

void CSScript::SetVec3Property(const char * propertyName, float3 value)
{
}

float3 CSScript::GetVec3Property(const char * propertyName)
{
	return float3();
}

void CSScript::SetVec4Property(const char * propertyName, float4 value)
{
}

float4 CSScript::GetVec4Property(const char * propertyName)
{
	return float4();
}

void CSScript::SetTextureProperty(const char * propertyName, Texture * value)
{
}

Texture * CSScript::GetTextureProperty(const char * propertyName)
{
	return nullptr;
}

std::vector<ScriptField*> CSScript::GetScriptFields()
{
	return std::vector<ScriptField*>();
}

MonoMethod * CSScript::GetFunction(const char * functionName, int parameters_count)
{
	MonoMethod* method = nullptr;

	if (mono_class != nullptr)
	{
		method = mono_class_get_method_from_name(mono_class, functionName, parameters_count);
	}

	return method;
}

void CSScript::CallFunction(MonoMethod * function, void ** parameter)
{
	MonoObject* exception = nullptr;
	mono_runtime_invoke((MonoMethod*)function, object, parameter, &exception);

	if (exception)
	{
		mono_print_unhandled_exception(exception);
	}
}
