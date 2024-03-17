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

	void SetCameraPosition(float x, float y);
	void GetCameraPosition(float& x, float& y);
	void GetCameraSize(float& x, float& y);

	void CameraShake(int amount, float time);

	void CoverScreen(float amount_in_ms,float falloff_in_ms, int r, int g, int b);
	RXRect GetScreenArea();
	int GetCoveragePercent();
private:

	class CameraImpl;
};

#endif // !CAMERA__H

