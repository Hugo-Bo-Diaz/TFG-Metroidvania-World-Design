#include "ClingingCreature.h"
#include "Audio.h"
#include "Particles.h"

ClingCreature::ClingCreature()
{
	animation.AddFrame({ 0,0,48,48});
	animation.AddFrame({ 48,0,48,48 });
	animation.AddFrame({ 96,0,48,48 });
	animation.AddFrame({ 144,0,48,48 });
	animation.AddFrame({ 192,0,48,48 });
}

ClingCreature::~ClingCreature()
{
	App->par->AddParticleEmitter(&App->par->stone_death, collider->x, collider->y, 200);
}

bool ClingCreature::Loop(float dt)
{

	std::vector<SDL_Rect*> colliders;
	App->phy->GetNearbyWalls(collider->x, collider->y, 50, colliders);

	bool thereisfloor = false;




	switch (curr_dir)
	{
	case CLINGCREATURE_RIGHT:
		
		collider->x += speed_param;

		//check_down.x = collider->x + collider->w / 2;
		check_down.x = collider->x;
		//check_down.w = 5;
		check_down.w = collider->w+5;
		check_down.y = collider->y + collider->h;
		check_down.h = collider->h/2;

		check_front.x = collider->x + collider->w;
		check_front.w = 5;
		check_front.y = collider->y;
		check_front.h = collider->h/2;

		break;
	case CLINGCREATURE_DOWN:
		collider->y += speed_param;

		check_down.x = collider->x - collider->w/2;
		check_down.w = collider->w/2;
		check_down.y = collider->y;
		//check_down.h = 5;
		check_down.h = collider->h+5;


		check_front.x = collider->x + collider->w/2;
		check_front.w = collider->w/2;
		check_front.y = collider->y+collider->h;
		check_front.h = 5;

		break;
	case CLINGCREATURE_LEFT:
		collider->x -= speed_param;

		//check_down.x = collider->x+collider->w - 5;
		check_down.x = collider->x - 5;
		//check_down.w = 5;
		check_down.w = collider->w + 5;
		check_down.y = collider->y - collider->h/2;
		check_down.h = collider->h/2;

		check_front.x = collider->x-5;
		check_front.w = 5;
		check_front.y = collider->y + collider->h / 2;
		check_front.h = collider->h / 2;

		break;
	case CLINGCREATURE_UP:
		collider->y -= speed_param;

		check_down.x = collider->x + collider->w;
		check_down.w = collider->w/2;
		//check_down.y = collider->y + collider->h - 5;
		check_down.y = collider->y - 5;
		//check_down.h = 5;
		check_down.h = collider->h + 5;

		check_front.x = collider->x;
		check_front.w = collider->w / 2;
		check_front.y = collider->y-5;
		check_front.h = 5;

		break;
	default:
		break;
	}



	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], &check_down, &result) == SDL_TRUE)// he goin crash!
		{
			thereisfloor = true;
		}
	}


	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], &check_front, &result) == SDL_TRUE)// he goin crash!
		{
			//turn up
			if (thereisfloor)
			{
				TurnCorner(false);
				hasturned = true;
			}
		}
	}

	if (shouldfall)
	{
		speed_y += accy;
		collider->y += speed_y;
		for (int i = 0; i < colliders.size(); ++i)
		{
			SDL_Rect result;
			if (SDL_IntersectRect(colliders[i], collider, &result) == SDL_TRUE)// he goin crash!
			{
				//turn up
				shouldfall = false;
				speed_y = 0;
				collider->y -= result.h;
			}
		}
	}


	if (!thereisfloor)
	{
		//turn down
		TurnCorner(true);
		hasturned = true;
	}

	if (hasturned)
	{
		timesturned += 1;
		hasturned = false;
		if (timesturned > 5)
		{
			shouldfall = true;
		}
	}
	else
	{
		timesturned = 0;
		
	}

	return true;
}
bool ClingCreature::Render()
{
	if (App->debug)
	{
		App->ren->DrawRect(&check_down, 255, 0, 0, 100, true);
		App->ren->DrawRect(&check_front, 0, 255, 0, 100, true);
	}

	if (animation_timer.Read() > animation_pace)
	{
		animation.NextFrame();
		animation_timer.Reset();
	}

	App->ren->Blit(App->tex->Get_Texture("cling_enemy"), collider->x, collider->y, animation.GetCurrentFrame(), -2,angle);


	return true;
}

void ClingCreature::TurnCorner(bool clockwise)
{
	int prevdir = curr_dir;

	int newdir = prevdir;
	if (clockwise)
	{
		newdir += 1;

		if (prevdir == CLINGCREATURE_UP)
			newdir = CLINGCREATURE_RIGHT;
	}
	else
	{
		newdir -= 1;

		if (prevdir == CLINGCREATURE_RIGHT)
			newdir = CLINGCREATURE_UP;
	}

	angle = newdir * 90;

	curr_dir = (ClingCreatureDirection)newdir;
	//printf("%d", curr_dir);
	
}

void ClingCreature::RecieveDamage(int dmg, int direction)
{
	App->aud->PlaySFX(SFX_ENEMY_HIT);

	health -= dmg;
	if (health <= 0)
	{
		App->phy->DeleteObject(this);
	}

}