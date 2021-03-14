#include "Cloud.h"
#include "Application.h"
#include "Input.h"
#include "Console.h"
#include "Render.h"
#include "Rain.h"

Cloud::Cloud()
{
	SDL_Rect wall = { collider->x,collider->y,collider->w,collider->h };

	blitrect = {96,64,64,32};
}

bool Cloud::Loop(float dt)
{
	bool ret = true;

	App->ren->Blit(App->tex->Get_Texture("spells"), collider->x, collider->y, &blitrect, -2);

	if (timer.Read()>life_expectancy)
	{
		App->phy->DeleteObject(this);
	}

	//handle rain
	
	//first spawn if the timer is up
	if (rain_timer.Read() > rain_frequency)
	{
		Rain* r = new Rain();
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

void Cloud::DeleteRainDrop(Rain * drop)
{
	rain_to_delete.push_back(drop);
}

Cloud::~Cloud()
{
}