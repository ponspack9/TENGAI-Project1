#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"

#define NUM_MODULES 10

class ModuleWindow;
class ModuleInput;
class ModuleTextures;
class ModuleRender;
class ModuleBackground;
class ModulePlayer;
class ModulePlayerMotor;
class ModuleUI;
class ModuleAudio;
class ModuleBullet;
class Module;

class Application
{
public:

	Module *				modules[NUM_MODULES]		= { nullptr };
	ModuleWindow*		window					= nullptr;
	ModuleRender*		render					= nullptr;
	ModuleInput*			input					= nullptr;
	ModuleTextures*		textures					= nullptr;
	ModuleBackground*	background				= nullptr;
	ModulePlayer*		player					= nullptr;
	ModulePlayerMotor*	playerMotor				= nullptr;
	ModuleUI*			UI						= nullptr;
	ModuleAudio*			audio					= nullptr;
	ModuleBullet*		bullet					= nullptr;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

};

// Global var made extern for Application ---
extern Application* App;

#endif // __APPLICATION_H__