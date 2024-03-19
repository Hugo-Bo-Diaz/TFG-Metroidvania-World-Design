#ifndef PART__H
#define PART__H

#include <string>

#define DLL_EXPORT __declspec(dllexport)

class EngineAPI;

class Part
{
protected:
	Part(const char* aName, EngineAPI& aApi) :name(aName), mApp(aApi)
	{ };
	virtual ~Part() {};

	std::string name;
	EngineAPI& mApp;

	class Part_Impl;
	Part_Impl* mPartFuncts;

	friend class Application;
	friend class EngineAPI;
};
#endif