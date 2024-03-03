#ifndef SPELL__H
#define SPELL__H

#include "EngineAPI.h"
class Player;

class Spell
{
public:
	Spell(EngineAPI* aAPI):Engine(aAPI){}

	Player* player;

	virtual void Init() {};

	virtual void Loop(float dt) {};
	virtual void Render() {};

	virtual void CleanUp() {};

	virtual void UnlockMovementEvent() {};

	virtual void Switched_in() {};
	virtual void Switched_out() {};

protected:
	EngineAPI* Engine;
};

#endif