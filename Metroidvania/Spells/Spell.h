#ifndef SPELL__H
#define SPELL__H

class Player;

class Spell
{
public:

	Player* player;

	virtual void Init() {};

	virtual void Loop(float dt) {};
	virtual void Render() {};

	virtual void CleanUp() {};

	virtual void UnlockMovementEvent() {};

	virtual void Switched_in() {};
	virtual void Switched_out() {};
};

#endif