#ifndef ENEMY__DEF__H
#define ENEMY__DEF__H

#include "Modules/ObjectManager.h"

class Enemy : public GameObject
{
public:
	virtual void RecieveDamage(int dmg, int direction) {};

};



#endif