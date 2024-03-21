#include "Water.h"
#include "../GameObjects/Player.h"
#include "Application.h"
#include "Modules/Input.h"
#include "Modules/ObjectManager.h"
#include "Modules/Render.h"
#include "../GameObjects/SpellProjectiles/IceShard.h"
#include "../GameObjects/SpellProjectiles/IceBlock.h"
#include "../GameObjects/SpellProjectiles/Cloud.h"
#include "Modules/Camera.h"
#include "Utils/Utils.h"
#include "Modules/Input.h"

void Water::Init()
{

}

void Water::Loop(float dt)
{
	//iceshard--------------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(X) == BUTTON_DOWN)
	if (Engine->GetModule<Input>().GetInput(BUTTON_2) == KEY_DOWN)
	{
		IceShard* iceshard = (IceShard*)Engine->GetModule<ObjectManager>().AddObject(player->collider.x, player->collider.y+player->collider.h/2-12, 48, 24, GetTypeIndex<IceShard>());
		iceshard->Fire(player->is_right);
		Engine->GetModule<Camera>().CameraShake(10, 100);
	}

	//Iceblock------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(Y) == BUTTON_DOWN && !is_iceblock_on_cooldown)
	if (Engine->GetModule<Input>().GetInput(BUTTON_3) == KEY_DOWN && !is_iceblock_on_cooldown)
	{
		if (player->is_right)
		{
			IceBlock* block = (IceBlock*)Engine->GetModule<ObjectManager>().AddObject(player->collider.x + player->collider.w, player->collider.y, 64, 64, GetTypeIndex<IceBlock>());
			block->Init();
		}
		else
		{
			IceBlock* block = (IceBlock*)Engine->GetModule<ObjectManager>().AddObject(player->collider.x - 64, player->collider.y, 64, 64, GetTypeIndex<IceBlock>());
			block->Init();
		}

		player->LockMovement(cast_time_block);
		iceblock_timer.Reset();
		iceblock_timer.Start();
		is_iceblock_on_cooldown = true;
	}

	if (is_iceblock_on_cooldown && iceblock_timer.Read()>cooldown_block)
	{
		is_iceblock_on_cooldown = false;
	}

	////Cloud--------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(B) == BUTTON_DOWN && !is_cloud_on_cooldown)
	if (Engine->GetModule<Input>().GetInput(BUTTON_4) == KEY_DOWN && !is_cloud_on_cooldown)
	{

		float joystickx, joysticky;
		Engine->GetModule<Input>().GetJoystick(true, joystickx, joysticky);

		Engine->GetModule<Camera>().CameraShake(10, 200);

		if (joystickx < 0.15 && joystickx>-0.15)
		{
			if (player->is_right)
			{
				current_cloud = (Cloud*)Engine->GetModule<ObjectManager>().AddObject(player->collider.x + player->collider.w + min_radius, player->collider.y, 64, 32, GetTypeIndex<Cloud>());
			}
			else
			{
				current_cloud = (Cloud*)Engine->GetModule<ObjectManager>().AddObject(player->collider.x - 64 - min_radius, player->collider.y, 64, 32, GetTypeIndex<Cloud>());
			}
		}
		else
		{
			if (joystickx > 0)
			{
				float coord = min_radius + (max_radius - min_radius)*abs(joystickx);
				current_cloud = (Cloud*)Engine->GetModule<ObjectManager>().AddObject(player->collider.x + player->collider.w + coord, player->collider.y, 64, 32, GetTypeIndex<Cloud>());
			}

			if (joystickx < 0)
			{
				float coord = min_radius + (max_radius - min_radius)*abs(joystickx);
				current_cloud = (Cloud*)Engine->GetModule<ObjectManager>().AddObject(player->collider.x - 64 - coord, player->collider.y, 64, 32, GetTypeIndex<Cloud>());
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
		Engine->GetModule<ObjectManager>().DeleteObject(current_cloud);
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