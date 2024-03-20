#ifndef SCENECONTROLLER__H
#define SCENECONTROLLER__H

#include "PartsDef.h"
#include <vector>
#include <list>
#include <functional>
#include "Part.h"

#include "Textures.h"
#define ROOMS_MAX_X 10
#define ROOMS_MAX_Y 10

//module that manages loading maps and objects
class DLL_EXPORT SceneController : public Part
{
public:
	SceneController(EngineAPI&);

	//loads a map found at that file
	void LoadMap(const char* filename);
	//removes all map elements
	void CleanMap();
	//returns the room's size
	void GetRoomSize(int& x, int& y);

	//the function given as an argument will be called every frame
	bool AssignGameLoopFunction(std::function<void()> SceneFunction);
	//the function given as an argument will be called every time a new scene is loaded
	bool AssignLoadFunction(std::function<void()> SceneFunction);

private:
	class SceneControllerImpl;
};

#endif // !SCENECONTROLLER__H

