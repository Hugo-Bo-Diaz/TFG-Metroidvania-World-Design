#include "RXpch.h"
#include "Modules/Window.h"
#include "Modules/Input.h"
#include "Modules/SceneController.h"
#include "Modules/Render.h"
#include "Utils/Logger.h"

#include "CameraImpl.h"


Camera::Camera(EngineAPI& aAPI) : Part("Camera", aAPI)
{
	mPartFuncts = new CameraImpl(this);
}

#pragma region IMPLEMENTATION

bool Camera::CameraImpl::Init()
{
	width = 1024;//App->win->width;
	height = 576;//App->win->height;

	screenarea = { 0,0,width,height };

	return true;
}

bool Camera::CameraImpl::Loop(float dt)//camera can't go offbounds
{
	screenarea.x = position_x;
	screenarea.y = position_y;

	int window_w, window_h;
	mPartInst->mApp.GetModule<Window>().GetWindowSize(window_w, window_h);
	screenarea.w = window_w;
	screenarea.h = window_h;
	
	if (target != nullptr)
	{
		position_x = (target->collider->x + target->collider->w / 2) - width / 2;
		position_y = (target->collider->y + target->collider->h / 2) - height / 2;

		int room_w, room_h;
		mPartInst->mApp.GetModule<SceneController>().GetRoomSize(room_w, room_h);
		if (position_y > room_h * 48 - height)
		{
			position_y = room_h * 48 - height;
		}
		else if (position_y < 0)
		{
			position_y = 0;
		}
		
		if (position_x > room_w * 48 - width)
		{
			position_x = room_w * 48 - width;
		}
		else if (position_x < 0)
		{
			position_x = 0;
		}
		position_x *= mPartInst->mApp.GetModule<Window>().GetScale();
		position_y *= mPartInst->mApp.GetModule<Window>().GetScale();
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
		if (screencover.Read() > total_cover_time)
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
			alpha = ((total_cover_time - screencover.Read()) / falloff) * 255;
		}
		else
		{
			alpha = 255;
		}

		//mPartInst->mApp.GetModule<Render>().DrawRect(mPartInst->screenarea, mPartInst->r, mPartInst->g, mPartInst->b, mPartInst->alpha,true,RenderQueue::RENDER_DEBUG,-1000);
	}
	else { alpha = 0; }

	return true;
}

#pragma endregion

#pragma region PUBLIC API

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
	CameraImpl* lImpl = dynamic_cast<CameraImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
	}

	return -lImpl->position_x;
}

float Camera::GetCameraYoffset()
{
	CameraImpl* lImpl = dynamic_cast<CameraImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
	}

	return -lImpl->position_y;
}

bool Camera::isOnScreen(RXRect& rectangle,bool aMapCoords)
{
	CameraImpl* lImpl = dynamic_cast<CameraImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
	}

	if(aMapCoords)
		return RXRectDoesCollide(&lImpl->screenarea, &rectangle);
	else
	{
		RXRect lScreenAtZero = { 0, 0, lImpl->screenarea.w, lImpl->screenarea.h };
		return RXRectDoesCollide(&lScreenAtZero, &rectangle);
	}
}

void Camera::SetCameraPosition(float x, float y)
{
	CameraImpl* lImpl = dynamic_cast<CameraImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
	}

	lImpl->position_x = x;
	lImpl->position_y = y;
}

void Camera::GetCameraPosition(float& x, float& y)
{
	CameraImpl* lImpl = dynamic_cast<CameraImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
	}

	x = lImpl->position_x;
	y = lImpl->position_y;
}

void Camera::GetCameraSize(float& w, float& h)
{
	CameraImpl* lImpl = dynamic_cast<CameraImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
	}

	w = lImpl->width;
	h = lImpl->height;
}

void Camera::CameraShake(int _amount, float _time)
{
	CameraImpl* lImpl = dynamic_cast<CameraImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
	}

	lImpl->is_shaking = true;
	lImpl->shaking.Start();
	lImpl->total_shaking_time = _time;
	lImpl->amount = _amount;
	srand(time(NULL));
}

void Camera::CoverScreen(float amount_in_ms, float falloff_in_ms, int _r, int _g, int _b)
{
	std::stringstream lStr;
	lStr << "Starting coverscreen for: " << amount_in_ms << "ms, with colors: r" << _r << " g" << _g << " b" << _b;
	Logger::Console_log(LogLevel::LOG_INFO, lStr.str().c_str());

	CameraImpl* lImpl = dynamic_cast<CameraImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
	}

	lImpl->total_cover_time = amount_in_ms;
	lImpl->falloff = falloff_in_ms;
	lImpl->is_covered = true;

	lImpl->screencover.Start();
}

int Camera::GetCoveragePercent()
{
	CameraImpl* lImpl = dynamic_cast<CameraImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
	}

	if (lImpl->is_covered)
	{
		return (lImpl->alpha / 255) * 100;
	}
	else
	{
		return 0;
	}
}

RXRect Camera::GetScreenArea()
{
	CameraImpl* lImpl = dynamic_cast<CameraImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
	}

	return lImpl->screenarea;
}

#pragma endregion