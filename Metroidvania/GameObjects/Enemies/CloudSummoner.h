#ifndef CLOUD__SUMMONER__H
#define CLOUD__SUMMONER__H

#include "Physics.h"
#include "Animation.h"
#include "Enemy.h"

enum CloudSummonerState
{
	CS_PATROL,
	CS_AGGRO
};

class CloudSummoner : public Enemy
{
public:
	CloudSummoner();
	CloudSummoner(float initial_x, float initial_y);
	~CloudSummoner();

	bool Loop(float dt);
	bool Render();

	CloudSummonerState state = CS_PATROL;
	CloudSummonerState last_state = CS_PATROL;
	float health = 1;

	void RecieveDamage(int dmg, int direction);

	float x;
	float y;

	float initial_y;
	float initial_x;

	//graphics
	Animation facing_left;
	Animation facing_right;
	Animation facing_front;
	Animation book;

	//patrol
	float patrol_speed = 2.5;

	SDL_Rect aggro = { 0,0,600,600 };

	//follow
	float min_distance_to_player = 200;
	float max_distance_to_player = 400;
	float speed_follow = 2;
	float max_distance_from_origin = 1000;
	GameObject* following;
	bool home = true;

	Timer shooting_timer;
	float shooting_timer_charge = 900;
	float shooting_timer_total_cycle = 4000;

	float projectile_speed = 6;

	//other
	float speed_x = patrol_speed;
	float speed_y = -10;

	//BOOK MOVEMENT
	float base_book_offset_x;
	float base_book_offset_y;

	float oscilated_book_x;
	float oscilated_book_y;

	float oscilated_book_strength = 16;
	int book_angle;

	//MOVEMENT OSCILATION
	float oscilated_x;
	float oscilated_y;

	float oscilated_diff_x;
	float oscilated_diff_y;

	int perm = 0;

};

#endif