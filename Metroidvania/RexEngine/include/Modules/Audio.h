#ifndef AUDIO__H
#define AUDIO__H

#include "Part.h"
#include "PartsDef.h"

struct _Mix_Music;
struct Mix_Chunk;

typedef int AudioID;

//Module that processes all audio
class DLL_EXPORT Audio : public Part
{
public:
	Audio(EngineAPI& aAPI);
	~Audio() {};
	
	//loads a song and returns an ID
	AudioID LoadMusic(const char* file, float fade= 500.0f,float volume = 1.0f);
	//loads an SFX and returns an ID
	AudioID LoadSFX(const char* file, float volume = 1.0f);

	//plays a song
	void PlayMusic(AudioID music_id,float fade_in_ms);
	//plays an SFX and returns the channel it is playing
	uint PlaySFX(AudioID sfx_id, int repeat = 0, uint channel=-1);

	//stops the music
	void StopMusic();
	//stops the channel that is playing
	void StopChannel(uint channel);

	/*from 100 to 0*/
	//Sets the general volume
	void SetGeneralVolume(float aVolume);
	//Returns the General volume
	float GetGeneralVolume();
	//Sets the music volume
	void SetMusicVolume(float aVolume);
	//Returns the Music volume
	float GetMusicVolume();
	//Sets the SFX volume
	void SetSFXVolume(float aVolume);
	//Returns the SFX volume
	float GetSFXVolume();

	//Sets the time to fade out
	void SetFadeOut(float aFade);
	//Returns the current song that is currently playing
	AudioID GetCurrentSong();
	//Sets the next song to play after it fades out
	void SetNextSongAfterFadeOut(AudioID aNextSong);

	void StartNextSongAfterFadeOut() {};

private:
	class AudioImpl;
};
#endif