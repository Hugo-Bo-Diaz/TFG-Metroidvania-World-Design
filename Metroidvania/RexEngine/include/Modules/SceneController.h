#ifndef SCENECONTROLLER__H
#define SCENECONTROLLER__H

#include "PartsDef.h"
#include <vector>
#include <list>
#include <functional>
//#include "SDL/include/SDL.h"
#include "Part.h"

#include "Textures.h"
#define ROOMS_MAX_X 10
#define ROOMS_MAX_Y 10



class DLL_EXPORT SceneController : public Part
{
public:
	SceneController(EngineAPI&);

	void LoadMap(const char* filename);

	void CleanMap();

	void GetRoomSize(int& x, int& y);

	bool AssignGameLoopFunction(std::function<void()> SceneFunction);
	bool AssignLoadFunction(std::function<void()> SceneFunction);

private:
	class SceneControllerImpl;
};

#endif // !SCENECONTROLLER__H

