#include "Application.h"
#include "Camera.h"
#include "Window.h"
#include "Physics.h"
#include <stdlib.h>
#include <time.h>
#include "Console.h"
#include "Input.h"

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
	return true;
}

bool Camera::Loop(float dt)//camera can't drop further than 704-height
{
	if (target != nullptr)
	{
		position_x = (target->collider->x + target->collider->w / 2) - width / 2;
		position_y = (target->collider->y + target->collider->h / 2) - height / 2;
		if (position_y > 576 - height)
		{
			position_y = 576 - height;
		}
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

	if (App->inp->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		CameraShake(10, 300);
	}
	
	if(is_shaking)
	{
		if (shaking.Read() > total_shaking_time)
		{
			is_shaking = false;
		}
	}

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
