#ifndef SCENECONTROLLER__H
#define SCENECONTROLLER__H

#include "PartsDef.h"
#include<vector>
#include "Text.h"

class SceneController : public Part
{
public:
	SceneController();

	bool Init();
	bool Loop(float dt);
	bool CleanUp();

	bool LoadMap(const char* filename);

	bool LoadBackground(pugi::xml_node&);
	bool LoadWalls(pugi::xml_node&);
	bool LoadObjects(pugi::xml_node&);

	TextPrint* p;
	TextPrint* c;
	TextPrint* v;

	uint song_try;

private:

	std::vector<std::string>backgrounds;
};

#endif // !SCENECONTROLLER__H

