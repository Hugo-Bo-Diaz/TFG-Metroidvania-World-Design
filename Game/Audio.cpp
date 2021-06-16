#include "Audio.h"
#include "Application.h"
#include "Console.h"
#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

#include <string.h>
Audio::Audio()
{
	name = "Audio";
}

bool Audio::LoadConfig(pugi::xml_node& config_node)
{
	bool ret = true;

	pugi::xml_node& volume_node = config_node.child("volume");
	settings_volume = volume_node.attribute("general").as_float(100);
	music_volume = volume_node.attribute("music").as_float(100);
	sfx_volume = volume_node.attribute("sfx").as_float(100);

	return ret;
}

bool Audio::CreateConfig(pugi::xml_node& config_node)
{
	bool ret = true;

	pugi::xml_node& volume_node = config_node.append_child("volume");
	volume_node.append_attribute("general") = 100;
	volume_node.append_attribute("music") = 100;
	volume_node.append_attribute("sfx") = 100;


	return ret;
}

bool Audio::Init()
{
	App->con->Console_log("Initializing audio");

	bool ret = true;

	SDL_Init(0);
	if(SDL_InitSubSystem(SDL_INIT_AUDIO)<0)
	{
		App->con->Console_log("Could not initialize audio");
		ret = false;
	}

	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if ((init & flags) != flags)
	{
		App->con->Console_log("could not initialize mixer lib.");
		std::string s = Mix_GetError();
		
		ret = false;
	}
	
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		App->con->Console_log("could not initialize SDL_Mixer");
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

	LoadMusic("Assets/Music/not-in-vain.ogg", "menu");
	LoadMusic("Assets/Music/first-story.ogg", "first_castle");
	LoadMusic("Assets/Music/underground-town.ogg", "second_castle");
	LoadMusic("Assets/Music/in-the-city.ogg", "magic_area");

	LoadSFX("Assets/SFX/jump.wav","jump");
	LoadSFX("Assets/SFX/player_hit.wav", "player_hit");

	LoadSFX("Assets/SFX/fireball_small.wav", "fbig");
	LoadSFX("Assets/SFX/fireball_big.wav", "fsmall");
	LoadSFX("Assets/SFX/waterfall.wav", "firewaterfall");
	LoadSFX("Assets/SFX/shield2.wav", "fireshield");
	LoadSFX("Assets/SFX/rock_throw.wav", "rock_throw");
	LoadSFX("Assets/SFX/groundpound.wav", "gpstart");
	LoadSFX("Assets/SFX/shockwave.wav", "shockwave");
		
	LoadSFX("Assets/SFX/enemy_hit.wav", "enemy_hit");
	LoadSFX("Assets/SFX/ping.wav", "ping");
	LoadSFX("Assets/SFX/hit_floor.wav", "hit_floor");

	LoadSFX("Assets/SFX/menu_change.wav", "menu_change");
	LoadSFX("Assets/SFX/menu_choose2.wav", "menu_choose");

	return ret;
}

bool Audio::Loop(float dt)
{

	if(prev_sett_volume != settings_volume || prev_mus_volume != music_volume || prev_sfx_volume != sfx_volume)
	{

		RecalculateVolume();

		prev_sett_volume = settings_volume;
		prev_mus_volume = music_volume;
		prev_sfx_volume = sfx_volume;
	}

	return true;
}

bool Audio::CleanUp()
{
	for (std::vector<SFX*>::iterator it = sfx_list.begin(); it != sfx_list.end(); it++)
	{
		Mix_FreeChunk((*it)->sfx);
		delete(*it);
	}
	sfx_list.clear();

	for (std::vector<Music*>::iterator it = music_list.begin(); it != music_list.end(); it++)
	{
		Mix_FreeMusic((*it)->music);
		delete(*it);
	}
	music_list.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

uint Audio::LoadMusic(const char * file,const char*name, float fade,float volume)
{
	Music* new_music = new Music();
	new_music->id = music_list.size();
	new_music->name = name;
	new_music->fade = fade;
	new_music->volume = volume;

	new_music->music = Mix_LoadMUS(file);
	if (new_music->music == NULL)
	{
		App->con->Console_log("Can't load music");
		delete new_music;
		return -1;
	}
	else
	{
		music_list.push_back(new_music);
		return new_music->id;
	}

}

uint Audio::LoadSFX(const char * file, const char*name, float volume)
{
	SFX* new_sfx = new SFX();
	new_sfx->id = sfx_list.size();
	new_sfx->volume = volume;
	new_sfx->name = name;

	new_sfx->sfx = Mix_LoadWAV(file);
	if (new_sfx->sfx == NULL)
	{
		App->con->Console_log("Can't load sfx");
		delete new_sfx;
		return -1;
	}
	else
	{
		sfx_list.push_back(new_sfx);
		return new_sfx->id;
	}
}

void Audio::PlayMusic(uint music_id, float fade_in_ms)
{
	Music* music_selected = music_list[music_id];
	if (music_id != current_song)
	{
		current_song = music_id;

		if (Mix_PlayingMusic() == 1)
		{
			//fade out music
			Mix_FadeOutMusic(current_fade_out);
			next_song_after_fade_out = music_selected->music;
			next_song_after_fade_out_fade_time = music_selected->fade;
			Mix_HookMusicFinished(StartNextSongAfterFadeOut);
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

uint Audio::PlaySFX(uint sfx_id, int repeat, uint channel)
{
	SFX* sfx_selected = sfx_list[sfx_id];
	
	int chan = channel;
	if (channel == -1)
	{
		chan = GetFirstFreeChannel();
	}
	//Mix_Resume(chan);

	int i = Mix_PlayChannel(-1, sfx_list[sfx_id]->sfx, repeat);

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

void StartNextSongAfterFadeOut()
{
	Mix_FadeInMusic(App->aud->next_song_after_fade_out, -1, App->aud->next_song_after_fade_out_fade_time);

}
