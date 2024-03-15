#ifndef FIRE__BALL__H
#define FIRE__BALL__H

#include "Modules/ObjectManager.h"
#include "EngineElements/Animation.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Audio.h"

class FireBall : public GameObject
{
public:

	FireBall();
	FireBall(std::list<ObjectProperty*>& aList) { new (this) FireBall; };

	void Destroy();


	void Fire(bool left_dir, bool is_big);

	void Init();
	bool Loop(float dt);
	bool Render();

	float speed = 15;
	int direction;

	bool is_big = false;

	Animation fireball_small;
	Animation fireball_big;

	SDL_Rect r1exp;
	particle_preset explosion;

	int damage = 2;

	TextureID spells;
	TextureID particles;

	AudioID mSFXGroundHit;
};

#endif // !PLAYER__REP__H
