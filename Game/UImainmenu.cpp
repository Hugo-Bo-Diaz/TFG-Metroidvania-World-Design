#include "UImainmenu.h"
#include "Application.h"
#include "Input.h"
#include "ProgressTracker.h"
#include "Gui.h"
UIMainMenu::UIMainMenu()
{
}

void UIMainMenu::Loop()
{

	if (App->inp->GetKey(SDL_SCANCODE_Q) == BUTTON_DOWN)
	{
		App->trk->start_new_game = true;
		App->gui->RemoveElement(this);
	}

}

void UIMainMenu::Render()
{
}
