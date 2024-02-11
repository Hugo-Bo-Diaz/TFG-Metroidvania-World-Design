#ifndef CAMERA__H
#define CAMERA__H

#include "PartsDef.h"
#include<vector>

#include "SDL/include/SDL.h"

class GameObject;

class Camera : public Part
{
public:
	Camera();

	int width;
	int height;

	bool Init();
	bool Loop(float dt);
	bool CleanUp();

	void FollowObject(GameObject*target);

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

	Timer screencover;
	bool is_covered = false;
	void CoverScreen(float amount_in_ms,float falloff_in_ms, int r, int g, int b);
	float total_cover_time;
	float falloff;
	int r, g, b;
	SDL_Rect screenarea;
	int GetCoveragePercent();
	int alpha=0;

	GameObject* target = nullptr;
};

#endif // !CAMERA__H

