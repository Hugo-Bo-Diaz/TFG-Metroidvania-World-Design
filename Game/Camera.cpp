#include "Application.h"
#include "Camera.h"
#include "Window.h"
#include "Physics.h"
#include <stdlib.h>
#include <time.h>
#include "Console.h"
#include "Input.h"
#include "SceneController.h"
#include "Render.h"

#include <string>

Camera::Camera()
{
	name = "Camera";
	shaking.Pause();
	shaking.Reset();

}

bool Camera::Init()
{
	width = 1024;//App->win->width;
	height = 576;//App->win->height;

	screenarea = { 0,0,width,height };

	return true;
}

bool Camera::Loop(float dt)//camera can't go offbounds
{
	screenarea.w = App->win->width;
	screenarea.h = App->win->height;
	
	if (target != nullptr)
	{
		position_x = (target->collider->x + target->collider->w / 2) - width / 2;
		position_y = (target->collider->y + target->collider->h / 2) - height / 2;

		if (position_y > App->scn->room_h * 48 - height)
		{
			position_y = App->scn->room_h * 48 - height;
		}
		else if (position_y < 0)
		{
			position_y = 0;
		}
		
		if (position_x > App->scn->room_w * 48 - width)
		{
			position_x = App->scn->room_w * 48 - width;
		}
		else if (position_x < 0)
		{
			position_x = 0;
		}
		position_x *= App->win->GetScale();
		position_y *= App->win->GetScale();
		if (is_shaking)
		{
			position_x += rand() % amount + 1;
			position_y += rand() % amount + 1;

			//printf("%f %f \n", position_x, position_y);

			std::string s = std::to_string(position_x);
			s += " ";
			s += std::to_string(position_y);
			//App->con->Console_log((char*)s.c_str());
		}
	}
	
	if(is_shaking)
	{
		if (shaking.Read() > total_shaking_time)
		{
			is_shaking = false;
			alpha = 0;
		}
	}

	if (is_covered)
	{
		if(screencover.Read()>total_cover_time)
		{
			is_covered = false;
		}
		alpha = 255;

		if (screencover.Read() <= falloff)
		{
			//alpha = 255;
			alpha = (screencover.Read() / falloff) * 255;
			printf(" %d in", alpha);
		}
		else if (screencover.Read() > total_cover_time - falloff)
		{
			alpha = ((total_cover_time-screencover.Read()) / falloff) * 255;
			//alpha = 255;
			printf(" %d out", alpha);
		}
		else
		{
			alpha = 255;
		}

		App->ren->DrawRect(&screenarea,r,g,b,alpha,true);
	}
	else { alpha = 0; }

	return true;
}

bool Camera::CleanUp()
{
	return true;
}

void Camera::FollowObject(physobj* new_target)
{
	target = new_target;
}

float Camera::GetCameraXoffset()
{
	return -position_x;
}

float Camera::GetCameraYoffset()
{
	return -position_y;
}

bool Camera::isOnScreen(SDL_Rect rectangle)
{
	return false;
}

void Camera::CameraShake(int _amount, float _time)
{
	is_shaking = true;
	shaking.Start();
	total_shaking_time = _time;
	amount = _amount;
	srand(time(NULL));

}

void Camera::CoverScreen(float amount_in_ms, float falloff_in_ms, int _r, int _g, int _b)
{
	total_cover_time = amount_in_ms;
	falloff = falloff_in_ms;
	r = _r;
	g = _g;
	b = _b;
	is_covered = true;

	screencover.Start();
}

int Camera::GetCoveragePercent()
{

	if (is_covered)
	{
		return (alpha / 255) * 100;
	}
	else
	{
		return 0;
	}
}
