#ifndef CLING__CREATURE__H
#define CLING_CREATURE__H

#include "Physics.h"
#include "Animation.h"
#include "Enemy.h"

enum ClingCreatureDirection {
	CLINGCREATURE_RIGHT,
	CLINGCREATURE_DOWN,
	CLINGCREATURE_LEFT,
	CLINGCREATURE_UP,
};

class ClingCreature : public Enemy
{
public:
	ClingCreature();
	~ClingCreature();

	bool Loop(float dt);
	bool Render();

	ClingCreatureDirection curr_dir;

	Animation animation;
	float angle;

	float speed_param = 1;

	float animation_pace=500;
	Timer animation_timer;

	void TurnCorner(bool clockwise);
	SDL_Rect check_down;
	SDL_Rect check_front;

	void RecieveDamage(int dmg, int direction);
	int health = 3;

	int timesturned = 0;
	bool hasturned = false;
	bool shouldfall = false;
	float accy = 1.0;
	float speed_y = 0;
};

#endif // !CLING__CREATURE__H
