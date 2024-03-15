#ifndef TEXTURES_IMPL__H
#define TEXTURES_IMPL__H

#include "PartImpl.h"
#include "../include/Modules/Textures.h"
#include "SDL_image/include/SDL_image.h"

struct SDL_Texture;
struct SDL_Surface;

struct Texture
{
	TextureID id;
	std::string name;
	SDL_Texture* texture;
	bool operator==(const TextureID& t)
	{
		if (id == t)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator==(const char* t)
	{
		if (name == t)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	~Texture()
	{
		SDL_DestroyTexture(texture);
	}
};


class Textures::TexturesImpl : public Part::Part_Impl
{
public:

	Textures::TexturesImpl(Textures* aTextures) :mPartInst(aTextures) {};

	void SetTexturesInst(Textures* aTextures)
	{
		mPartInst = aTextures;
	}
	SDL_Texture* Get_Texture(TextureID name);
	TextureID AddTexture(SDL_Texture* aTextureToAdd,const char* aTextureName);
protected:
	bool LoadConfig(pugi::xml_node& config_node);
	bool CreateConfig(pugi::xml_node& config_node);
	bool CleanUp();

private:
	int number_of_textures = 0;
	std::vector<Texture*> texture_list;

	friend class Textures;
	Textures* mPartInst;
};

#endif // !AUDIO_IMPL__H