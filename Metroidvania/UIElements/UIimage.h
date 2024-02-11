#ifndef UI__IMAGE
#define UI__IMAGE

#include "UIelement.h"
#include <string>


class UIimage : public UIelement
{
public:

	UIimage(float x, float y, float w, float h, float depth, const char* texture);

	std::string texture_name;

	void Loop();
	void Render();


};

#endif // !UI__IMAGE
