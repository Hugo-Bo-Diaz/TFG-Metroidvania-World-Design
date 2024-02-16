#ifndef UI__IMAGE
#define UI__IMAGE

#include "UIelement.h"
#include <string>
#include "Textures.h"

class UIimage : public UIelement
{
public:

	UIimage(float x, float y, float w, float h, float depth, const char* texture);

	void Loop();
	void Render();

	TextureID Tex;
};

#endif // !UI__IMAGE
