#include "Application.h"
#include "Modules/Camera.h"
#include "Modules/Window.h"
#include "Modules/ObjectManager.h"
#include <stdlib.h>
#include <time.h>
#include "Modules/Input.h"
#include "Modules/SceneController.h"
#include "Modules/Render.h"
#include "Utils/Logger.h"

#include <string>
#include <sstream>

Camera::Camera(EngineAPI& aAPI) : Part("Camera", aAPI)
{
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
	screenarea.w = mApp.GetModule<Window>().width;
	screenarea.h = mApp.GetModule<Window>().height;
	
	if (target != nullptr)
	{
		position_x = (target->collider->x + target->collider->w / 2) - width / 2;
		position_y = (target->collider->y + target->collider->h / 2) - height / 2;

		if (position_y > mApp.GetModule<SceneController>().room_h * 48 - height)
		{
			position_y = mApp.GetModule<SceneController>().room_h * 48 - height;
		}
		else if (position_y < 0)
		{
			position_y = 0;
		}
		
		if (position_x > mApp.GetModule<SceneController>().room_w * 48 - width)
		{
			position_x = mApp.GetModule<SceneController>().room_w * 48 - width;
		}
		else if (position_x < 0)
		{
			position_x = 0;
		}
		position_x *= mApp.GetModule<Window>().GetScale();
		position_y *= mApp.GetModule<Window>().GetScale();
		if (is_shaking)
		{
			position_x += rand() % amount + 1;
			position_y += rand() % amount + 1;

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
			alpha = (screencover.Read() / falloff) * 255;
		}
		else if (screencover.Read() > total_cover_time - falloff)
		{
			alpha = ((total_cover_time-screencover.Read()) / falloff) * 255;
		}
		else
		{
			alpha = 255;
		}

		mApp.GetModule<Render>().DrawRect(screenarea,r,g,b,alpha,true,RenderQueue::RENDER_DEBUG,-1000);
	}
	else { alpha = 0; }

	return true;
}

bool Camera::CleanUp()
{
	return true;
}

void Camera::FollowObject(GameObject* new_target)
{
	target = new_target;
	Logger::Console_log(LogLevel::LOG_INFO,"New camera follow target!");
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
	std::stringstream lStr;
	lStr << "Starting coverscreen for: " << amount_in_ms << "ms, with colors: r" << _r << " g" << _g << " b" << _b;
	Logger::Console_log(LogLevel::LOG_INFO, lStr.str().c_str());

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
