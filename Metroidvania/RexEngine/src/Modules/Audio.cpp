#include "Application.h"
#include "Utils/Logger.h"
#include "Utils/Utils.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

#include <string.h>
#include <functional>
#include<iostream>

#include "AudioImpl.h"


Audio::Audio(EngineAPI& aAPI) : Part("Audio",aAPI)
{
	mPartFuncts = new AudioImpl(this);
}

bool Audio::AudioImpl::LoadConfig(pugi::xml_node& config_node)
{
	bool ret = true;

	pugi::xml_node& volume_node = config_node.child("volume");
	mPartInst->settings_volume = volume_node.attribute("general").as_float(100);
	mPartInst->music_volume = volume_node.attribute("music").as_float(100);
	mPartInst->sfx_volume = volume_node.attribute("sfx").as_float(100);

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

	float real_music_volume = (mPartInst->music_volume * 128 / 100)*(mPartInst->settings_volume / 100);
	Mix_VolumeMusic(real_music_volume);

	float real_fx_volume = (mPartInst->sfx_volume * 128 / 100)*(mPartInst->settings_volume / 100);
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

bool Audio::AudioImpl::Loop(float dt)
{
	if(mPartInst->prev_sett_volume != mPartInst->settings_volume || mPartInst->prev_mus_volume != mPartInst->music_volume || mPartInst->prev_sfx_volume != mPartInst->sfx_volume)
	{
		mPartInst->RecalculateVolume();

		mPartInst->prev_sett_volume = mPartInst->settings_volume;
		mPartInst->prev_mus_volume = mPartInst->music_volume;
		mPartInst->prev_sfx_volume = mPartInst->sfx_volume;
	}

	return true;
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

	Music* new_music = new Music();
	new_music->id = lImpl->music_list.size();
	new_music->path = file;
	new_music->fade = fade;
	new_music->volume = volume;

	new_music->music = Mix_LoadMUS(file);
	if (new_music->music == NULL)
	{
		Logger::Console_log(LogLevel::LOG_ERROR,"Can't load music");
		delete new_music;
		return -1;
	}
	else
	{
		uint lNextAudioID = lImpl->music_list.size() + lImpl->sfx_list.size();
		lImpl->music_list.insert(std::make_pair(lNextAudioID,new_music));
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

	SFX* new_sfx = new SFX();
	new_sfx->id = lImpl->sfx_list.size();
	new_sfx->volume = volume;
	new_sfx->path = file;

	new_sfx->sfx = Mix_LoadWAV(file);
	if (new_sfx->sfx == NULL)
	{
		Logger::Console_log(LogLevel::LOG_ERROR,"Can't load sfx");
		delete new_sfx;
		return -1;
	}
	else
	{
		uint lNextAudioID = lImpl->music_list.size() + lImpl->sfx_list.size();
		lImpl->sfx_list.insert(std::make_pair(lNextAudioID,new_sfx));
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
	if (music_id != current_song)
	{
		current_song = music_id;

		if (Mix_PlayingMusic() == 1)
		{
			//fade out music
			Mix_FadeOutMusic(current_fade_out);
			lImpl->next_song_after_fade_out = music_selected->music;
			next_song_after_fade_out_fade_time = music_selected->fade;
			//Mix_HookMusicFinished([]() {thisInst->StartNextSongAfterFadeOut(); });
			current_fade_out = music_selected->fade;
		}
		else
		{
			Mix_PlayMusic(music_selected->music, -1);
			current_fade_out = music_selected->fade;
		}
	}
	else
	{

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
	
	int chan = channel;
	if (channel == -1)
	{
		chan = GetFirstFreeChannel();
	}
	//Mix_Resume(chan);

	int i = Mix_PlayChannel(chan, sfx_selected->sfx, repeat);

	//return chan;
	return i;
}

void Audio::StopMusic()
{
	Mix_HaltMusic();
}

void Audio::StopChannel(uint channel)
{
	//Mix_HaltChannel(channel);
	//Mix_PlayChannel(channel, sfx_list[SFX_ENEMY_HIT]->sfx,1);
	Mix_Pause(channel);
}

void Audio::RecalculateVolume()
{
	float setvol = settings_volume / 100;

	float musvol = setvol * (music_volume * (MIX_MAX_VOLUME / 100));
	Mix_VolumeMusic(musvol);

	float sfxvol = setvol * (sfx_volume * (MIX_MAX_VOLUME / 100));
	Mix_Volume(-1,sfxvol);

}

int Audio::GetFirstFreeChannel()
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

//void Audio::StartNextSongAfterFadeOut()
//{
//	Mix_FadeInMusic(next_song_after_fade_out, -1, next_song_after_fade_out_fade_time);
//
//}
