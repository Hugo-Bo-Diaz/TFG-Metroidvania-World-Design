#include "Ground.h"
#include "Player.h"
#include "Application.h"
#include "Input.h"
#include "Physics.h"
#include "Render.h"

#include "Rock.h"
#include "Shockwave.h"

void Ground::Init()
{

}

void Ground::Loop(float dt)
{

	//rock throw--------------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(X) == BUTTON_DOWN && ! is_rock_on_cooldown)
	if (App->inp->GetInput(BUTTON_2) == KEY_DOWN && !is_rock_on_cooldown)
	{
			((Rock*)App->phy->AddObject(player->collider->x+player->collider->w/2, player->collider->y+player->collider->h / 2, 32, 32, ROCK))->Fire(player->is_right,45,12,1);
			is_rock_on_cooldown = true;
			rock_timer.Reset();
			rock_timer.Start();
	}
	
	if(is_rock_on_cooldown && rock_timer.Read()> cooldown_rock)
	{
		is_rock_on_cooldown = false;
	}

	
	//groundpound------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(Y) == BUTTON_DOWN && !groundpounding)
	if (App->inp->GetInput(BUTTON_3) == KEY_DOWN && !groundpounding)
	{
		current_yspeed = initial_yspeed;
		groundpounding = true;
		player->LockMovement();
	}

	if (groundpounding)
	{
		//add graphic
		App->ren->Blit(App->tex->Get_Texture("groundpound"), player->collider->x, player->collider->y + player->collider->h, &groundpound, -2);

		// move player accordingly
		if (!is_on_gp_lag)
		{
			player->nextpos->y += current_yspeed;
			current_yspeed += gravity;
		}
		//check for floor and stop once on it
		std::vector<SDL_Rect*> colliders;
		App->phy->GetNearbyWalls(player->nextpos->x + player->nextpos->w / 2, player->nextpos->y + player->nextpos->h / 2, 300, colliders);

		for (int i = 0; i < colliders.size(); ++i)
		{
			SDL_Rect hitbox = {player->nextpos->x,player->nextpos->y+player->nextpos->h,player->nextpos->w,32};
			SDL_Rect result;
			if (current_yspeed > 0 &&!is_on_gp_lag && SDL_IntersectRect(colliders[i], &hitbox, &result) == SDL_TRUE)// he goin crash!
			{
				player->nextpos->y -= result.h;
				is_on_gp_lag = true;
				//start timer
				groundpound_end_timer.Reset();
				groundpound_end_timer.Start();
				current_yspeed = 0;
			}
		}
	}

	if(is_on_gp_lag && groundpound_end_timer.Read() > groundpound_endlag)
	{
		groundpounding = false;
		is_on_gp_lag = false;
		player->UnlockMovement();
	}

	
	//earthquake--------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(B) == BUTTON_DOWN && player->grounded && !is_eq_on_cooldown)
	if (App->inp->GetInput(BUTTON_4) == KEY_DOWN && player->grounded && !is_eq_on_cooldown)
	{
		((Shockwave*)App->phy->AddObject(player->collider->x + player->collider->w / 2, player->collider->y + player->collider->h-32, 32, 32, SHOCKWAVE))->Fire(true, 8);
		((Shockwave*)App->phy->AddObject(player->collider->x + player->collider->w / 2, player->collider->y + player->collider->h-32, 32, 32, SHOCKWAVE))->Fire(false, 8);
		
		is_eq_on_cooldown = true;
		earthquake_timer.Reset();
		earthquake_timer.Start();
	}

	if (is_eq_on_cooldown && earthquake_timer.Read() > cooldown_earthquake)
	{
		is_eq_on_cooldown = false;
	}
}

void Ground::UnlockMovementEvent()
{

}


void Ground::CleanUp()
{

}