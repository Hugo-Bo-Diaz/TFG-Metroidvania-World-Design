#ifndef WATER__BUCKET__DROP__H
#define WATER__BUCKET__DROP__H

#include "Modules/SceneController.h"
#include "EngineElements/Animation.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Audio.h"

struct WaterParticle
{
	WaterParticle(float _x, float _y) : x(_x),y(_y){}
	float x, y;
	float v_x, v_y;
	int lRect;
	float mLife;
};

class WaterBucketDrop : public GameObject
{
public:

	WaterBucketDrop(float aAmountOfParticles);
	WaterBucketDrop();
	WaterBucketDrop(std::list<ObjectProperty*>& aList);
	~WaterBucketDrop() {};

	int mAmountOfParticles = 30;
	int msizeOfParticles = 12;
	int mIsRight = false;
	float mMinLife = 1500;
	float mMaxLife = 2000;

	std::vector<WaterParticle*> mParticles;
	std::vector<RXRect> mRectangles;

	void Init();
	bool Loop(float dt);
	bool Render();
	void Destroy();

	int damage = 2;
	bool mHasCollided = false;

	RexTextureID particles;

	Timer mLifeTime;
};

#endif // !PLAYER__REP__H
