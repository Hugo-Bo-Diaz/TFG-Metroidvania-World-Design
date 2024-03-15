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
};

struct SFX
{
	std::string path;
	uint id;
	Mix_Chunk* sfx;
	float volume = 1.0f;
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
	bool Loop(float dt);
	bool CleanUp();

	bool LoadConfig(pugi::xml_node& config_node);
	bool CreateConfig(pugi::xml_node& config_node);

private:
	std::map<AudioID, Music*> music_list;
	std::map<AudioID, SFX*> sfx_list;
	_Mix_Music* next_song_after_fade_out;
	friend class Audio;

	Audio* mPartInst;
};

#endif // !AUDIO_IMPL__H