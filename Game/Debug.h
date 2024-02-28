#ifndef DEBUG__H
#define DEBUG__H

#include "PartsDef.h"
#include <queue>
#include "Text.h"
#include "SDL/include/SDL.h"
#include <Windows.h>

#define MAX_DEBUG_QUEUE_DATA_SAMPLE 60

class Debug : public Part
{
private:
	bool mIsDebugPanelActive = false;
	bool mIsDebugSceneActive = false;

	SDL_Rect mPanel;
	
	SDL_Keycode mKeyTogglePanel = SDL_SCANCODE_F1;
	SDL_Keycode mKeyToggleScene = SDL_SCANCODE_F2;
	
	FontID mDebugPanelFont;

	std::queue<float> mUpdateTimesQueue;
	std::queue<float> mCPUUsageQueue;

	ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
	int numProcessors;
	HANDLE self;

public:
	Debug();

	bool Init();
	bool Start();
	bool Loop(float dt);

	bool LoadConfig(pugi::xml_node&);
	bool CreateConfig(pugi::xml_node&);

	bool IsDebugActive() { return mIsDebugSceneActive; };

	bool GetTotalMemoryUsage(float& virtualmem,float& physicalmem);
	float GetPercentCPUUsage();

	float GetQueueMedianNumber(std::queue<float> lQueue);
};

#endif // !DEBUG__H