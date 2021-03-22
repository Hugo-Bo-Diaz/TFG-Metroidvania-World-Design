#ifndef TEXTURES__H
#define TEXTURES__H

#include "PartsDef.h"
#include <vector>
#include <map>

#include "SDL_image/include/SDL_image.h"


struct SDL_Texture;
struct SDL_Surface;

struct Texture
{
	int id;
	std::string name;
	SDL_Texture* texture;
	bool operator==(const int& t)
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

class Textures : public Part
{
public:
	Textures();
	bool Init();

	void Load_Texture(const char* path,const char* name,pugi::xml_node& node = pugi::xml_node::xml_node());
	SDL_Texture* Load_Texture_Scene(const char* path);
	bool Valid_Texture(const char* texture_name);
	void Destroy_Texture(const char* texture_name);

	SDL_Texture* Get_Texture(const char* name);

	bool CleanUp();

	bool LoadConfig(pugi::xml_node&);
	bool CreateConfig(pugi::xml_node&);

	int number_of_textures = 0;

private:
	std::vector<Texture*> texture_list;
};
#endif //TEXTURES__H