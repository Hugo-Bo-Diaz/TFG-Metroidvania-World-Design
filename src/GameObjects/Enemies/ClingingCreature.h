#ifndef CLING__CREATURE__H
#define CLING_CREATURE__H

#include "Modules/ObjectManager.h"
#include "EngineElements/Animation.h"
#include "Enemy.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Textures.h"
#include "Modules/Audio.h"

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
	ClingCreature(std::list<ObjectProperty*>&);

	void Init();
	bool Loop(float dt);
	bool Render();
	void RenderDebug();
	void Destroy();

	ClingCreatureDirection curr_dir;

	Animation animation;
	float angle;
	particle_preset stone_death;
	particle_preset metal_and_sand;
	SDL_Rect r7buff;
	SDL_Rect r16sandfirst;
	SDL_Rect r17sandsecond;
	SDL_Rect r18metalfirst;

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

	TextureID cling_enemy;
	TextureID particles;

	AudioID mSFXHit;
};

#endif // !CLING__CREATURE__H
