#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleSho.h"
#include "ModuleAudio.h"

ModuleSho::ModuleSho()
{
	graphics = NULL;
	current_animation = NULL;

	position.x = 0;
	position.y = 120;

	// idle animation
	idle.PushBack({ 36, 8, 29, 27 });
	idle.PushBack({ 73, 9, 29, 26 });
	idle.PushBack({ 110, 9, 29, 27 });
	idle.loop = true;
	idle.speed = 0.10f;

	// backward animation (arcade sprite sheet)
	backward.PushBack({ 147,8,24,27 });
	backward.speed = 0.15f;

	// run animation (arcade sprite sheet)
	run.PushBack({ 72,7,33,35 });
	run.PushBack({ 108,7,33,35 });
	run.PushBack({ 145,7,33,35 });
	run.PushBack({ 190,7,33,35 });
	run.PushBack({ 230,7,33,35 });
	run.PushBack({ 270,7,33,35 });
	run.PushBack({ 308,7,33,35 });
	run.PushBack({ 349,7,33,35 });
	run.speed = 0.19f;

	// die animation 
	die.PushBack({ 630,7,35,35 });

	// shield animation
	shield.PushBack({ 673,7,35,35 });
	shield.PushBack({ 713,7,35,35 });
	shield.PushBack({ 750,7,35,35 });
	shield.PushBack({ 795,7,35,35 });
	shield.PushBack({ 830,7,35,35 });
	shield.PushBack({ 870,7,35,35 });
	shield.PushBack({ 911,7,35,35 });
	shield.speed = 0.1f;

}

ModuleSho::~ModuleSho()
{}

// Load assets
bool ModuleSho::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("tengai/shoSpritesheet.png");

	position.x = 10;
	position.y = 150;
	screen_position.x = 10;
	screen_position.y = 150;

	player_collider = App->collision->AddCollider({ position.x, position.y, 35, 31 }, COLLIDER_PLAYER, this);
	//bullet_collider = App->collision->AddCollider({ position.x + 31, position.y + 6,12,12 }, COLLIDER_PLAYER_SHOT);

	return true;

}

// Unload assets
bool ModuleSho::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

// Update: draw background
update_status ModuleSho::Update()
{
	int speed = 5;

	if (shield.Finished())
	{
		Shield_Animation = false;
		shield.Reset();
	}

	if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_STATE::KEY_REPEAT)
	{

		if (!Shield_Animation)current_animation = &backward;
		if (screen_position.x - speed > -10)
		{
			position.x -= speed;
			screen_position.x -= speed;
		}
	}
	else if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_STATE::KEY_REPEAT)
	{
		if (!Shield_Animation)current_animation = &idle;
		if (screen_position.x + speed < SCREEN_WIDTH - current_animation->GetCurrentFrame().w)
		{
			position.x += speed;
			screen_position.x += speed;
		}

	}

	if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_STATE::KEY_REPEAT)
	{
		if (!Shield_Animation)current_animation = &backward;
		if (screen_position.y - speed > -5)
		{
			position.y -= speed;
			screen_position.y -= speed;
		}
	}

	else if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_STATE::KEY_REPEAT)
	{
		if (!Shield_Animation)current_animation = &idle;
		if (screen_position.y + speed < SCREEN_HEIGHT - current_animation->GetCurrentFrame().h)
		{
			position.y += speed;
			screen_position.y += speed;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_RSHIFT] == KEY_STATE::KEY_DOWN)
	{
		//App->particles->AddParticle(App->particles->Mshot, position.x + 31, position.y + 6, COLLIDER_PLAYER_SHOT);
		//MikosShot = App->audio->LoadFx("audio/MikosShot.wav");
		//Mix_PlayChannel(-1, MikosShot, 0);
	}

	if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_STATE::KEY_IDLE
		&& App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_STATE::KEY_IDLE
		&& App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_STATE::KEY_IDLE
		&& App->input->keyboard[SDL_SCANCODE_UP] == KEY_STATE::KEY_IDLE
		&& !Shield_Animation)
	{
		current_animation = &idle;
	}

	//Update collider position to player position

	player_collider->SetPos(position.x, position.y);
	//bullet_collider->SetPos(App->particles->Mshot.position.x, App->particles->Mshot.position.y);

	// Draw everything --------------------------------------

	App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));

	return UPDATE_CONTINUE;
}

void ModuleSho::OnCollision(Collider* c1, Collider* c2)
{
	Shield_Animation = (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_ENEMY) || (c2->type == COLLIDER_PLAYER && c1->type == COLLIDER_ENEMY);
	if (Shield_Animation)
	{
		current_animation = &shield;
		
	}
}