#ifndef AUDIO__H
#define AUDIO__H

#include <vector>
#include <string>
#include "Part.h"
#include "PartsDef.h"

struct _Mix_Music;
struct Mix_Chunk;

enum AudioSFX
{
	SFX_JUMP,
	SFX_PLAYER_HIT,
	SFX_FIREBALL_BIG,
	SFX_FIREBALL_SMALL,
	SFX_FIRE_WATERFALL,
	SFX_FIRESHIELD,
	SFX_ROCK_THROW,
	SFX_GROUNDPOUND_START,
	SFX_SHOCKWAVE,

	SFX_ENEMY_HIT,
	SFX_ENEMY_PING,
	SFX_GROUND_HIT,

	SFX_MENU_CHANGE,
	SFX_MENU_SELECT
};


enum AudioMusic
{
	MUSIC_MENU,
	MUSIC_FIRST_CASTLE,
	MUSIC_SECOND_CASTLE,
	MUSIC_MAGIC_AREA
};

struct Music
{
	std::string name;
	uint id;
	_Mix_Music* music;
	float volume = 1.0f;
	float fade;
};

struct SFX
{
	std::string name;
	uint id;
	Mix_Chunk* sfx;
	float volume = 1.0f;
};


class Audio : public Part
{
public:
	Audio();
	~Audio() {};
	bool Init();
	bool Loop(float dt);
	bool CleanUp();

	bool LoadConfig(pugi::xml_node& config_node);
	bool CreateConfig(pugi::xml_node& config_node);

	uint LoadMusic(const char* file,const char* name,float fade= 500.0f,float volume = 1.0f);
	uint LoadSFX(const char* file, const char* name, float volume = 1.0f);

	void PlayMusic(uint music_id,float fade_in_ms);
	uint PlaySFX(uint sfx_id, int repeat = 0, uint channel=-1);

	void StopMusic();
	void StopChannel(uint channel);

	float settings_volume = 100;
	float music_volume = 100;
	float sfx_volume = 100;

	float prev_sett_volume = 100;
	float prev_mus_volume = 100;
	float prev_sfx_volume = 100;
	void RecalculateVolume();

	int GetFirstFreeChannel();

	float current_fade_out;

	_Mix_Music* next_song_after_fade_out;
	float next_song_after_fade_out_fade_time;

	uint current_song = -1;
private:
	std::vector<Music*> music_list;
	std::vector<SFX*> sfx_list;
};

void StartNextSongAfterFadeOut();

#endif