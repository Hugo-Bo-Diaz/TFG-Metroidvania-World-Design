#ifndef PART__H
#define PART__H

#include<string>

#include "pugiXML\src\pugixml.hpp"

class Part
{
public:
	std::string name;

	Part() {};
	~Part() {};

	virtual bool LoadConfig(pugi::xml_node&) { return true; };
	virtual bool CreateConfig(pugi::xml_node&) { return true; };

	virtual bool Init() { return true; }
	virtual bool Loop(float dt) { return true; };
	virtual bool CleanUp() { return true; };

	bool active=true;
	void activate() { active = true; }
	void deactivate() { active = false; }
private:


};
#endif