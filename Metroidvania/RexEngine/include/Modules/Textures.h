#ifndef TEXTURES__H
#define TEXTURES__H

#include "PartsDef.h"
#include "Part.h"

typedef unsigned int TextureID;

//module that manages the textures
class DLL_EXPORT Textures : public Part
{
public:
	Textures(EngineAPI& aAPI);
	
	//loads a texture and returns its id for future use
	TextureID Load_Texture(const char* path);
	//destroys a texture recieving a path
	void Destroy_Texture(const char* texture_name);
	//destroys a texture recieving its ID
	void Destroy_Texture(TextureID aTextureID);

	class TexturesImpl;
};
#endif //TEXTURES__H