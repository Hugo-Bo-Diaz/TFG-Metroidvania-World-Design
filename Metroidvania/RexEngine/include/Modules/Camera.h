#ifndef CAMERA__H
#define CAMERA__H

#include "PartsDef.h"
#include "Part.h"
#include<vector>
#include "Utils/Timer.h"
#include "RXRect.h"

class GameObject;

//module that manages the Camera inside of the scene
class DLL_EXPORT Camera : public Part
{
public:
	Camera(EngineAPI& aAPI);

	//sets an object for the camera to follow
	void FollowObject(GameObject*target);

	//gets the x offset for rendering to take place
	float GetCameraXoffset();
	//gets the y offset for rendering to take place
	float GetCameraYoffset();

	//returns true if the rectangle is on screen, if MapCoords is active it will use absoulte coordinates
	bool isOnScreen(RXRect& rectangle,bool aMapCoords = true);

	//sets the position
	void SetCameraPosition(float x, float y);
	//returns the position
	void GetCameraPosition(float& x, float& y);
	//returns size of the camera
	void GetCameraSize(float& x, float& y);

	//Shakes the camera by a max amount for a set amount of time
	void CameraShake(int amount, float time);

	//covers the screen to create a fade effect
	void CoverScreen(float amount_in_ms,float falloff_in_ms, int r, int g, int b);
	//returns how much of the screen has been covered in %
	int GetCoveragePercent();

	//returns the area the camera covers of the map
	RXRect GetScreenArea();
private:

	class CameraImpl;
};

#endif // !CAMERA__H

