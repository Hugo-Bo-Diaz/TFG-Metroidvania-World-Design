#include "Water.h"
#include "Player.h"
#include "Application.h"
#include "Input.h"
#include "Physics.h"
#include "Render.h"
#include "IceShard.h"
#include "IceBlock.h"
#include "Cloud.h"

void Water::Init()
{

}

void Water::Loop(float dt)
{
	//iceshard--------------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(X) == BUTTON_DOWN)
	if (App->inp->GetInput(BUTTON_2) == KEY_DOWN)
	{
		IceShard* iceshard = (IceShard*)App->phy->AddObject(player->collider->x, player->collider->y+player->collider->h/2-12, 48, 24, ICE_SHARD);
		iceshard->Fire(player->is_right);
	}

	//Iceblock------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(Y) == BUTTON_DOWN && !is_iceblock_on_cooldown)
	if (App->inp->GetInput(BUTTON_3) == KEY_DOWN && !is_iceblock_on_cooldown)
	{
		if (player->is_right)
		{
			IceBlock* block = (IceBlock*)App->phy->AddObject(player->collider->x + player->collider->w, player->collider->y, 64, 64, ICE_BLOCK);
			block->Init();
		}
		else
		{
			IceBlock* block = (IceBlock*)App->phy->AddObject(player->collider->x - 64, player->collider->y, 64, 64, ICE_BLOCK);
			block->Init();
		}

		player->LockMovement(cast_time_block);
		iceblock_timer.Reset();
		iceblock_timer.Start();
	}

	if (is_iceblock_on_cooldown && iceblock_timer.Read()>cooldown_block)
	{
		is_iceblock_on_cooldown = false;
	}

	////Cloud--------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(B) == BUTTON_DOWN && !is_cloud_on_cooldown)
	if (App->inp->GetInput(BUTTON_4) == KEY_DOWN && !is_cloud_on_cooldown)
	{

		float joystickx, joysticky;
		App->inp->GetJoystick(true, joystickx, joysticky);



		if (joystickx < 0.15 && joystickx>-0.15)
		{
			if (player->is_right)
			{
				current_cloud= (Cloud*)App->phy->AddObject(player->collider->x + player->collider->w + min_radius, player->collider->y, 64, 32, CLOUD);
			}
			else
			{
				current_cloud = (Cloud*)App->phy->AddObject(player->collider->x - 64 - min_radius, player->collider->y, 64, 32, CLOUD);
			}
		}
		else
		{
			if (joystickx > 0)
			{
				float coord = min_radius + (max_radius - min_radius)*abs(joystickx);
				current_cloud = (Cloud*)App->phy->AddObject(player->collider->x + player->collider->w + coord, player->collider->y, 64, 32, CLOUD);
			}

			if (joystickx < 0)
			{
				float coord = min_radius + (max_radius - min_radius)*abs(joystickx);
				current_cloud = (Cloud*)App->phy->AddObject(player->collider->x - 64 - coord, player->collider->y, 64, 32, CLOUD);
			}
		}

		player->LockMovement(cast_time_cloud);
		cloud_timer.Reset();
		cloud_timer.Start();

		current_cloud->life_expectancy = 5000;

		is_cloud_on_cooldown = true;
	}

	if(current_cloud != nullptr && cloud_timer.Read()> current_cloud->life_expectancy)
	{
		App->phy->DeleteObject(current_cloud);
		current_cloud = nullptr;
	}

	if (current_cloud != nullptr)
	{
		current_cloud->Loop(dt);
	}

	if (is_cloud_on_cooldown && cloud_timer.Read()>cooldown_cloud)
	{
		is_cloud_on_cooldown = false;
	}
}

void Water::UnlockMovementEvent()
{

}


void Water::CleanUp()
{

}