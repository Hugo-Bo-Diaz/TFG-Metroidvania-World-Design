#ifndef AUDIO_IMPL__H
#define AUDIO_IMPL__H

#include "../include/Modules/Audio.h"
#include "PartImpl.h"

struct Music
{
	std::string path;
	uint id;
	_Mix_Music* music;
	float volume = 1.0f;
	float fade;

	Music(const char* aPath, uint aID, _Mix_Music* aSFX, float aVolume, float aFade)
		:path(std::string(aPath)), id(aID), music(aSFX), volume(aVolume), fade(aFade) {};
};

struct SFX
{
	std::string path;
	uint id;
	Mix_Chunk* sfx;
	float volume = 1.0f;

	SFX(const char* aPath, uint aID, Mix_Chunk* aSFX, float aVolume) 
		:path(std::string(aPath)), id(aID), sfx(aSFX), volume(aVolume) {};
};

class Audio::AudioImpl : public Part::Part_Impl
{
public:

	Audio::AudioImpl(Audio* aAudio):mPartInst(aAudio){};

	void SetAudioInst(Audio* aAudio)
	{
		mPartInst = aAudio;
	}

protected:
	bool Init();
	bool CleanUp();

	bool LoadConfig(pugi::xml_node& config_node);
	bool CreateConfig(pugi::xml_node& config_node);

private:
	int mAudioCount = 0;
	std::map<AudioID, Music*> music_list;
	std::map<AudioID, SFX*> sfx_list;
	friend class Audio;

	float settings_volume = 100;
	float music_volume = 100;
	float sfx_volume = 100;

	AudioID next_song_after_fade_out;
	float current_fade_out;
	float next_song_after_fade_out_fade_time;
	AudioID current_song = -1;

	int GetFirstFreeChannel();

	Audio* mPartInst;
};

#endif // !AUDIO_IMPL__H