#include "CSScript.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "Data.h"
#include "ModuleScriptImporter.h"

#pragma comment (lib, "mono/lib/mono-2.0-sgen.lib")


CSScript::CSScript()
{
	mono_domain = nullptr;
	mono_assembly = nullptr;
	mono_class = nullptr;
	mono_image = nullptr;

	SetScriptType(ScriptType::CsScript);
}

CSScript::~CSScript()
{
}

bool CSScript::LoadScript(std::string script_path)
{
	bool ret = false;
	SetLibraryPath(script_path);

	if (mono_class)
	{
		mono_object = mono_object_new(App->script_importer->GetDomain(), mono_class);
	}
	if (mono_object)
	{
		mono_runtime_object_init(mono_object);
		ret = true;
	}
	
	return ret;
}

void CSScript::InitScript()
{
	MonoMethod* init = GetFunction("Init", 0);

	if (init != nullptr)
	{
		CallFunction(init, nullptr);
	}
}

void CSScript::StartScript()
{
	MonoMethod* start = GetFunction("Start", 0);

	if (start != nullptr)
	{
		CallFunction(start, nullptr);
	}
}

void CSScript::UpdateScript(float deltaTime)
{
	MonoMethod* update = GetFunction("Update", 0);

	if (update != nullptr)
	{
		CallFunction(update, nullptr);
	}
}

void CSScript::OnCollisionEnter()
{
	MonoMethod* on_collision_enter = GetFunction("OnCollisionEnter", 1);

	if (on_collision_enter != nullptr)
	{
		CallFunction(on_collision_enter, nullptr); //nullptr should be the collision
	}
}

void CSScript::OnCollisionStay()
{
	MonoMethod* on_collision_stay = GetFunction("OnCollisionStay", 1);

	if (on_collision_stay != nullptr)
	{
		CallFunction(on_collision_stay, nullptr); //nullptr should be the collision
	}
}

void CSScript::OnCollisionExit()
{
	MonoMethod* on_collision_exit = GetFunction("OnCollisionExit", 1);

	if (on_collision_exit != nullptr)
	{
		CallFunction(on_collision_exit, nullptr); //nullptr should be the collision
	}
}

void CSScript::OnEnable()
{
	MonoMethod* on_enable = GetFunction("OnEnable", 0);

	if (on_enable != nullptr)
	{
		CallFunction(on_enable, nullptr);
	}
}

void CSScript::OnDisable()
{
	MonoMethod* on_disable = GetFunction("OnDisable", 0);

	if (on_disable != nullptr)
	{
		CallFunction(on_disable, nullptr);
	}
}

void CSScript::SetIntProperty(const char * propertyName, int value)
{
	SetFloatProperty(propertyName, (float)value);
}

int CSScript::GetIntProperty(const char * propertyName)
{
	int value = 0;

	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		mono_field_get_value(mono_object, field, &value);
	}

	return value;
}

void CSScript::SetDoubleProperty(const char * propertyName, double value)
{
	SetFloatProperty(propertyName, (double)value);
}

double CSScript::GetDoubleProperty(const char * propertyName)
{
	double value = 0.0;

	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		mono_field_get_value(mono_object, field, &value);
	}

	return value;
}

void CSScript::SetFloatProperty(const char * propertyName, float value)
{
	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);
	
	if(field)
	{
		void* params = &value;
		mono_field_set_value(mono_object, field, params);
	}
}

float CSScript::GetFloatProperty(const char * propertyName)
{
	float value = 0.0f;

	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		mono_field_get_value(mono_object, field, &value);
	}

	return value;
}

void CSScript::SetBoolProperty(const char * propertyName, bool value)
{
	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		void* params = &value;
		mono_field_set_value(mono_object, field, params);
	}
}

bool CSScript::GetBoolProperty(const char * propertyName)
{
	bool value = true;

	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		mono_field_get_value(mono_object, field, &value);
	}

	return value;
}

void CSScript::SetStringProperty(const char * propertyName, const char * value)
{
	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		void* params = &value;
		mono_field_set_value(mono_object, field, params);
	}
}

std::string CSScript::GetStringProperty(const char * propertyName)
{
	std::string value = "";

	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		mono_field_get_value(mono_object, field, &value);
	}

	return value;
}

void CSScript::SetGameObjectProperty(const char * propertyName, GameObject * value)
{
	/*MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);
	
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
	}*/

	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);
	if (field)
	{
		void* params = &value;
		mono_field_set_value(mono_object, field, params);
	}
}

GameObject * CSScript::GetGameObjectProperty(const char * propertyName)
{
	/*GameObject* gameobject = nullptr;

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

	return gameobject;*/

	GameObject* value = nullptr;
	MonoClassField* Field = mono_class_get_field_from_name(mono_class, propertyName);
	if (Field)
	{
		mono_field_get_value(mono_object, Field, &value);
	}

	return value;
}

void CSScript::SetVec2Property(const char * propertyName, float2 value)
{
	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		MonoType* type = mono_field_get_type(field);
		MonoClass* eclass = mono_class_get_element_class(mono_type_get_class(type));
		MonoArray* array_value = mono_array_new(mono_domain, eclass, 2);

		mono_array_set(array_value, float, 0, value.x);
		mono_array_set(array_value, float, 1, value.y);

		mono_field_set_value(mono_object, field, array_value);
	}
}

float2 CSScript::GetVec2Property(const char * propertyName)
{
	float2 value;

	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		MonoArray* array_value = nullptr;
		mono_field_get_value(mono_object, field, &array_value);

		if (array_value != nullptr)
		{
			value.x = mono_array_get(array_value, float, 0);
			value.y = mono_array_get(array_value, float, 1);
		}
	}

	return value;
}

