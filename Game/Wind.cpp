#include "Wind.h"
#include "Player.h"
#include "Application.h"
#include "Input.h"
#include "Physics.h"
#include "Render.h"
#include "WindSlash.h"

void Wind::Init()
{

}

void Wind::Loop(float dt)
{
	//Windslash--------------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(X) == BUTTON_DOWN)
	if (App->inp->GetInput(BUTTON_2)==KEY_DOWN)
	{
		WindSlash* windslash = (WindSlash*)App->phy->AddObject(player->collider->x, player->collider->y, 48, 48, WIND_SLASH);
		windslash->Fire(player->is_right);
	}

	//Superjump------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(Y) == BUTTON_DOWN && jump_available)
	if (App->inp->GetInput(BUTTON_3) == KEY_DOWN && jump_available)
	{
		player->speed_y = -jump_force;
		jump_available = false;
	}

	if (!jump_available && player->grounded)
	{
		jump_available=true;
	}

	//Mobility--------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(B) == BUTTON_DOWN && !is_mobility_on_cooldown)
	if (App->inp->GetInput(BUTTON_4) == KEY_DOWN && !is_mobility_on_cooldown)
	{
		player->LockMovement(mobility_activation_time);
		is_mobility_on_cooldown = true;
		mobility_timer.Start();
		mobility_timer.Reset();
	}

	if (mobility_timer.Read() > mobility_activation_time && !is_mobility_up)
	{
		is_mobility_up = true;
	}

	if (mobility_timer.Read() > mobility_max_time)
	{
		is_mobility_up = false;
		player->mobility_multiplier = 1;
	}

	if (mobility_timer.Read() > cooldown_mobility)
	{
		is_mobility_on_cooldown = false;
	}

	if (is_mobility_up)
	{
		player->mobility_multiplier = 1.5;
	}
}

void Wind::UnlockMovementEvent()
{

}


void Wind::CleanUp()
{

}