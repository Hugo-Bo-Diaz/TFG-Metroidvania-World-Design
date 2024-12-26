#ifndef ENEMY__DEF__H
#define ENEMY__DEF__H

#include "Modules/SceneController.h"
#include "Utils/Timer.h"

class Enemy : public GameObject
{
public:

	//returns true if it survives
	virtual bool RecieveDamage(int dmg, int direction) { return true; };

	virtual bool CanFreeze() { return true; };
};



#endif