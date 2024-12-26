#ifndef SHOCKWAVE__H
#define SHOCKWAVE__H

#include "Modules/SceneController.h"
#include "Modules/Audio.h"
#include "EngineElements/Animation.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Render.h"

class ParticleEmitter;

class Shockwave : public GameObject
{
public:

	Shockwave();
	Shockwave(std::list<ObjectProperty*>& aList) { new (this) Shockwave; };
	~Shockwave();

	void Fire(bool left_dir, float speed);

	void Init();
	bool Loop(float dt);
	bool Render();

	float x_speed = 0;
	RXRect* nextpos = nullptr;

	Animation shockwave_left;
	Animation shockwave_right;

	RXPoint floor_check;

	ParticleEmitter* p;

	int damage = 2;
	int direction;

	particle_preset groundcontact;
	particle_preset rockblockexplosion;
	RXRect r8ground;
	RXRect r9ground;

	RexTextureID spells;
	RexTextureID particles;

	AudioID mSFXGroundHit;
};

#endif // !PLAYER__REP__H
