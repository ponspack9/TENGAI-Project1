#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleJunis.h"
#include "ModuleAudio.h"

ModuleJunis::ModuleJunis()
{
	graphics = NULL;
	current_animation = NULL;

	path_spawn.PushBack({ 0.025f, 0.0f }, 100, &touch);

	path_die.PushBack({ 0.0f, 0.0f }, 2);
	path_die.PushBack({ -0.1f,-0.35f }, 5);
	path_die.PushBack({ -0.25f, 0.25f }, 15);
	path_die.PushBack({ -0.1f, 0.4f }, 40);

	// idle animation
	idle.PushBack({ 4, 0, 27, 24 });
	idle.PushBack({ 46, 0, 27, 24 });
	idle.PushBack({ 89, 1, 27, 24 });
	idle.speed = 0.10f;

	// junis touched animation
	touch.PushBack({ 4, 0, 27, 24 });
	touch.PushBack({ 1,1,1,1 });
	touch.PushBack({ 46, 0, 27, 24 });
	touch.PushBack({ 1,1,1,1 });
	touch.PushBack({ 89, 1, 27, 24 });
	touch.PushBack({ 1,1,1,1 });
	touch.speed = 0.1f;

	// backward animation (arcade sprite sheet)
	backward.PushBack({ 79,33,16,28 });
	backward.speed = 0.15f;

	// run animation (arcade sprite sheet)
	run.PushBack({ 4, 0, 27, 24 });
	run.PushBack({ 46, 0, 27, 24 });
	run.PushBack({ 89, 1, 27, 24 });
	run.speed = 0.10f;
	/*run.PushBack({ 72,7,33,35 });
	run.PushBack({ 108,7,33,35 });
	run.PushBack({ 145,7,33,35 });
	run.PushBack({ 190,7,33,35 });
	run.PushBack({ 230,7,33,35 });
	run.PushBack({ 270,7,33,35 });
	run.PushBack({ 308,7,33,35 });
	run.PushBack({ 349,7,33,35 });
	run.speed = 0.19f;*/

	// die animation 
	die.PushBack({ 1,73,23,25 });

	// shield animation
	shield.PushBack({ 37,111,19,26 });
	shield.PushBack({ 71,112,14,24 });
	shield.PushBack({ 102,110,19,26 });
	shield.PushBack({ 5,110,15,26 });
	shield.PushBack({ 37,111,19,26 });
	shield.speed = 0.1f;

}

ModuleJunis::~ModuleJunis()
{}

// Load assets
bool ModuleJunis::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("tengai/junisSpritesheet.png");
	JunisShot = App->audio->LoadFx("audio/JunisShot.wav");
	JunisCollision = App->audio->LoadFx("audio/JunisCollision.wav");

	position.x = 10;
	position.y = 150;
	alive = true;
	player_collider = App->collision->AddCollider({ position.x, position.y, 27, 28 }, COLLIDER_PLAYER, this);

	return true;

}

// Unload assets
bool ModuleJunis::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

// Update: draw background
update_status ModuleJunis::Update()
{
	int camera_x = (-App->render->camera.x / 2);// Divided by camera.speed;

	if (alive) {
		if (shield.Finished())
		{
			Shield_Animation = false;
			shield.Reset();
		}
		else if (Spawn_Animation) {
			Spawn_Animation = Spawn();
		}
		else
		{
			if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_STATE::KEY_REPEAT)
			{
				if (!Shield_Animation)current_animation = &backward;
				if (position.x - speed > camera_x - 2)
				{
					position.x -= speed;
				}
			}
			else if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_STATE::KEY_REPEAT)
			{
				if (!Shield_Animation)current_animation = &idle;
				if (position.y > SCREEN_HEIGHT - 43) current_animation = &run;
				if (position.x + 29 + speed < SCREEN_WIDTH + camera_x)
				{
					position.x += speed;
				}
			}
			if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_STATE::KEY_REPEAT)
			{
				if (!Shield_Animation)current_animation = &backward;
				if (position.y - speed > -2)
				{
					position.y -= speed;
				}
			}
			else if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_STATE::KEY_REPEAT)
			{
				if (!Shield_Animation)current_animation = &idle;
				if (position.y + 31 + speed < SCREEN_HEIGHT)
				{
					position.y += speed;
				}
			}
			if (App->input->keyboard[SDL_SCANCODE_RCTRL] == KEY_STATE::KEY_DOWN)
			{
				App->particles->AddParticle(App->particles->Jshot, position.x + 31, position.y + 6, COLLIDER_PLAYER_SHOT);
				Mix_PlayChannel(-1, JunisShot, 0);
			}
			if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_STATE::KEY_IDLE
				&& App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_STATE::KEY_IDLE
				&& App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_STATE::KEY_IDLE
				&& App->input->keyboard[SDL_SCANCODE_UP] == KEY_STATE::KEY_IDLE
				&& !Shield_Animation)
			{
				if (position.y > SCREEN_HEIGHT - 43) current_animation = &run;
				else current_animation = &idle;
			}
			if (App->input->keyboard[SDL_SCANCODE_F3] == KEY_STATE::KEY_DOWN)
			{
				current_animation = &die;
				player_collider->to_delete = true;
				alive = false;
			}
		}
		player_collider->SetPos(position.x, position.y);
	}
	// if dead
	else {
		if (!path_die.loop) 
		{
			position += path_die.GetCurrentSpeed();
		}
		else if (JunisLife != 3)
		{
			JunisLife++;
			Spawn();
		}
	}

	App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));

	return UPDATE_CONTINUE;	
}

void ModuleJunis::Die() {
	path_die.Reset();
	alive = false;
	current_animation = &die;
	Mix_PlayChannel(-1, JunisCollision, 0);
	player_collider->to_delete = true;
}

bool ModuleJunis::Spawn() {
	//first time is called, spawn behind camera
	if (!Spawn_Animation)
	{
		path_spawn.Reset();
		current_animation = &touch;
		position = iPoint(-App->render->camera.x / 2, 50);
		player_collider = App->collision->AddCollider({ position.x, position.y, 27, 28 }, COLLIDER_PLAYER, this);
		Spawn_Animation = true;
		alive = true;
	}
	//Actually moving behind the camera
	else {
		position += path_spawn.GetCurrentSpeed();
	}
	//if is finished
	return !path_spawn.loop;
}

void ModuleJunis::OnCollision(Collider* c1, Collider* c2)
{
	Shield_Animation = (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_ENEMY) || (c2->type == COLLIDER_PLAYER && c1->type == COLLIDER_ENEMY);
	if (Shield_Animation)
	{
		if (power_ups > 0) { current_animation = &shield; }
		else if (alive) { Die(); }
	}

	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_ENEMY_SHOT)
	{
		if (Jlife == 1)
		{
			if (alive)
			{
				Die();
				Jlife = 3;
			}
		}
		else
		{
			Jlife--;
		}
	}
}