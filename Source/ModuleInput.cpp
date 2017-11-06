#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleImport.h"
#include "PerformanceWindow.h"
#include "ModuleRenderer3D.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled, bool is_game) : Module(app, start_enabled, is_game)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
	name = "Input";
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init(Data* editor_config)
{
	CONSOLE_DEBUG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		CONSOLE_DEBUG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	StoreStringKeys();
	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	ms_timer.Start();
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x;
	mouse_y;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		App->editor->HandleInput(&e);

		switch(e.type)
		{

			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x;
			mouse_y = e.motion.y;

			mouse_x_motion = e.motion.xrel;
			mouse_y_motion = e.motion.yrel;
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if (e.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
					App->window->SetWidth(e.window.data1);
					App->window->SetHeight(e.window.data2);
				}
			}
				break;
			case SDL_DROPFILE:
				App->import->LoadFile(e.drop.file);
				SDL_free(e.drop.file);
				break;
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;
	App->editor->performance_window->AddModuleData(this->name, ms_timer.ReadMs());
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	CONSOLE_DEBUG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

void ModuleInput::StoreStringKeys()
{
	stringKeys["A"] = SDL_SCANCODE_A;
	stringKeys["B"] = SDL_SCANCODE_B;
	stringKeys["C"] = SDL_SCANCODE_C;
	stringKeys["D"] = SDL_SCANCODE_D;
	stringKeys["E"] = SDL_SCANCODE_E;
	stringKeys["F"] = SDL_SCANCODE_F;
	stringKeys["G"] = SDL_SCANCODE_G;
	stringKeys["H"] = SDL_SCANCODE_H;
	stringKeys["I"] = SDL_SCANCODE_I;
	stringKeys["J"] = SDL_SCANCODE_J;
	stringKeys["K"] = SDL_SCANCODE_K;
	stringKeys["L"] = SDL_SCANCODE_L;
	stringKeys["M"] = SDL_SCANCODE_M;
	stringKeys["N"] = SDL_SCANCODE_N;
	stringKeys["O"] = SDL_SCANCODE_O;
	stringKeys["P"] = SDL_SCANCODE_P;
	stringKeys["Q"] = SDL_SCANCODE_Q;
	stringKeys["R"] = SDL_SCANCODE_R;
	stringKeys["S"] = SDL_SCANCODE_S;
	stringKeys["T"] = SDL_SCANCODE_T;
	stringKeys["U"] = SDL_SCANCODE_U;
	stringKeys["V"] = SDL_SCANCODE_V;
	stringKeys["W"] = SDL_SCANCODE_W;
	stringKeys["X"] = SDL_SCANCODE_X;
	stringKeys["Y"] = SDL_SCANCODE_Y;
	stringKeys["Z"] = SDL_SCANCODE_Z;
	stringKeys["0"] = SDL_SCANCODE_0;
	stringKeys["1"] = SDL_SCANCODE_1;
	stringKeys["2"] = SDL_SCANCODE_2;
	stringKeys["3"] = SDL_SCANCODE_3;
	stringKeys["4"] = SDL_SCANCODE_4;
	stringKeys["5"] = SDL_SCANCODE_5;
	stringKeys["6"] = SDL_SCANCODE_6;
	stringKeys["7"] = SDL_SCANCODE_7;
	stringKeys["8"] = SDL_SCANCODE_8;
	stringKeys["9"] = SDL_SCANCODE_9;
	stringKeys["ESC"] = SDL_SCANCODE_ESCAPE;
	stringKeys["L_CTRL"] = SDL_SCANCODE_LCTRL;
	stringKeys["L_SHIFT"] = SDL_SCANCODE_LSHIFT;
	stringKeys["L_ALT"] = SDL_SCANCODE_LALT;
	stringKeys["R_CTRL"] = SDL_SCANCODE_RCTRL;
	stringKeys["R_SHIFT"] = SDL_SCANCODE_RSHIFT;
	stringKeys["R_ALT"] = SDL_SCANCODE_RALT;
	stringKeys["MENU"] = SDL_SCANCODE_MENU;
	stringKeys["["] = SDL_SCANCODE_LEFTBRACKET;
	stringKeys["]"] = SDL_SCANCODE_RIGHTBRACKET;
	stringKeys[";"] = SDL_SCANCODE_SEMICOLON;
	stringKeys[","] = SDL_SCANCODE_COMMA;
	stringKeys["."] = SDL_SCANCODE_PERIOD;
	stringKeys["'"] = SDL_SCANCODE_APOSTROPHE;
	stringKeys["/"] = SDL_SCANCODE_SLASH;
	stringKeys["BACK_SLASH"] = SDL_SCANCODE_BACKSLASH;
	stringKeys["~"] = SDL_SCANCODE_GRAVE;
	stringKeys["="] = SDL_SCANCODE_EQUALS;
	stringKeys["-"] = SDL_SCANCODE_ALTERASE;
	stringKeys["SPACE"] = SDL_SCANCODE_SPACE;
	stringKeys["RETURN"] = SDL_SCANCODE_RETURN;
	stringKeys["BACK_SPACE"] = SDL_SCANCODE_BACKSPACE;
	stringKeys["TAB"] = SDL_SCANCODE_TAB;
	stringKeys["PAGE_UP"] = SDL_SCANCODE_PAGEUP;
	stringKeys["PAGE_DOWN"] = SDL_SCANCODE_PAGEDOWN;
	stringKeys["END"] = SDL_SCANCODE_END;
	stringKeys["HOME"] = SDL_SCANCODE_HOME;
	stringKeys["INSERT"] = SDL_SCANCODE_INSERT;
	stringKeys["DELETE"] = SDL_SCANCODE_DELETE;
	stringKeys["+"] = SDL_SCANCODE_KP_PLUS;
	stringKeys["-"] = SDL_SCANCODE_KP_MINUS;
	stringKeys["*"] = SDL_SCANCODE_KP_MULTIPLY;
	stringKeys["/"] = SDL_SCANCODE_KP_DIVIDE;
	stringKeys["LEFT_ARROW"] = SDL_SCANCODE_LEFT;
	stringKeys["RIGHT_ARROW"] = SDL_SCANCODE_RIGHT;
	stringKeys["UP_ARROW"] = SDL_SCANCODE_UP;
	stringKeys["DOWN_ARROW"] = SDL_SCANCODE_DOWN;
	stringKeys["NUM_PAD_0"] = SDL_SCANCODE_KP_0;
	stringKeys["NUM_PAD_1"] = SDL_SCANCODE_KP_1;
	stringKeys["NUM_PAD_2"] = SDL_SCANCODE_KP_2;
	stringKeys["NUM_PAD_3"] = SDL_SCANCODE_KP_3;
	stringKeys["NUM_PAD_4"] = SDL_SCANCODE_KP_4;
	stringKeys["NUM_PAD_5"] = SDL_SCANCODE_KP_5;
	stringKeys["NUM_PAD_6"] = SDL_SCANCODE_KP_6;
	stringKeys["NUM_PAD_7"] = SDL_SCANCODE_KP_7;
	stringKeys["NUM_PAD_8"] = SDL_SCANCODE_KP_8;
	stringKeys["NUM_PAD_9"] = SDL_SCANCODE_KP_9;
	stringKeys["F1"] =  SDL_SCANCODE_F1;
	stringKeys["F2"] =  SDL_SCANCODE_F2;
	stringKeys["F3"] =  SDL_SCANCODE_F3;
	stringKeys["F4"] =  SDL_SCANCODE_F4;
	stringKeys["F5"] =  SDL_SCANCODE_F5;
	stringKeys["F6"] =  SDL_SCANCODE_F6;
	stringKeys["F7"] =  SDL_SCANCODE_F7;
	stringKeys["F8"] =  SDL_SCANCODE_F8;
	stringKeys["F9"] =  SDL_SCANCODE_F9;
	stringKeys["F10"] = SDL_SCANCODE_F10;
	stringKeys["F11"] = SDL_SCANCODE_F11;
	stringKeys["F12"] = SDL_SCANCODE_F12;
	stringKeys["F13"] = SDL_SCANCODE_F13;
	stringKeys["F14"] = SDL_SCANCODE_F14;
	stringKeys["F15"] = SDL_SCANCODE_F15;
	stringKeys["PAUSE"] = SDL_SCANCODE_PAUSE;
}

SDL_Keycode ModuleInput::StringToKey(std::string key)
{
	SDL_Keycode ret;
	if (stringKeys.find(key) != stringKeys.end()) {
		ret = stringKeys[key];
	}
	else {
		ret = SDL_SCANCODE_UNKNOWN;
	}
	return ret;
}

std::string ModuleInput::KeyToString(SDL_Keycode key)
{
	std::string ret;
	for (std::map<std::string, SDL_Keycode>::iterator it = stringKeys.begin(); it != stringKeys.end(); ++it)
		if (it->second == key)
			return it->first;
}
