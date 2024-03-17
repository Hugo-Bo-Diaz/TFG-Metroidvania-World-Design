#ifndef CAMERA_IMPL__H
#define CAMERA_IMPL__H

#include "PartImpl.h"
#include "../include/Modules/Camera.h"
#include "EngineElements/GameObject.h"

class Camera::CameraImpl : public Part::Part_Impl
{
public:
	Camera::CameraImpl(Camera* aCamera):mPartInst(aCamera){
		shaking.Pause();
		shaking.Reset();
	};

	void SetCameraInst(Camera* aCamera)
	{
		mPartInst = aCamera;
	}

protected:
	bool Init();
	bool Loop(float dt);

private:
	Camera* mPartInst;
	friend class Camera;

	float position_x;
	float position_y;

	int width;
	int height;

	bool is_shaking = false;
	Timer shaking;
	float total_shaking_time;
	int amount;

	RXRect screenarea;

	Timer screencover;
	bool is_covered = false;
	float total_cover_time;
	float falloff;

	int alpha = 0;

	GameObject* target = nullptr;
};

#endif
