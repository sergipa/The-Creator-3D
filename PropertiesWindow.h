#pragma once

#include "Window.h"
#include <map>
//#include <afx.h>

class PropertiesWindow :
	public Window
{
public:
	PropertiesWindow();
	virtual ~PropertiesWindow();

	void DrawWindow();
	//void DrawComponent(Component* component);
	//void DrawTransformPanel(ComponentTransform* transform);
	//void DrawSpriteRendererPanel(ComponentSpriteRenderer* spriteRenderer);
	//void DrawAnimatonPanel(ComponentAnimation* animator);
	//void DrawAudioPanel(ComponentAudio* audio);
	//void DrawParticlePanel(ComponentParticle* particle);
	//void DrawScriptPanel(ComponentScript* script);
	//bool ScriptHasChanged(string path);
	//void UpdateScript(ComponentScript * script);

private:
	//std::map<std::string, CTime> scriptsChangesData;
	int anim_index = 0;
	int sound_index = 0;
	int music_index = 0;
	int particle_FX_index = 0;
};

