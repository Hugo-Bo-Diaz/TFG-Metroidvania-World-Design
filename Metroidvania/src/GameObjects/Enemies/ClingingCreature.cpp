#include "ClingingCreature.h"
#include "Modules/Audio.h"
#include "Modules/Particles.h"
#include "Modules/Debug.h"

ClingCreature::ClingCreature()
{
}

ClingCreature::ClingCreature(std::list<ObjectProperty*>& aProperties)
{
	new (this) ClingCreature;

	for (std::list<ObjectProperty*>::iterator it = aProperties.begin(); it != aProperties.end(); ++it)
	{
		if ((*it)->name.compare("direction") == 0)
		{
			curr_dir = (ClingCreatureDirection)(int)(*it)->num_value;
		}
	}

}

void ClingCreature::Init()
{
	cling_enemy = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/enemies/cling_enemy.png");
	particles = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/particles.png");

	mSFXHit = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/enemy_hit.wav");

	r7buff = { 24,12,12,12 };
	r16sandfirst = { 48,0,12,12 };
	r17sandsecond = { 48,12,12,12 };
	r18metalfirst = { 48,24,12,12 };

	stone_death.area_in_texture.push_back(&r16sandfirst);
	stone_death.area_in_texture.push_back(&r17sandsecond);
	stone_death.name = "stone_monster_death";
	stone_death.minmax_x_offset = std::make_pair(0, 48);
	stone_death.minmax_y_offset = std::make_pair(0, 48);
	stone_death.minmax_speed_x = std::make_pair(-0.6, 0.6);
	stone_death.minmax_speed_y = std::make_pair(-2, -3);
	stone_death.minmax_scale = std::make_pair(1, 1.5);
	stone_death.minmax_angle = std::make_pair(0, 360);
	stone_death.minmax_lifespan = std::make_pair(200, 500);
	stone_death.minmax_frequency = std::make_pair(7, 15);
	stone_death.minmax_acc_y = std::make_pair(0.1, 0.3);
	stone_death.texture_name = particles;

	metal_and_sand.area_in_texture.push_back(&r16sandfirst);
	//metal_and_sand.area_in_texture.push_back(&r17sandsecond);
	metal_and_sand.area_in_texture.push_back(&r18metalfirst);
	metal_and_sand.name = "metal and sand";
	metal_and_sand.minmax_speed_y = std::make_pair(-6, -4);
	metal_and_sand.minmax_speed_x = std::make_pair(-3, 3);
	metal_and_sand.minmax_lifespan = std::make_pair(75, 150);
	metal_and_sand.minmax_angle_speed = std::make_pair(5, 15);
	metal_and_sand.minmax_angle = std::make_pair(0, 360);
	metal_and_sand.minmax_x_offset = std::make_pair(-15, 15);
	metal_and_sand.minmax_y_offset = std::make_pair(-15, 15);
	metal_and_sand.minmax_acc_y = std::make_pair(1, 2);
	metal_and_sand.minmax_scale = std::make_pair(1.1, 1.3);
	metal_and_sand.minmax_frequency = std::make_pair(5, 12);
	metal_and_sand.texture_name = particles;

	animation.AddFrame({ 0,0,48,48});
	animation.AddFrame({ 48,0,48,48 });
	animation.AddFrame({ 96,0,48,48 });
	animation.AddFrame({ 144,0,48,48 });
	animation.AddFrame({ 192,0,48,48 });
}

bool ClingCreature::Loop(float dt)
{

	std::vector<RXRect*> colliders;
	Engine->GetModule<ObjectManager>().GetNearbyWalls(collider->x, collider->y, 50, colliders);

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
		RXRect result;
		if (RXRectCollision(colliders[i], &check_down, &result) == true)// he goin crash!
		{
			thereisfloor = true;
		}
	}


	for (int i = 0; i < colliders.size(); ++i)
	{
		RXRect result;
		if (RXRectCollision(colliders[i], &check_front, &result) == true)// he goin crash!
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
			RXRect result;
			if (RXRectCollision(colliders[i], collider, &result) == true)// he goin crash!
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
	if (animation_timer.Read() > animation_pace)
	{
		animation.NextFrame();
		animation_timer.Reset();
	}

	Engine->GetModule<::Render>().Blit(cling_enemy, collider->x, collider->y, *animation.GetCurrentFrame(), -2, RenderQueue::RENDER_GAME, angle);


	return true;
}

void ClingCreature::RenderDebug()
{
	Engine->GetModule<::Render>().DrawRect(check_down, 255, 0, 0, 100, true, RenderQueue::RENDER_DEBUG, 0);
	Engine->GetModule<::Render>().DrawRect(check_front, 0, 255, 0, 100, true, RenderQueue::RENDER_DEBUG, 0);
}

void ClingCreature::Destroy()
{
	Engine->GetModule<Particles>().AddParticleEmitter(&stone_death, collider->x, collider->y, 200);
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
}

void ClingCreature::RecieveDamage(int dmg, int direction)
{
	Engine->GetModule<Audio>().PlaySFX(mSFXHit);

	health -= dmg;
	if (health <= 0)
	{
		Engine->GetModule<ObjectManager>().DeleteObject(this);
	}
	else
	{
		Engine->GetModule<Particles>().AddParticleEmitter(&metal_and_sand, collider->x, collider->y, 200);
	}

}