#ifndef AUDIO__H
#define AUDIO__H

#include <vector>
#include <string>
#include "Part.h"
#include "PartsDef.h"

struct _Mix_Music;
struct Mix_Chunk;

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

	int GetFirstFreeChannel();

	float current_fade_out;

	_Mix_Music* next_song_after_fade_out;
	float next_song_after_fade_out_fade_time;

private:
	std::vector<Music*> music_list;
	std::vector<SFX*> sfx_list;
};

void StartNextSongAfterFadeOut();

#endif