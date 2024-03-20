#ifndef DEBUG__H
#define DEBUG__H

#include "PartsDef.h"
#include "Part.h"
#include <queue>
#include <Windows.h>

#define MAX_DEBUG_QUEUE_DATA_SAMPLE 60

typedef int FontID;
//module that contains debug functionalities
class DLL_EXPORT Debug : public Part
{
private:
	class DebugImpl;

public:
	Debug(EngineAPI&);

	//returns true if debug is active, false otherwise
	bool IsDebugActive();

	//returns memory usage
	bool GetTotalMemoryUsage(float& virtualmem,float& physicalmem);
	//returns percent usage in a moment
	float GetPercentCPUUsage();

	//activates the debug of the scene
	void ActivateDebugScene();
	//deactivates the debug of the scene
	void DeActivateDebugScene();
	
	//activates the debug of the performance
	void ActivateDebugPerformance();
	//deactivates the debug of the performance
	void DeActivateDebugPerformance();
	
	FontID mDebugPanelFont;
};

#endif // !DEBUG__H