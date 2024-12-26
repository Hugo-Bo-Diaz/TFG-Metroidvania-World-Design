#ifndef UI__IMAGE
#define UI__IMAGE

#include "EngineElements/ParticleEmitter.h"
#include <string>
#include "Modules/Textures.h"
#include "EngineElements/UIelement.h"

class UIimage : public UIelement
{
public:

	UIimage(float x, float y, float w, float h, float depth, const char* texture);

	void Init();
	void Loop();
	void Render();

	std::string mPath;
	TextureID Tex;
};

#endif // !UI__IMAGE
