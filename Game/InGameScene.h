#ifndef INGAMESCENE__H
#define INGAMESCENE__H

#include "Scene.h"

class InGameScene : public Scene
{
public:

	bool Create();
	bool Start();
	bool Loop();
	bool Stop();
	bool Destroy();

private:

	SDL_Rect Wall_limit_left;
	SDL_Rect Wall_limit_right;
	SDL_Rect Floor_left;
	SDL_Rect Floor_right;
	SDL_Rect Middle_platform;
	SDL_Rect Left_platform;
	SDL_Rect Right_platform;
	SDL_Rect Ceiling;
};

#endif //INGAMESCENE__H
