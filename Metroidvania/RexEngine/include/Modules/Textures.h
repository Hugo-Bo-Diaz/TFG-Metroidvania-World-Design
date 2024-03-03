#ifndef TEXTURES__H
#define TEXTURES__H

#include "PartsDef.h"
#include <vector>
#include <map>
#include <list>
#include "Part.h"
#include "SDL_image/include/SDL_image.h"

typedef unsigned int TextureID;

struct SDL_Texture;
struct SDL_Surface;

struct DLL_EXPORT Texture
{
	TextureID id;
	std::string name;
	SDL_Texture* texture;
	bool operator==(const TextureID& t)
	{
		if(id == t)
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

class DLL_EXPORT Textures : public Part
{
public:
	Textures(EngineAPI& aAPI);
	bool Init();

	TextureID Load_Texture(const char* path);
	void Destroy_Texture(const char* texture_name);

	SDL_Texture* Get_Texture(TextureID name);

	bool CleanUp();

	bool LoadConfig(pugi::xml_node&);
	bool CreateConfig(pugi::xml_node&);

	int number_of_textures = 0;

private:
	std::vector<Texture*> texture_list;
};
#endif //TEXTURES__H