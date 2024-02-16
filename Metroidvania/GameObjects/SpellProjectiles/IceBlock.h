#ifndef ICE__BLOCK__H
#define ICE__BLOCK__H

#include "Physics.h"
#include "Animation.h"
#include "Textures.h"

class IceBlock : public GameObject
{
public:

	IceBlock();
	//~IceBlock();

	int wall_id;

	void Init();

	bool Loop(float dt);
	bool Render();

	SDL_Rect spr_on_img = { 160, 64,64,64};

	Timer timer;
	float life_expectancy = 5000;
	TextureID spells;
};

#endif // !PLAYER__REP__H
