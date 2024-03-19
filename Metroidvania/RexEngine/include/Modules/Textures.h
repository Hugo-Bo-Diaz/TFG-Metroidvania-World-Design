#ifndef TEXTURES__H
#define TEXTURES__H

#include "PartsDef.h"
#include "Part.h"


typedef unsigned int TextureID;

class DLL_EXPORT Textures : public Part
{
public:
	Textures(EngineAPI& aAPI);

	TextureID Load_Texture(const char* path);
	void Destroy_Texture(const char* texture_name);

	class TexturesImpl;
};
#endif //TEXTURES__H