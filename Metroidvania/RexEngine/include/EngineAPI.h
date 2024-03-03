#ifndef ENGINE_API__H
#define ENGINE_API__H

#include "Application.h"
#include "Utils/Logger.h"
#include "Modules/Part.h"

#include <unordered_set>

class DLL_EXPORT EngineAPI
{
private:
	Application* mApplication;
public:
	~EngineAPI() {};

	EngineAPI(EngineAPI& aAPI)
	{
		this->mApplication = aAPI.mApplication;
	};
	EngineAPI(Application* aApp) : mApplication(aApp) {};

	std::unordered_set<Part*> mDisabledModules;

	template<class T>
	T& GetModule()
	{
		for (std::list<Part*>::iterator it = mApplication->parts.begin(); it != mApplication->parts.end(); it++)
		{
			if (dynamic_cast<T*>(*it)!= nullptr && mDisabledModules.count(*it) == 0)
			{
				return (*(T*)(*it));
			}
		}

		Logger::Console_log(LogLevel::LOG_ERROR, "Could not get module since type was not registered as one");
	}

	template<class T>
	bool DisableModuleAPIMod()
	{
		for (std::list<Part*>::iterator it = mApplication->parts.begin(); it != mApplication->parts.end(); it++)
		{
			if (dynamic_cast<T*>(*it) != nullptr && mDisabledModules.count(*it) == 0)
			{
				mDisabledModules.insert(*it);
				return true;
			}
		}
		return false;
	}

	template<class T>
	bool EnableModuleAPIMod()
	{
		for (std::list<Part*>::iterator it = mApplication->parts.begin(); it != mApplication->parts.end(); it++)
		{
			if (dynamic_cast<T*>(*it) != nullptr)
			{
				auto lIt = mDisabledModules.find(*it);
				mDisabledModules.erase(lIt);
			}
		}
	}

	void Quit() {};
	float GetLastUpdateTime() { return mApplication->GetLastUpdateTime(); };

	friend class Application;
};


#endif // !ENGINE_API__H
