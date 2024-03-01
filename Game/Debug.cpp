#include "Application.h"
#include "Render.h"
#include "Input.h"
#include "Debug.h"
#include "Physics.h"
#include "Gui.h"
#include <Psapi.h>

Debug::Debug()
{
	name = "Debug";
}

bool Debug::Init()
{
	//https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
	SYSTEM_INFO sysInfo;
	FILETIME ftime, fsys, fuser;

	GetSystemInfo(&sysInfo);
	numProcessors = sysInfo.dwNumberOfProcessors;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&lastCPU, &ftime, sizeof(FILETIME));

	self = GetCurrentProcess();
	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
	memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));

	mPanel = { 0,0,200,200 };

	return true;
}

bool Debug::Loop(float dt)
{
	if (App->inp->GetKey(mKeyTogglePanel) == Keystate::KEY_DOWN)
	{
		mIsDebugPanelActive = !mIsDebugPanelActive;
	}

	if (App->inp->GetKey(mKeyToggleScene) == Keystate::KEY_DOWN)
	{
		mIsDebugSceneActive = !mIsDebugSceneActive;
	}

	if (mIsDebugPanelActive)
	{
		App->ren->DrawRect(mPanel,0,0,0,220,true,RenderQueue::RENDER_DEBUG,1,true);
		std::string lString;

		lString = "FPS: ";
		mUpdateTimesQueue.push(App->GetLastUpdateTime());
		if (mUpdateTimesQueue.size() > MAX_DEBUG_QUEUE_DATA_SAMPLE)
		{
			mUpdateTimesQueue.pop();
		}
		
		float averageDT = GetQueueMedianNumber(mUpdateTimesQueue);
		float FPS = (1.0f / averageDT) * 1000;

		lString += std::to_string(averageDT);
		App->ren->BlitText(lString.c_str(), mDebugPanelFont, 10, 20, 0, SDL_Color{255,255,255,255},RenderQueue::RENDER_DEBUG,true);
		
		float virmem,physmem;
		GetTotalMemoryUsage(virmem,physmem);

		lString = "Virt Mem: ";
		lString += std::to_string(virmem);
		App->ren->BlitText(lString.c_str(), mDebugPanelFont, 10, 45, 0, SDL_Color{ 255,255,255,255 }, RenderQueue::RENDER_DEBUG, true);

		lString = "Phys Mem: ";
		lString += std::to_string(physmem);
		App->ren->BlitText(lString.c_str(), mDebugPanelFont, 10, 70, 0, SDL_Color{ 255,255,255,255 }, RenderQueue::RENDER_DEBUG, true);

		lString = "CPU Usage: ";

		mCPUUsageQueue.push(GetPercentCPUUsage());
		if (mCPUUsageQueue.size() > MAX_DEBUG_QUEUE_DATA_SAMPLE)
		{
			mCPUUsageQueue.pop();
		}

		float averageCPU = GetQueueMedianNumber(mCPUUsageQueue);

		lString += std::to_string(averageCPU);
		App->ren->BlitText(lString.c_str(), mDebugPanelFont, 10, 95, 0, SDL_Color{ 255,255,255,255 }, RenderQueue::RENDER_DEBUG, true);

		lString = "Draw Calls: ";
		lString += std::to_string(App->ren->GetDrawCallsLastFrame());
		App->ren->BlitText(lString.c_str(), mDebugPanelFont, 10, 120, 0, SDL_Color{ 255,255,255,255 }, RenderQueue::RENDER_DEBUG, true);

		lString = "Total Objects: ";
		lString += std::to_string(App->phy->GetTotalObjectNumber());
		App->ren->BlitText(lString.c_str(), mDebugPanelFont, 10, 145, 0, SDL_Color{ 255,255,255,255 }, RenderQueue::RENDER_DEBUG, true);
	}

	if (mIsDebugSceneActive)
	{
		App->phy->RenderDebug();
		App->gui->RenderDebug();
	}

	return true;
}

bool Debug::LoadConfig(pugi::xml_node& lNode)
{
	std::string lFont = lNode.child("debug_font").first_child().value();

	mDebugPanelFont = App->txt->LoadFont(lFont.c_str(), SDL_Color{ 255,255,255,255 }, 18);
	return true;
}

bool Debug::CreateConfig(pugi::xml_node& aNode)
{
	pugi::xml_node lNode = aNode.append_child("debug_node");
	lNode.text().set("Assets/Fonts/Lato-Regular.ttf");

	return true;
}

bool Debug::GetTotalMemoryUsage(float& virmem, float& physmem)
{
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	virmem = (pmc.PrivateUsage / 1024) / 1024;
	physmem = (pmc.WorkingSetSize / 1024) / 1024;

	return true;
}

float Debug::GetPercentCPUUsage()
{
	FILETIME ftime, fsys, fuser;
	ULARGE_INTEGER now, sys, user;
	double percent;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));

	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));
	percent = (sys.QuadPart - lastSysCPU.QuadPart) +
		(user.QuadPart - lastUserCPU.QuadPart);
	percent /= (now.QuadPart - lastCPU.QuadPart);
	percent /= numProcessors;
	lastCPU = now;
	lastUserCPU = user;
	lastSysCPU = sys;

	return percent * 100;
}

float Debug::GetQueueMedianNumber(std::queue<float> lQueue)
{
	float lSumOfallTimes = 0;
	int lTotalTimes = lQueue.size();
	while (!lQueue.empty()) 
	{
		lSumOfallTimes += lQueue.front();
		lQueue.pop();
	}

	return lSumOfallTimes / (float)lTotalTimes;
}
