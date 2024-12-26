#ifndef HAZARD__GROUND__CRUMBLE__H
#define HAZARD__GROUND__CRUMBLE__H

#include "Modules/SceneController.h"
#include "Utils/Timer.h"
#include "EngineElements/ParticleEmitter.h"

class HazardGroundCrumble :
	public GameObject
{
public:
	HazardGroundCrumble();
	HazardGroundCrumble(std::list<ObjectProperty*>& aList) { new (this) HazardGroundCrumble; };

	void Init();
	bool Loop(float dt);
	bool Render();

	void Destroy();
	RXRect check_collider;

	RXRect tex_rect;
	RexTextureID hazards;

	float crumble_time = 30;
	Timer crumble_timer;

	int wallId;
	particle_preset particle_Crumble;
	RexTextureID particles;
	RXRect rPart1;
	RXRect rPart2;
};

#endif
