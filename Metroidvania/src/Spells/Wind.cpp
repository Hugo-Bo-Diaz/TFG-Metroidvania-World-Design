#include "Wind.h"
#include "../GameObjects/Player.h"
#include "Application.h"
#include "Modules/Input.h"
#include "Modules/Render.h"
#include "../GameObjects/SpellProjectiles/WindSlash.h"
#include "Modules/Camera.h"
#include "Utils/Utils.h"

Wind::~Wind()
{
	Engine->GetModule<::Render>().RemoveParticleEmitter(p);
	//Engine->GetModule<::Render>().to_delete.push_back(p);
}

void Wind::Init()
{
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/particles.png", particle);

	r6buff = { 12,12,12,12 };
	r7buff = { 24,12,12,12 };
	windbuff.area_in_texture.push_back(&r6buff);
	windbuff.area_in_texture.push_back(&r7buff);
	windbuff.name = "windbuff";
	windbuff.minmax_speed_y = std::make_pair(-1, 1);
	windbuff.minmax_speed_x = std::make_pair(-1, 1);
	windbuff.minmax_lifespan = std::make_pair(75, 150);
	windbuff.minmax_x_offset = std::make_pair(0, 48);
	windbuff.minmax_y_offset = std::make_pair(40, 48);
	windbuff.minmax_frequency = std::make_pair(5, 10);
	windbuff.texture_name = particle;

	p = Engine->GetModule<::Render>().AddParticleEmitter(&windbuff, 0, 0);
	p->active = false;
}

void Wind::Loop(float dt)
{
	p->position_x = player->collider.x;
	p->position_y = player->collider.y;

	//Windslash--------------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(X) == BUTTON_DOWN)
	if (player->manaCost(0.2) && Engine->GetModule<Input>().GetInput(BUTTON_2)==KEY_DOWN)
	{
		WindSlash* windslash = (WindSlash*)Engine->GetModule<SceneController>().AddObject(player->collider.x, player->collider.y, 48, 48, GetTypeIndex<WindSlash>());
		windslash->Fire(player->is_right);
		Engine->GetModule<Camera>().CameraShake(7, 100);
		player->AddMana(-0.2);
	}

	//Superjump------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(Y) == BUTTON_DOWN && jump_available)
	if (Engine->GetModule<Input>().GetInput(BUTTON_3) == KEY_DOWN && jump_available && player->manaCost(0.3))
	{
		player->speed_y = -jump_force;
		jump_available = false;
		player->grounded = false;
		player->acceleration_y = 0.3;
		Engine->GetModule<::Render>().AddParticleEmitter(&windbuff, player->collider.x, player->collider.y, 400);
		player->AddMana(-0.3);
	}

	if (!jump_available && player->grounded)
	{
		player->acceleration_y = 0.5;
		jump_available=true;
	}

	//Mobility--------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(B) == BUTTON_DOWN && !is_mobility_on_cooldown)
	if (Engine->GetModule<Input>().GetInput(BUTTON_4) == KEY_DOWN && !is_mobility_on_cooldown && player->manaCost(1.0))
	{
		is_mobility_up = true;
		is_mobility_on_cooldown = true;
		mobility_timer.Start();
		mobility_timer.Reset();
		player->AddMana(-1.0);
	}

	if (mobility_timer.Read() > mobility_max_time)
	{
		is_mobility_up = false;
		player->mobility_multiplier = 1;
		p->active = false;
	}

	if (mobility_timer.Read() > cooldown_mobility)
	{
		is_mobility_on_cooldown = false;
	}

	if (is_mobility_up)
	{
		p->active = true;
		player->mobility_multiplier = 1.5;
	}
}

void Wind::UnlockMovementEvent()
{

}

void Wind::Switched_in()
{
	is_mobility_up = false;
	player->mobility_multiplier = 1;
	p->active = false;
}

void Wind::Switched_out()
{
	is_mobility_up = false;
	player->mobility_multiplier = 1;
	p->active = false;
}


void Wind::CleanUp()
{

}