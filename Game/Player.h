#ifndef PLAYER__H
#define PLAYER__H

#include "Physics.h"
#include "Console.h"
#include "Animation.h"

#include "Spell.h"

enum state
{
IDLE,

WALKING_LEFT,
WALKING_RIGHT,

AIRBORNE_LEFT,
AIRBORNE_RIGHT,

LAG
};

enum spell_type
{
	FIRE,
	WIND,
	WATER,
	GRASS,
	GROUND,
	NONE_UNLOCKED
};

class Player : public physobj
{
public:
	Player();
	~Player();
	state current_state = IDLE;
	bool Loop(float dt);
	bool Render();

	float min_invin_interval = 0;
	float max_invin_interval=200;
	float current_draw_interval = max_invin_interval;
	Timer invin_draw_timer;
	bool ShouldDraw();
	bool is_drawing = true;
	int separation = 5;

	int x = 0;
	int y = 0;

	float speed_x = 0;
	float speed_y = -10;

	float speed_y_cap = 30;
	
	int spd=0;

	float acceleration_x = 0;
	float acceleration_y = 1.0;

	bool grounded = true;

	void LockMovement(float time_in_ms =-1);
	float time_to_lock_movement=0;
	bool movement_is_locked = false;
	void UnlockMovement();
	Timer movement_lock;

	Timer invincibility;
	bool is_invincible = false;
	float time_invincible=1000;
	void StartInvincibility();
	float GetInvincibilityPercent();

	//STATS
	float max_speed = 6.0;
	float accel_x = 1;

	float mobility_multiplier = 1.0f;

	bool is_right = true;

	Animation idle_right;
	Animation idle_left;
	Animation walking_right;
	Animation walking_left;
	Animation air_right;
	Animation air_left;

	var_track* pos_x;
	var_track* pos_y;

	var_track* xspeed;

	bool left;
	bool right;
	bool jump;
	bool stop;

	SDL_Point trail[30];

	spell_type current_spell = NONE_UNLOCKED;//=FIRE;
	//spell_type current_spell =FIRE;

	std::vector<Spell*> spells;
	int unlocked_spells;
	std::vector<bool> unlocked;

	void unlock_spell(spell_type to_unlock);

	void cycle_spell(int direction);

	int get_next_spell(int direction);

	float max_health = 4;
	float max_mana = 3;
	
	float health = 4;
	float mana = 3;
	Timer time_since_mana_use;
	float time_to_start_regen = 600;
	float base_mana_regen = 0.007;//always add this
	float scaling_mana_regen = 0.08;//multiply this by the percent of current mana

	bool manaCost(float mana);

	void AddHealth(int amount, int knockbackdirection = -1);
	void AddMana(float amount);

	void AddMaxHealth();
	void AddMaxMana();
};

#endif // !PLAYER__H
