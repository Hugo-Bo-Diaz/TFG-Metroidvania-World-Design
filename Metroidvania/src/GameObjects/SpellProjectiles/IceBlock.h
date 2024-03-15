#ifndef ICE__BLOCK__H
#define ICE__BLOCK__H

#include "Modules/ObjectManager.h"
#include "EngineElements/Animation.h"
#include "Modules/Textures.h"

class IceBlock : public GameObject
{
public:

	IceBlock();
	IceBlock(std::list<ObjectProperty*>& aList) { new (this) IceBlock; };
	~IceBlock();

	int wall_id;

	void Init();

	bool Loop(float dt);
	bool Render();

	RXRect spr_on_img = { 160, 64,64,64};

	Timer timer;
	float life_expectancy = 5000;
	TextureID spells;
};

#endif // !PLAYER__REP__H
