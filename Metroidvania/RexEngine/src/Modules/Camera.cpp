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
#include "CameraImpl.h"


Camera::Camera(EngineAPI& aAPI) : Part("Camera", aAPI)
{
	mPartFuncts = new CameraImpl(this);

	shaking.Pause();
	shaking.Reset();
}

bool Camera::CameraImpl::Init()
{
	mPartInst->width = 1024;//App->win->width;
	mPartInst->height = 576;//App->win->height;

	mPartInst->screenarea = { 0,0,mPartInst->width,mPartInst->height };

	return true;
}

bool Camera::CameraImpl::Loop(float dt)//camera can't go offbounds
{
	int window_w, window_h;
	mPartInst->mApp.GetModule<Window>().GetWindowSize(window_w, window_h);
	mPartInst->screenarea.w = window_w;
	mPartInst->screenarea.h = window_h;
	
	if (target != nullptr)
	{
		mPartInst->position_x = (target->collider->x + target->collider->w / 2) - mPartInst->width / 2;
		mPartInst->position_y = (target->collider->y + target->collider->h / 2) - mPartInst->height / 2;

		int room_w, room_h;
		mPartInst->mApp.GetModule<SceneController>().GetRoomSize(room_w, room_h);
		if (mPartInst->position_y > room_h * 48 - mPartInst->height)
		{
			mPartInst->position_y = room_h * 48 - mPartInst->height;
		}
		else if (mPartInst->position_y < 0)
		{
			mPartInst->position_y = 0;
		}
		
		if (mPartInst->position_x > room_w * 48 - mPartInst->width)
		{
			mPartInst->position_x = room_w * 48 - mPartInst->width;
		}
		else if (mPartInst->position_x < 0)
		{
			mPartInst->position_x = 0;
		}
		mPartInst->position_x *= mPartInst->mApp.GetModule<Window>().GetScale();
		mPartInst->position_y *= mPartInst->mApp.GetModule<Window>().GetScale();
		if (mPartInst->is_shaking)
		{
			mPartInst->position_x += rand() % mPartInst->amount + 1;
			mPartInst->position_y += rand() % mPartInst->amount + 1;

			std::string s = std::to_string(mPartInst->position_x);
			s += " ";
			s += std::to_string(mPartInst->position_y);
			//App->con->Console_log((char*)s.c_str());
		}
	}
	
	if(mPartInst->is_shaking)
	{
		if (mPartInst->shaking.Read() > mPartInst->total_shaking_time)
		{
			mPartInst->is_shaking = false;
			mPartInst->alpha = 0;
		}
	}

	if (mPartInst->is_covered)
	{
		if (mPartInst->screencover.Read() > mPartInst->total_cover_time)
		{
			mPartInst->is_covered = false;
		}
		mPartInst->alpha = 255;

		if (mPartInst->screencover.Read() <= mPartInst->falloff)
		{
			mPartInst->alpha = (mPartInst->screencover.Read() / mPartInst->falloff) * 255;
		}
		else if (mPartInst->screencover.Read() > mPartInst->total_cover_time - mPartInst->falloff)
		{
			mPartInst->alpha = ((mPartInst->total_cover_time - mPartInst->screencover.Read()) / mPartInst->falloff) * 255;
		}
		else
		{
			mPartInst->alpha = 255;
		}

		mPartInst->mApp.GetModule<Render>().DrawRect(mPartInst->screenarea, mPartInst->r, mPartInst->g, mPartInst->b, mPartInst->alpha,true,RenderQueue::RENDER_DEBUG,-1000);
	}
	else { mPartInst->alpha = 0; }

	return true;
}

bool Camera::CameraImpl::CleanUp()
{
	return true;
}

void Camera::FollowObject(GameObject* new_target)
{
	CameraImpl* lImpl = dynamic_cast<CameraImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
	}
	lImpl->target = new_target;
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

bool Camera::isOnScreen(RXRect& rectangle)
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
