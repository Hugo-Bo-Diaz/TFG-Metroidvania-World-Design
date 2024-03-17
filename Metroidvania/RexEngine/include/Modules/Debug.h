#ifndef DEBUG__H
#define DEBUG__H

#include "PartsDef.h"
#include "Part.h"
#include <queue>
#include <Windows.h>

#define MAX_DEBUG_QUEUE_DATA_SAMPLE 60

typedef int FontID;

class DLL_EXPORT Debug : public Part
{
private:
	class DebugImpl;

public:
	Debug(EngineAPI&);

	bool IsDebugActive();

	bool GetTotalMemoryUsage(float& virtualmem,float& physicalmem);
	float GetPercentCPUUsage();
	
	FontID mDebugPanelFont;
};

#endif // !DEBUG__H