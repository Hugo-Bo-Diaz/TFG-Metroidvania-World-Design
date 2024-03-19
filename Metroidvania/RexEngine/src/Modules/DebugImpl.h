#ifndef DEBUG_IMPL__H
#define DEBUG_IMPL__H

#include "PartImpl.h"
#include "../include/Modules/Debug.h"
#include "SDL/include/SDL.h"


class Debug::DebugImpl : public Part::Part_Impl
{
public:
	Debug::DebugImpl(Debug* aDebug):mPartInst(aDebug) {};

	void SetDebugInst(Debug* aCamera)
	{
		mPartInst = aCamera;
	}

protected:
	bool Init();
	bool Loop(float dt);

	bool LoadConfig(pugi::xml_node& config_node);
	bool CreateConfig(pugi::xml_node& config_node);

private:
	Debug* mPartInst;
	friend class Debug;

	bool mIsDebugPanelActive = false;
	bool mIsDebugSceneActive = false;

	SDL_Keycode mKeyTogglePanel = SDL_SCANCODE_F1;
	SDL_Keycode mKeyToggleScene = SDL_SCANCODE_F2;

	RXRect mPanel;

	std::queue<float> mUpdateTimesQueue;
	std::queue<float> mCPUUsageQueue;

	ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
	int numProcessors;
	HANDLE self;
};




#endif