#include "RXpch.h"
#include "Utils/Logger.h"
#include "Utils/Utils.h"
#include "AudioImpl.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

Audio::Audio(EngineAPI& aAPI) : Part("Audio",aAPI)
{
	mPartFuncts = new AudioImpl(this);
}

#pragma region IMPLEMENTATION

bool Audio::AudioImpl::LoadConfig(pugi::xml_node& config_node)
{
	bool ret = true;

	pugi::xml_node& volume_node = config_node.child("volume");
	settings_volume = volume_node.attribute("general").as_float(100);
	music_volume = volume_node.attribute("music").as_float(100);
	sfx_volume = volume_node.attribute("sfx").as_float(100);

	return ret;
}

bool Audio::AudioImpl::CreateConfig(pugi::xml_node& config_node)
{
	bool ret = true;

	pugi::xml_node& volume_node = config_node.append_child("volume");
	volume_node.append_attribute("general") = 100;
	volume_node.append_attribute("music") = 100;
	volume_node.append_attribute("sfx") = 100;

	return ret;
}

bool Audio::AudioImpl::Init()
{
	//thisInst = this;
	Logger::Console_log(LogLevel::LOG_INFO,"Initializing SDL audio");

	bool ret = true;

	SDL_Init(0);
	if(SDL_InitSubSystem(SDL_INIT_AUDIO)<0)
	{
		Logger::Console_log(LogLevel::LOG_ERROR,"Could not initialize SDL audio");
		ret = false;
	}

	int flags = MIX_INIT_OGG;// | MIX_INIT_MP3; TO DO
	int init = Mix_Init(flags);

	if ((init & flags) != flags)
	{
		Logger::Console_log(LogLevel::LOG_ERROR,"could not initialize mixer lib.");
		std::string s = Mix_GetError();
		
		ret = false;
	}
	
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		Logger::Console_log(LogLevel::LOG_ERROR,"could not initialize SDL_Mixer");
		ret = false;
	}

	Mix_AllocateChannels(16);

	float real_music_volume = (music_volume * 128 / 100)*(settings_volume / 100);
	Mix_VolumeMusic(real_music_volume);

	float real_fx_volume = (sfx_volume * 128 / 100)*(settings_volume / 100);
	if (real_fx_volume < 0)
		real_fx_volume = 0;
	
	for (int i = 0; i < 8; ++i)
	{
		Mix_Volume(i, real_fx_volume);
	}

	std::list<std::string> lSongs;
	GetAllExtensionPathRecursive("", "ogg", lSongs);

	for (std::list<std::string>::iterator it = lSongs.begin(); it != lSongs.end(); it++)
	{
		mPartInst->LoadMusic((*it).c_str());
	}

	std::list<std::string> lSFX;
	GetAllExtensionPathRecursive("", "wav", lSFX);

	for (std::list<std::string>::iterator it = lSFX.begin(); it != lSFX.end(); it++)
	{
		mPartInst->LoadSFX((*it).c_str());
	}

	return ret;
}

bool Audio::AudioImpl::CleanUp()
{
	for (std::map<AudioID,SFX*>::iterator it = sfx_list.begin(); it != sfx_list.end(); it++)
	{
		Mix_FreeChunk((*it).second->sfx);
		delete(*it).second;
	}
	sfx_list.clear();

	for (std::map<AudioID,Music*>::iterator it = music_list.begin(); it != music_list.end(); it++)
	{
		Mix_FreeMusic((*it).second->music);
		delete(*it).second;
	}
	music_list.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

#pragma endregion

#pragma region PUBLIC API

AudioID Audio::LoadMusic(const char * file, float fade,float volume)
{
	AudioImpl* lImpl = dynamic_cast<AudioImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return 0;
	}

	for (std::map<AudioID, Music*>::iterator it = lImpl->music_list.begin(); it != lImpl->music_list.end(); it++)
	{
		if (std::strcmp((*it).second->path.c_str(), file) == 0)
		{
			return (*it).first;
		}
	}


	Mix_Music* lMusic= Mix_LoadMUS(file);
	if (lMusic == NULL)
	{
		Logger::Console_log(LogLevel::LOG_ERROR,"Can't load music");
		return -1;
	}
	else
	{
		lImpl->mAudioCount++;
		Music* new_music = new Music(file, lImpl->mAudioCount, lMusic, volume, fade);
		lImpl->music_list.insert(std::make_pair(lImpl->mAudioCount,new_music));
		return new_music->id;
	}
}

