#ifndef AUDIO__H
#define AUDIO__H

#include <map>
#include <string>
#include "Part.h"
#include "PartsDef.h"

struct _Mix_Music;
struct Mix_Chunk;

typedef int AudioID;

struct Music
{
	std::string path;
	uint id;
	_Mix_Music* music;
	float volume = 1.0f;
	float fade;
};

struct SFX
{
	std::string path;
	uint id;
	Mix_Chunk* sfx;
	float volume = 1.0f;
};


class DLL_EXPORT Audio : public Part
{
private:
	bool Init();
	bool Loop(float dt);
	bool CleanUp();

	bool LoadConfig(pugi::xml_node& config_node);
	bool CreateConfig(pugi::xml_node& config_node);

	std::map<AudioID,Music*> music_list;
	std::map<AudioID,SFX*> sfx_list;

	//static Audio* thisInst;
public:
	Audio(EngineAPI& aAPI);
	~Audio() {};
	
	AudioID LoadMusic(const char* file, float fade= 500.0f,float volume = 1.0f);
	AudioID LoadSFX(const char* file, float volume = 1.0f);

	void PlayMusic(AudioID music_id,float fade_in_ms);
	uint PlaySFX(AudioID sfx_id, int repeat = 0, uint channel=-1);

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
	void StartNextSongAfterFadeOut();
};
#endif