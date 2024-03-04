#include "Cloud.h"
#include "Application.h"
#include "Modules/Input.h"
#include "Modules/Render.h"
#include "../SpellProjectiles/Rain.h"

Cloud::Cloud()
{
	SDL_Rect wall = { collider->x,collider->y,collider->w,collider->h };

	blitrect = {96,96,64,32};
}

void Cloud::Init()
{
	spells = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/spells.png");
}

bool Cloud::Loop(float dt)
{
	bool ret = true;

	Engine->GetModule<::Render>().Blit(spells, collider->x, collider->y, &blitrect, -2);

	if (timer.Read()>life_expectancy)
	{
		Engine->GetModule<ObjectManager>().DeleteObject(this);
	}

	//handle rain
	
	//first spawn if the timer is up
	if (rain_timer.Read() > rain_frequency)
	{
		Rain* r = new Rain(Engine);
		r->blitrect = { 0, 192, 64, 32};
		r->speed = rain_speed;
		r->hitbox = { collider->x,collider->y,64,32};
		r->parent = this;
		rain_timer.Reset();
		rain_vector.push_back(r);
	}

	//now update all the current drops

	for (int i = 0; i < rain_vector.size(); ++i)
	{
		rain_vector[i]->Loop();
	}

	//delete all the ones that should

	for (int i = 0; i < rain_to_delete.size(); ++i)
	{
		delete (rain_to_delete[i]);
		rain_vector.erase(std::find(rain_vector.begin(),rain_vector.end(),rain_to_delete[i]));
	}
	rain_to_delete.clear();

	return ret;
}

bool Cloud::Render()
{
	for (int i = 0; i < rain_vector.size(); ++i)
	{
		rain_vector[i]->Render();
	}
	return true;
}

void Cloud::DeleteRainDrop(Rain * drop)
{
	rain_to_delete.push_back(drop);
}