AudioID Audio::LoadSFX(const char * file, float volume)
{
	AudioImpl* lImpl = dynamic_cast<AudioImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return 0;
	}

	for (std::map<AudioID, SFX*>::iterator it = lImpl->sfx_list.begin(); it != lImpl->sfx_list.end(); it++)
	{
		if (std::strcmp((*it).second->path.c_str(), file) == 0)
		{
			return (*it).first;
		}
	}

	Mix_Chunk* lChunk = Mix_LoadWAV(file);
	if (lChunk == NULL)
	{
		Logger::Console_log(LogLevel::LOG_ERROR,"Can't load sfx");
		return -1;
	}
	else
	{
		lImpl->mAudioCount++;
		SFX* new_sfx = new SFX(file, lImpl->mAudioCount, lChunk, volume);
		lImpl->sfx_list.insert(std::make_pair(lImpl->mAudioCount, new_sfx));
		return new_sfx->id;
	}
}

void Audio::PlayMusic(AudioID music_id, float fade_in_ms)
{
	AudioImpl* lImpl = dynamic_cast<AudioImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	Music* music_selected = lImpl->music_list[music_id];
	if (music_id != lImpl->current_song)
	{
		lImpl->current_song = music_id;

		if (Mix_PlayingMusic() == 1)
		{
			//fade out music
			Mix_FadeOutMusic(lImpl->current_fade_out);
			lImpl->next_song_after_fade_out = music_selected->id;
			lImpl->next_song_after_fade_out_fade_time = music_selected->fade;
			//Mix_HookMusicFinished([]() {thisInst->StartNextSongAfterFadeOut(); });
			lImpl->current_fade_out = music_selected->fade;
		}
		else
		{
			Mix_PlayMusic(music_selected->music, -1);
			lImpl->current_fade_out = music_selected->fade;
		}
	}
}

uint Audio::PlaySFX(AudioID sfx_id, int repeat, uint channel)
{
	AudioImpl* lImpl = dynamic_cast<AudioImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return 0;
	}

	SFX* sfx_selected = lImpl->sfx_list[sfx_id];

	if (sfx_selected == nullptr)
	{
		return 0;
	}
	
	int chan = channel;
	if (channel == -1)
	{
		chan = lImpl->GetFirstFreeChannel();
	}

	int i = Mix_PlayChannel(chan, sfx_selected->sfx, repeat);
	return i;
}

void Audio::StopMusic()
{
	Mix_HaltMusic();
}

void Audio::StopChannel(uint channel)
{
	Mix_Pause(channel);
}

int Audio::AudioImpl::GetFirstFreeChannel()
{
	for (int i = 0; i < 8; ++i)
	{
		if (Mix_Playing(i) ==0)
		{
			return i;
		}
	}
	return -1;
}

void Audio::SetGeneralVolume(float aNewVolume)
{
	AudioImpl* lImpl = dynamic_cast<AudioImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}
	lImpl->settings_volume = aNewVolume;
}

float Audio::GetGeneralVolume()
{
	AudioImpl* lImpl = dynamic_cast<AudioImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return 0;
	}

	return lImpl->settings_volume;
}

void Audio::SetMusicVolume(float aNewVolume)
{
	AudioImpl* lImpl = dynamic_cast<AudioImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	float setvol = lImpl->settings_volume / 100;
	//keep volume val between 0 and 100
	aNewVolume = min(max(aNewVolume, 0), 100);

	lImpl->music_volume = aNewVolume;

	float musvol = setvol * (lImpl->music_volume * (MIX_MAX_VOLUME / 100));
	Mix_VolumeMusic(musvol);
}

float Audio::GetMusicVolume()
{
	AudioImpl* lImpl = dynamic_cast<AudioImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return 0;
	}

	return lImpl->music_volume;
}

void Audio::SetSFXVolume(float aNewVolume)
{
	AudioImpl* lImpl = dynamic_cast<AudioImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	float setvol = lImpl->settings_volume / 100;
	//keep volume val between 0 and 100
	aNewVolume = min(max(aNewVolume, 0), 100);

	lImpl->sfx_volume = aNewVolume;

	float musvol = setvol * (lImpl->sfx_volume * (MIX_MAX_VOLUME / 100));
	Mix_VolumeMusic(musvol);
}

float Audio::GetSFXVolume()
{
	AudioImpl* lImpl = dynamic_cast<AudioImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return 0;
	}

	return lImpl->sfx_volume;
}

AudioID Audio::GetCurrentSong()
{
	AudioImpl* lImpl = dynamic_cast<AudioImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return 0;
	}

	return lImpl->current_song;
}

void Audio::SetFadeOut(float aNewFadeOut)
{
	AudioImpl* lImpl = dynamic_cast<AudioImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	lImpl->current_fade_out = aNewFadeOut;
}

void Audio::SetNextSongAfterFadeOut(AudioID aSongAfterFade)
{
	AudioImpl* lImpl = dynamic_cast<AudioImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	lImpl->next_song_after_fade_out = aSongAfterFade;
}

#pragma endregion
//void Audio::StartNextSongAfterFadeOut()
//{
//	Mix_FadeInMusic(next_song_after_fade_out, -1, next_song_after_fade_out_fade_time);
//
//}
