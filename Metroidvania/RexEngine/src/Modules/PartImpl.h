	#ifndef PART_IMPL__H
#define PART_IMPL__H

#include "../include/Modules/Part.h"
#include "RXpch.h"
#include "pugiXML\src\pugixml.hpp"

class Part::Part_Impl
{
public:
	~Part_Impl() {};
protected:

	virtual bool LoadConfig(pugi::xml_node&) { return true; };
	virtual bool CreateConfig(pugi::xml_node&) { return true; };

	virtual bool Init() { return true; }
	virtual bool Loop(float dt) { return true; };
	virtual bool CleanUp() { return true; };

	friend class Application;

private:
	bool active = true;
	void activate() { active = true; }
	void deactivate() { active = false; }
};

#endif // !1