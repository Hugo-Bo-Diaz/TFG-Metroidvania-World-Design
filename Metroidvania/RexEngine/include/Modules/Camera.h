#ifndef CAMERA__H
#define CAMERA__H

#include "PartsDef.h"
#include "Part.h"
#include<vector>
#include "Utils/Timer.h"
#include "RXRect.h"

class GameObject;

class DLL_EXPORT Camera : public Part
{

public:
	Camera(EngineAPI& aAPI);

	void FollowObject(GameObject*target);

	float GetCameraXoffset();
	float GetCameraYoffset();

	bool isOnScreen(RXRect& rectangle,bool aMapCoords = true);

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
	RXRect screenarea;
	int GetCoveragePercent();
	int alpha=0;
private:

	class CameraImpl;
};

#endif // !CAMERA__H

