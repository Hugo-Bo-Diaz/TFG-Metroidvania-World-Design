#ifndef UI_SPELL_DISPLAY
#define UI_SPELL_DISPLAY

#include "EngineElements/ParticleEmitter.h"
#include <vector>
#include "Modules/Textures.h"
#include "EngineElements/UIelement.h"

class Player;
class EngineAPI;
class UIspelldisplay : public UIelement
{
public:

	//texture with bases = spell_display_base
	//texture with books = spell_books

	UIspelldisplay();

	SDL_Rect left_border;
	SDL_Rect right_border;
	SDL_Rect left_cover;
	SDL_Rect right_cover;
	SDL_Rect square;
	std::vector<SDL_Rect> books;
	
	Player* player;
	
	int current_spell;

	TextureID lTexture;
	TextureID lBooksTex;

	void Init();
	void Loop();
	void Render();
	void RenderDebug();
};
#endif // !UI_HEALTHBAR
