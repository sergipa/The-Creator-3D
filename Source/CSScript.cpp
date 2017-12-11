#include "CSScript.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "Data.h"
#include "ModuleScriptImporter.h"
#include <ctime>
#include "ModuleScene.h"
#include "ModuleResources.h"
#include <mono/metadata/reflection.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/exception.h>
#include "ComponentTransform.h"
#include "ModuleInput.h"

#pragma comment (lib, "../EngineResources/mono/lib/mono-2.0-sgen.lib")

CSScript::CSScript()
{
	mono_domain = nullptr;
	mono_assembly = nullptr;
	mono_class = nullptr;
	mono_image = nullptr;

	init = nullptr;
	start = nullptr;
	update = nullptr;
	on_collision_enter = nullptr;
	on_collision_stay = nullptr;
	on_collision_exit = nullptr;
	on_enable = nullptr;
	on_disable = nullptr;

	active_gameobject = nullptr;
	attached_gameobject = nullptr;
	inside_function = false;
	modifying_self = false;

	SetType(Resource::ScriptResource);
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
		
		init = GetFunction("Init", 0);
		start = GetFunction("Start", 0);
		update = GetFunction("Update", 0);
		on_collision_enter = GetFunction("OnCollisionEnter", 1);
		on_collision_stay = GetFunction("OnCollisionStay", 1);
		on_collision_exit = GetFunction("OnCollisionExit", 1);
		on_enable = GetFunction("OnEnable", 0);
		on_disable = GetFunction("OnDisable", 0);

		ret = true;
	}
	
	return ret;
}

void CSScript::SetAttachedGameObject(GameObject * gameobject)
{
	attached_gameobject = gameobject;
	CreateSelfGameObject();
}

void CSScript::InitScript()
{
	if (init != nullptr)
	{
		CallFunction(init, nullptr);
		inside_function = false;
	}
}

void CSScript::StartScript()
{
	if (start != nullptr)
	{
		CallFunction(start, nullptr);
		inside_function = false;
	}
}

void CSScript::UpdateScript()
{
	if (update != nullptr)
	{
		CallFunction(update, nullptr);
		inside_function = false;
	}
}

void CSScript::OnCollisionEnter()
{
	if (on_collision_enter != nullptr)
	{
		CallFunction(on_collision_enter, nullptr); //nullptr should be the collision
		inside_function = false;
	}
}

void CSScript::OnCollisionStay()
{
	if (on_collision_stay != nullptr)
	{
		CallFunction(on_collision_stay, nullptr); //nullptr should be the collision
		inside_function = false;
	}
}

void CSScript::OnCollisionExit()
{
	if (on_collision_exit != nullptr)
	{
		CallFunction(on_collision_exit, nullptr); //nullptr should be the collision
		inside_function = false;
	}
}

void CSScript::OnEnable()
{
	if (on_enable != nullptr)
	{
		CallFunction(on_enable, nullptr);
		inside_function = false;
	}
}

