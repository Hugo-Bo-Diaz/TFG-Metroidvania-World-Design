#ifndef CAMERA__H
#define CAMERA__H

#include "PartsDef.h"
#include<vector>

#include "SDL/include/SDL.h"

class physobj;

class Camera : public Part
{
public:
	Camera();

	int width;
	int height;

	bool Init();
	bool Loop(float dt);
	bool CleanUp();

	void FollowObject(physobj*target);

	float GetCameraXoffset();
	float GetCameraYoffset();

	bool isOnScreen(SDL_Rect rectangle);

	float position_x;
	float position_y;

	void CameraShake(int amount, float time);
	bool is_shaking = false;
	Timer shaking;
	float total_shaking_time;
	int amount;

	physobj* target = nullptr;
};

#endif // !CAMERA__H

