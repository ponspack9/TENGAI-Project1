#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment(lib,"SDL_mixer/libx86/SDL2_mixer.lib")

struct SDL_Texture;
struct Collider;

class ModulePlayer : public Module
{
public:
	ModulePlayer();
	~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	bool Shield_Animation = false;
	void OnCollision(Collider* c1, Collider* c2);

public:

	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation idle;
	Animation backward;
	Animation run;
	Animation die;
	Animation shield;
	iPoint position;
	Collider* player_collider;
	Collider* bullet_collider;

	iPoint screen_position;
	int distance_forward;
	int distance_backward;
	int distance_up;
	int distance_down;

	Mix_Chunk* MikosShot = nullptr;
};

#endif