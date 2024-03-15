#ifndef CAMERA_IMPL__H
#define CAMERA_IMPL__H

#include "PartImpl.h"
#include "../include/Modules/Camera.h"
#include "EngineElements/GameObject.h"

class Camera::CameraImpl : public Part::Part_Impl
{
public:
	Camera::CameraImpl(Camera* aCamera):mPartInst(aCamera){};

	void SetCameraInst(Camera* aCamera)
	{
		mPartInst = aCamera;
	}

	GameObject* target = nullptr;

protected:
	bool Init();
	bool Loop(float dt);
	bool CleanUp();
private:
	Camera* mPartInst;
};

#endif
