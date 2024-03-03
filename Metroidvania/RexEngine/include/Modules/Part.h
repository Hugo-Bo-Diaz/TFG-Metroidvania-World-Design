#ifndef PART__H
#define PART__H

#include<string>
#include "EngineAPI.h"

#define DLL_EXPORT __declspec(dllexport)

#include "pugiXML\src\pugixml.hpp"

class Part
{
protected:
	Part(const char* aName, EngineAPI& aApi):name(aName),mApp(aApi)
	{ };
	~Part() {};
	std::string name;

	virtual bool LoadConfig(pugi::xml_node&) { return true; };
	virtual bool CreateConfig(pugi::xml_node&) { return true; };

	virtual bool Init() { return true; }
	virtual bool Loop(float dt) { return true; };
	virtual bool CleanUp() { return true; };

	EngineAPI& mApp;

	friend class Application;

private:
	bool active=true;
	void activate() { active = true; }
	void deactivate() { active = false; }
};
#endif