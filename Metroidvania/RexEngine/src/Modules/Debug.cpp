#include "RXpch.h"
#include "Application.h"
#include "Modules/Render.h"
#include "Modules/Input.h"
#include "Modules/Debug.h"
#include "Utils/Utils.h"
#include <Psapi.h>

#include "DebugImpl.h"
#include "ObjectManagerImpl.h"
#include "GuiImpl.h"

Debug::Debug(EngineAPI& aAPI) : Part("Debug",aAPI)
{
	mPartFuncts = new DebugImpl(this);
}

#pragma region IMPLEMENTATION

bool Debug::DebugImpl::Init()
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

bool Debug::DebugImpl::Loop(float dt)
{
	if (mPartInst->mApp.GetModule<Input>().GetKey(mKeyTogglePanel) == Keystate::KEY_DOWN)
	{
		mIsDebugPanelActive = !mIsDebugPanelActive;
	}

	if (mPartInst->mApp.GetModule<Input>().GetKey(mKeyToggleScene) == Keystate::KEY_DOWN)
	{
		mIsDebugSceneActive = !mIsDebugSceneActive;
	}

	if (mIsDebugPanelActive)
	{
		mPartInst->mApp.GetModule<Render>().RenderRect(mPanel, RXColor{ 0,0,0,220 }, true, RenderQueue::RENDER_DEBUG, 1, true);
		std::string lString;

		lString = "FPS: ";
		mUpdateTimesQueue.push(mPartInst->mApp.GetLastUpdateTime());
		if (mUpdateTimesQueue.size() > MAX_DEBUG_QUEUE_DATA_SAMPLE)
		{
			mUpdateTimesQueue.pop();
		}
		
		float averageDT = GetQueueMedianNumber(mUpdateTimesQueue);
		float FPS = (1.0f / averageDT) * 1000;

		lString += std::to_string(averageDT);
		mPartInst->mApp.GetModule<Render>().RenderText(lString.c_str(), mPartInst->mDebugPanelFont, 10, 20, 0, {255,255,255,255},RenderQueue::RENDER_DEBUG,true);
		
		float virmem,physmem;
		mPartInst->GetTotalMemoryUsage(virmem,physmem);

		lString = "Virt Mem: ";
		lString += std::to_string(virmem);
		mPartInst->mApp.GetModule<Render>().RenderText(lString.c_str(), mPartInst->mDebugPanelFont, 10, 45, 0, { 255,255,255,255 }, RenderQueue::RENDER_DEBUG, true);

		lString = "Phys Mem: ";
		lString += std::to_string(physmem);
		mPartInst->mApp.GetModule<Render>().RenderText(lString.c_str(), mPartInst->mDebugPanelFont, 10, 70, 0, {255,255,255,255 }, RenderQueue::RENDER_DEBUG, true);

		lString = "CPU Usage: ";

		mCPUUsageQueue.push(mPartInst->GetPercentCPUUsage());
		if (mCPUUsageQueue.size() > MAX_DEBUG_QUEUE_DATA_SAMPLE)
		{
			mCPUUsageQueue.pop();
		}

		float averageCPU = GetQueueMedianNumber(mCPUUsageQueue);

		lString += std::to_string(averageCPU);
		mPartInst->mApp.GetModule<Render>().RenderText(lString.c_str(), mPartInst->mDebugPanelFont, 10, 95, 0, { 255,255,255,255 }, RenderQueue::RENDER_DEBUG, true);

		lString = "Draw Calls: ";
		lString += std::to_string(mPartInst->mApp.GetModule<Render>().GetDrawCallsLastFrame());
		mPartInst->mApp.GetModule<Render>().RenderText(lString.c_str(), mPartInst->mDebugPanelFont, 10, 120, 0, { 255,255,255,255 }, RenderQueue::RENDER_DEBUG, true);

		lString = "Total Objects: ";
		lString += std::to_string(mPartInst->mApp.GetModule<ObjectManager>().GetTotalObjectNumber());
		mPartInst->mApp.GetModule<Render>().RenderText(lString.c_str(), mPartInst->mDebugPanelFont, 10, 145, 0, { 255,255,255,255 }, RenderQueue::RENDER_DEBUG, true);
	}

	if (mIsDebugSceneActive)
	{
		mPartInst->mApp.GetImplementation<ObjectManager,ObjectManager::ObjectManagerImpl>()->RenderDebug();
		mPartInst->mApp.GetImplementation<UserInterface,UserInterface::GuiImpl>()->RenderDebug();
	}

	return true;
}

bool Debug::DebugImpl::LoadConfig(pugi::xml_node& lNode)
{
	std::string lFont = lNode.child("debug_font").first_child().value();

	mPartInst->mDebugPanelFont = mPartInst->mApp.GetModule<Text>().LoadFont(lFont.c_str(), { 255,255,255,255 }, 18);
	return true;
}

bool Debug::DebugImpl::CreateConfig(pugi::xml_node& aNode)
{
	pugi::xml_node lNode = aNode.append_child("debug_node");
	lNode.text().set("Assets/Fonts/Lato-Regular.ttf");

	return true;
}

#pragma endregion

#pragma region PUBLIC API

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
	DebugImpl* lImpl = dynamic_cast<DebugImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return 0;
	}

	FILETIME ftime, fsys, fuser;
	ULARGE_INTEGER now, sys, user;
	double percent;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));

	GetProcessTimes(lImpl->self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));
	percent = (sys.QuadPart - lImpl->lastSysCPU.QuadPart) +
		(user.QuadPart - lImpl->lastUserCPU.QuadPart);
	percent /= (now.QuadPart - lImpl->lastCPU.QuadPart);
	percent /= lImpl->numProcessors;
	lImpl->lastCPU = now;
	lImpl->lastUserCPU = user;
	lImpl->lastSysCPU = sys;

	return percent * 100;
}

bool Debug::IsDebugActive()
{
	DebugImpl* lImpl = dynamic_cast<DebugImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return 0;
	}

	return lImpl->mIsDebugSceneActive;
};

#pragma endregion