void CSScript::SetVec3Property(const char * propertyName, float3 value)
{
	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		MonoType* type = mono_field_get_type(field);
		MonoClass* eclass = mono_class_get_element_class(mono_type_get_class(type));
		MonoArray* array_value = mono_array_new(mono_domain, eclass, 3);

		mono_array_set(array_value, float, 0, value.x);
		mono_array_set(array_value, float, 1, value.y);
		mono_array_set(array_value, float, 2, value.z);

		mono_field_set_value(mono_object, field, array_value);
	}
}

float3 CSScript::GetVec3Property(const char * propertyName)
{
	float3 value;

	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		MonoArray* array_value = nullptr;
		mono_field_get_value(mono_object, field, &array_value);

		if (array_value != nullptr)
		{
			value.x = mono_array_get(array_value, float, 0);
			value.y = mono_array_get(array_value, float, 1);
			value.z = mono_array_get(array_value, float, 2);
		}
	}

	return value;
}

void CSScript::SetVec4Property(const char * propertyName, float4 value)
{
	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		MonoType* type = mono_field_get_type(field);
		MonoClass* eclass = mono_class_get_element_class(mono_type_get_class(type));
		MonoArray* array_value = mono_array_new(mono_domain, eclass, 3);

		mono_array_set(array_value, float, 0, value.x);
		mono_array_set(array_value, float, 1, value.y);
		mono_array_set(array_value, float, 2, value.z);
		mono_array_set(array_value, float, 3, value.w);

		mono_field_set_value(mono_object, field, array_value);
	}
}

float4 CSScript::GetVec4Property(const char * propertyName)
{
	float4 value;

	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		MonoArray* array_value = nullptr;
		mono_field_get_value(mono_object, field, &array_value);

		if (array_value != nullptr)
		{
			value.x = mono_array_get(array_value, float, 0);
			value.y = mono_array_get(array_value, float, 1);
			value.z = mono_array_get(array_value, float, 2);
			value.w = mono_array_get(array_value, float, 3);
		}
	}

	return value;
}

void CSScript::SetTextureProperty(const char * propertyName, Texture * value)
{
	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);
	if (field)
	{
		void* params = &value;
		mono_field_set_value(mono_object, field, params);
	}

}

Texture * CSScript::GetTextureProperty(const char * propertyName)
{
	Texture* value = nullptr;
	MonoClassField* Field = mono_class_get_field_from_name(mono_class, propertyName);
	if (Field)
	{
		mono_field_get_value(mono_object, Field, &value);
	}

	return value;
}

std::vector<ScriptField*> CSScript::GetScriptFields()
{
	void* iter = nullptr;
	MonoClassField* field = mono_class_get_fields(mono_class, &iter);

	script_fields.clear();
	
	while (field != nullptr)
	{
		uint32_t flags = mono_field_get_flags(field);
		if ((flags & MONO_FIELD_ATTR_PUBLIC) && (flags & MONO_FIELD_ATTR_STATIC) == 0)
		{
			ScriptField* property_field = new ScriptField();
			property_field->fieldName = mono_field_get_name(field);
			MonoType* type = mono_field_get_type(field);
			ConvertMonoType(type, *property_field);
			script_fields.push_back(property_field);
		}
		field = mono_class_get_fields(mono_class, &iter);
	}

	return script_fields;
}

void CSScript::SetDomain(MonoDomain * mono_domain)
{
	this->mono_domain = mono_domain;
}

void CSScript::SetImage(MonoImage * mono_image)
{
	this->mono_image = mono_image;
}

void CSScript::SetClass(MonoClass * mono_class)
{
	this->mono_class = mono_class;
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
	mono_runtime_invoke((MonoMethod*)function, mono_object, parameter, &exception);

	if (exception)
	{
		mono_print_unhandled_exception(exception);
	}
}

void CSScript::ConvertMonoType(MonoType * type, ScriptField& script_field)
{
	std::string name = mono_type_get_name(type);

	switch (mono_type_get_type(type))
	{
	case MONO_TYPE_I4:
		script_field.propertyType = ScriptField::Integer;
		break;
	case MONO_TYPE_R4:
		script_field.propertyType = ScriptField::Float;
		break;
	case MONO_TYPE_BOOLEAN:
		script_field.propertyType = ScriptField::Bool;
		break;
	case MONO_TYPE_STRING:
		script_field.propertyType = ScriptField::String;
		break;
	case MONO_TYPE_CLASS:
		if(name == "GameObject") script_field.propertyType = ScriptField::GameObject;
		else if (name == "Texture") script_field.propertyType = ScriptField::Texture;
		else if (name == "Animation") script_field.propertyType = ScriptField::Animation;
		else if (name == "Audio") script_field.propertyType = ScriptField::Audio;
		break;
	case MONO_TYPE_SZARRAY:
		break;
	case MONO_TYPE_VALUETYPE:
		break;
	case MONO_TYPE_GENERICINST:
		break;
	case MONO_TYPE_CHAR:
		break;
	case MONO_TYPE_I1:
		break;
	case MONO_TYPE_U1:
		break;
	case MONO_TYPE_I2:
		break;
	case MONO_TYPE_U2:
		break;
	case MONO_TYPE_U4:
		break;
	case MONO_TYPE_I8:
		break;
	case MONO_TYPE_U8:
		break;
	default:
		break;
	}
}

void CSScript::RegisterLibrary()
{
}

void CSScript::Save(Data & data) const
{
}

bool CSScript::Load(Data & data)
{
	return false;
}

void CSScript::CreateMeta() const
{
}

void CSScript::LoadToMemory()
{
}

void CSScript::UnloadFromMemory()
{
}
