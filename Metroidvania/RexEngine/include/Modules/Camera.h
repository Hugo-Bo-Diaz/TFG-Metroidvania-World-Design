#ifndef CAMERA__H
#define CAMERA__H

#include "PartsDef.h"
#include<vector>

#include "SDL/include/SDL.h"
#include "Part.h"
class GameObject;

class DLL_EXPORT Camera : public Part
{
private:

	bool Init();
	bool Loop(float dt);
	bool CleanUp();
	GameObject* target = nullptr;

public:
	Camera(EngineAPI& aAPI);

	void FollowObject(GameObject*target);

	float GetCameraXoffset();
	float GetCameraYoffset();

	bool isOnScreen(SDL_Rect rectangle);

	float position_x;
	float position_y;

	int width;
	int height;

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
};

#endif // !CAMERA__H

