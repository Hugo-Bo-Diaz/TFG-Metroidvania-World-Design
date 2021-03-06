#ifndef MAINMENUSCENE__H
#define MAINMENUSCENE__H

#include "Scene.h"

enum MainMenuOption
{
	MIN_OPTION,
	SERVER_OPTION,
	CLIENT_OPTION,
	MAX_OPTION
};
class SceneMainMenu : public Scene
{
public:
	bool Create();
	bool Start();
	bool Loop();
	bool Stop();
	bool Destroy();
private:

	MainMenuOption current = SERVER_OPTION;
	SDL_Texture* background;
	SDL_Texture* tex_client_button;
	SDL_Texture* tex_server_button;
	SDL_Rect* option_chosen = new SDL_Rect({0,116,272,116});
	SDL_Rect* option_not_chosen = new SDL_Rect({ 0,0,272,116 });
};

#endif //MAINMENUSCENE__H
