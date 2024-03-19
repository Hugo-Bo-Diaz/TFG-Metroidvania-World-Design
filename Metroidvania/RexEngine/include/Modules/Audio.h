#ifndef AUDIO__H
#define AUDIO__H

#include "Part.h"
#include "PartsDef.h"

struct _Mix_Music;
struct Mix_Chunk;

typedef int AudioID;

class DLL_EXPORT Audio : public Part
{
public:
	Audio(EngineAPI& aAPI);
	~Audio() {};
	
	AudioID LoadMusic(const char* file, float fade= 500.0f,float volume = 1.0f);
	AudioID LoadSFX(const char* file, float volume = 1.0f);

	void PlayMusic(AudioID music_id,float fade_in_ms);
	uint PlaySFX(AudioID sfx_id, int repeat = 0, uint channel=-1);

	void StopMusic();
	void StopChannel(uint channel);

	/*from 100 to 0*/
	void SetGeneralVolume(float aVolume);
	float GetGeneralVolume();
	void SetMusicVolume(float aVolume);
	float GetMusicVolume();
	void SetSFXVolume(float aVolume);
	float GetSFXVolume();

	void SetFadeOut(float aFade);
	AudioID GetCurrentSong();
	void SetNextSongAfterFadeOut(AudioID aNextSong);

	void StartNextSongAfterFadeOut() {};

private:
	class AudioImpl;
};
#endif