void CSScript::OnDisable()
{
	if (on_disable != nullptr)
	{
		CallFunction(on_disable, nullptr);
		inside_function = false;
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
	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);
	if (field)
	{
		mono_field_get_value(mono_object, field, &value);
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

void CSScript::SetAssembly(MonoAssembly * mono_assembly)
{
	this->mono_assembly = mono_assembly;
}

void CSScript::SetNameSpace(std::string name_space)
{
	this->name_space = name_space;
}

void CSScript::SetClassName(std::string class_name)
{
	this->class_name = class_name;
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
	inside_function = true;
	MonoObject* exception = nullptr;
	MonoMarshalSpec* spec;

	MonoObject* obj = mono_runtime_invoke(function, mono_object, parameter, &exception);

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
		if(name == "TheEngine.TheGameObject") script_field.propertyType = ScriptField::GameObject;
		else if (name == "TheEngine.Texture") script_field.propertyType = ScriptField::Texture;
		else if (name == "TheEngine.Animation") script_field.propertyType = ScriptField::Animation;
		else if (name == "TheEngine.Audio") script_field.propertyType = ScriptField::Audio;
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

void CSScript::CreateSelfGameObject()
{
	MonoClass* c = mono_class_from_name(App->script_importer->GetEngineImage(), "TheEngine", "TheGameObject");
	if (c)
	{
		MonoObject* new_object = mono_object_new(mono_domain, c);
		if (new_object)
		{
			mono_self_object = new_object;
			created_gameobjects[mono_self_object] = attached_gameobject;
		}
	}
}

void CSScript::SetGameObjectName(MonoObject * object, MonoString* name)
{
	if (!MonoObjectIsValid(object))
	{
		return;
	}
	if (!GameObjectIsValid())
	{
		return;
	}

	if (name != nullptr)
	{
		const char* new_name = mono_string_to_utf8(name);
		active_gameobject->SetName(new_name);
	}
}

MonoString* CSScript::GetGameObjectName(MonoObject * object)
{
	if (!MonoObjectIsValid(object))
	{
		return nullptr;
	}
	if (!GameObjectIsValid())
	{
		return nullptr;
	}
	return mono_string_new(mono_domain, active_gameobject->GetName().c_str());
}

void CSScript::SetGameObjectTag(MonoObject * object, MonoString * tag)
{
	if (!MonoObjectIsValid(object))
	{
		return;
	}
	if (!GameObjectIsValid())
	{
		return;
	}
	if (tag != nullptr)
	{
		const char* new_tag = mono_string_to_utf8(tag);
		active_gameobject->SetName(new_tag);
	}
}

MonoString * CSScript::GetGameObjectTag(MonoObject * object)
{
	if (!MonoObjectIsValid(object))
	{
		return nullptr;
	}

	if (!GameObjectIsValid())
	{
		return nullptr;
	}
	return mono_string_new(mono_domain, active_gameobject->GetTag().c_str());
}

void CSScript::SetGameObjectLayer(MonoObject * object, MonoString * layer)
{
	if (!MonoObjectIsValid(object))
	{
		return;
	}
	if (!GameObjectIsValid())
	{
		return;
	}
	if (layer != nullptr)
	{
		const char* new_layer = mono_string_to_utf8(layer);
		active_gameobject->SetLayer(new_layer);
	}
}

MonoString * CSScript::GetGameObjectLayer(MonoObject * object)
{
	if (!MonoObjectIsValid(object))
	{
		return nullptr;
	}

	if (!GameObjectIsValid())
	{
		return nullptr;
	}
	return mono_string_new(mono_domain, active_gameobject->GetLayer().c_str());
}

void CSScript::SetGameObjectStatic(MonoObject * object, mono_bool value)
{
	if (!MonoObjectIsValid(object))
	{
		return;
	}

	if (!GameObjectIsValid())
	{
		return;
	}
	active_gameobject->SetStatic(value);
}

mono_bool CSScript::GameObjectIsStatic(MonoObject * object)
{
	if (!MonoObjectIsValid(object))
	{
		return false;
	}

	if (!GameObjectIsValid())
	{
		return false;
	}
	return active_gameobject->IsStatic();
}

MonoObject* CSScript::AddComponent(MonoObject * object, MonoReflectionType * type)
{
	if (!MonoObjectIsValid(object))
	{
		return nullptr;
	}

	if (!GameObjectIsValid())
	{
		return nullptr;
	}

	MonoType* t = mono_reflection_type_get_type(type);
	std::string name = mono_type_get_name(t);

	MonoClass* c = mono_class_from_name(App->script_importer->GetEngineImage(), "TheEngine", "TheTransform");
	if (c)
	{
		MonoObject* new_object = mono_object_new(mono_domain, c);
		if (new_object)
		{
			return new_object;
		}
	}
	return nullptr;

	Component::ComponentType comp_type = Component::CompUnknown;
	if (name == "TheEngine.TheTransform") CONSOLE_ERROR("Can't add Transform component to %s. GameObjects cannot have more than 1 transform.", active_gameobject->GetName().c_str());
	//else if (name == )

	if (comp_type != Component::CompUnknown)
	{
		active_gameobject->AddComponent(comp_type);
	}
}

MonoObject* CSScript::GetComponent(MonoObject * object, MonoReflectionType * type)
{
	if (!MonoObjectIsValid(object))
	{
		return nullptr;
	}

	if (!GameObjectIsValid())
	{
		return nullptr;
	}

	MonoType* t = mono_reflection_type_get_type(type);
	std::string name = mono_type_get_name(t);

	if (name == "TheEngine.TheTransform")
	{
		MonoClass* c = mono_class_from_name(App->script_importer->GetEngineImage(), "TheEngine", "TheTransform");
		if (c)
		{
			MonoObject* new_object = mono_object_new(mono_domain, c);
			if (new_object)
			{
				return new_object;
			}
		}
		return nullptr;
	}
}

void CSScript::SetPosition(MonoObject * object, MonoObject * vector3)
{
	if (!GameObjectIsValid())
	{
		return;
	}
	MonoClass* c = mono_object_get_class(vector3);
	MonoClassField* x_field = mono_class_get_field_from_name(c, "x");
	MonoClassField* y_field = mono_class_get_field_from_name(c, "y");
	MonoClassField* z_field = mono_class_get_field_from_name(c, "z");

	float3 new_pos;

	if (x_field) mono_field_get_value(vector3, x_field, &new_pos.x);
	if (y_field) mono_field_get_value(vector3, y_field, &new_pos.y);
	if (z_field) mono_field_get_value(vector3, z_field, &new_pos.z);

	ComponentTransform* transform = (ComponentTransform*)active_gameobject->GetComponent(Component::CompTransform);
	transform->SetPosition(new_pos);
}

MonoObject* CSScript::GetPosition(MonoObject * object)
{
	if (!GameObjectIsValid())
	{
		return nullptr;
	}
	MonoString* str = mono_object_to_string(object, nullptr);
	const char* message = mono_string_to_utf8(str);
	MonoClass* c = mono_class_from_name(App->script_importer->GetEngineImage(), "TheEngine", "TheVector3");
	if (c)
	{
		MonoObject* new_object = mono_object_new(mono_domain, c);
		if (new_object)
		{
			MonoClassField* x_field = mono_class_get_field_from_name(c, "x");
			MonoClassField* y_field = mono_class_get_field_from_name(c, "y");
			MonoClassField* z_field = mono_class_get_field_from_name(c, "z");

			ComponentTransform* transform = (ComponentTransform*)active_gameobject->GetComponent(Component::CompTransform);
			float3 new_pos = transform->GetGlobalPosition();

			if (x_field) mono_field_set_value(new_object, x_field, &new_pos.x);
			if (y_field) mono_field_set_value(new_object, y_field, &new_pos.y);
			if (z_field) mono_field_set_value(new_object, z_field, &new_pos.z);

			return new_object;
		}
	}
	return nullptr;
}

mono_bool CSScript::IsKeyDown(MonoString * key_name)
{
	bool ret = false;
	const char* key = mono_string_to_utf8(key_name);
	SDL_Keycode code = App->input->StringToKey(key);
	if (code != SDL_SCANCODE_UNKNOWN)
	{
		if (App->input->GetKey(App->input->StringToKey(key) == KEY_REPEAT)) ret = true;
	}
	else
	{
		CONSOLE_WARNING("'%s' is not a key! Returned false by default", key);
	}

	return ret;
}

mono_bool CSScript::IsKeyUp(MonoString * key_name)
{
	bool ret = false;
	const char* key = mono_string_to_utf8(key_name);
	SDL_Keycode code = App->input->StringToKey(key);
	if (code != SDL_SCANCODE_UNKNOWN)
	{
		if (App->input->GetKey(App->input->StringToKey(key) == KEY_REPEAT)) ret = true;
	}
	else
	{
		CONSOLE_WARNING("'%s' is not a key! Returned false by default", key);
	}

	return ret;
}

mono_bool CSScript::IsKeyRepeat(MonoString * key_name)
{
	bool ret = false;
	const char* key = mono_string_to_utf8(key_name);
	SDL_Keycode code = App->input->StringToKey(key);
	if (code != SDL_SCANCODE_UNKNOWN)
	{
		if (App->input->GetKey(App->input->StringToKey(key) == KEY_REPEAT)) ret = true;
	}
	else
	{
		CONSOLE_WARNING("'%s' is not a key! Returned false by default", key);
	}
	
	return ret;
}

mono_bool CSScript::IsMouseDown(int mouse_button)
{
	bool ret = false;
	if (mouse_button >= 0 && mouse_button < 4)
	{
		if (App->input->GetMouseButton(mouse_button) == KEY_DOWN) ret = true;
	}
	else
	{
		CONSOLE_WARNING("%d is not a valid mouse button! Returned false by default");
	}

	return ret;
}

mono_bool CSScript::IsMouseUp(int mouse_button)
{
	bool ret = false;
	if (mouse_button >= 0 && mouse_button < 4)
	{
		if (App->input->GetMouseButton(mouse_button) == KEY_UP) ret = true;
	}
	else
	{
		CONSOLE_WARNING("%d is not a valid mouse button! Returned false by default");
	}

	return ret;
}

mono_bool CSScript::IsMouseRepeat(int mouse_button)
{
	bool ret = false;
	if (mouse_button >= 0 && mouse_button < 4)
	{
		if (App->input->GetMouseButton(mouse_button) == KEY_REPEAT) ret = true;
	}
	else
	{
		CONSOLE_WARNING("%d is not a valid mouse button! Returned false by default");
	}

	return ret;
}

void CSScript::CreateGameObject(MonoObject * object)
{
	if (!inside_function)
	{
		CONSOLE_ERROR("Can't create new GameObjects outside a function.");
		return;
	}
	GameObject* gameobject = App->scene->CreateGameObject();
	created_gameobjects[object] = gameobject;
}

MonoObject* CSScript::SetSelfGameObject()
{
	if (!GameObjectIsValid())
	{
		return nullptr;
	}
	else
	{
		if (mono_self_object != nullptr)
		{
			return mono_self_object;
		}
	}
}

void CSScript::SetGameObjectActive(MonoObject * object, mono_bool active)
{
	if (!MonoObjectIsValid(object))
	{
		return;
	}

	if (!GameObjectIsValid())
	{
		return;
	}
	active_gameobject->SetActive(active);
}

bool CSScript::GetGameObjectActive(MonoObject * object)
{
	if (!MonoObjectIsValid(object))
	{
		return false;
	}

	if (!GameObjectIsValid())
	{
		return false;
	}
	return active_gameobject->IsActive();
}

void CSScript::Save(Data & data) const
{
	data.AddString("library_path", GetLibraryPath());
	data.AddString("assets_path", GetAssetsPath());
	data.AddString("script_name", GetName());
	data.AddUInt("UUID", GetUID());
}

bool CSScript::Load(Data & data)
{
	bool ret = true;
	std::string library_path = data.GetString("library_path");

	Script* text = App->script_importer->LoadScriptFromLibrary(library_path);
	if (!text)
	{
		std::string assets_path = data.GetString("assets_path");
		if (App->file_system->FileExist(assets_path))
		{
			library_path = App->resources->CreateLibraryFile(Resource::ScriptResource, assets_path);
			if (!library_path.empty())
			{
				Load(data);
			}
		}
		else
		{
			ret = false;
		}
	}
	else
	{
		SetAssetsPath(data.GetString("assets_path"));
		SetLibraryPath(data.GetString("library_path"));
		SetName(data.GetString("script_name"));
		SetUID(data.GetUInt("UUID"));
	}

	return ret;
}

void CSScript::CreateMeta() const
{
	time_t now = time(0);
	char* dt = ctime(&now);

	Data data;
	data.AddInt("Type", GetType());
	data.AddUInt("UUID", GetUID());
	data.AddString("Time_Created", dt);
	data.AddString("Library_path", GetLibraryPath());
	data.SaveAsMeta(GetAssetsPath());
}

void CSScript::LoadToMemory()
{
}

void CSScript::UnloadFromMemory()
{
}

bool CSScript::MonoObjectIsValid(MonoObject* object)
{
	if (object != nullptr)
	{
		/*if (!modifying_self) 
		else modifying_self = false;*/
		active_gameobject = created_gameobjects[object];
		return true;
	}
	return false;
}

bool CSScript::GameObjectIsValid()
{
	if (active_gameobject == nullptr)
	{
		CONSOLE_ERROR("You are trying to modify a null GameObject")
			return false;
	}
	return true;
}
