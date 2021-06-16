#include "TextBoxObject.h"
#include "Input.h"
#include "UItextbox.h"
#include "ProgressTracker.h"
#include "Player.h"

TextBoxObject::TextBoxObject()
{
	rectanglekeyboard = {0,0,48,48};
	rectanglecontroller = { 48,0,48,48 };

}

bool TextBoxObject::Loop(float dt)
{
	bool iscontactingplayer = false;

	std::vector<collision*> collisions;
	App->phy->GetCollisions(collider, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->type == PLAYER)
			{
				iscontactingplayer = true;
				if (App->inp->GetInput(BUTTON_2) == BUTTON_DOWN && strings.size()>0)
				{
					textbox = (UItextbox*)App->gui->AddTextBox(author.c_str(), strings[0].c_str(), c, 15, 4, 272, 420, 2, 0.2);
					int max_text = strings.size();
					if (max_text > 1)
					{
						for (int i = 1; i < max_text; ++i)
						{
							textbox->AddPanelToTextBox(strings[i].c_str());
						}
					}
					textboxexists = true;

					if (lore_unlock != -1 && !App->trk->HasLogBeenUnlocked(lore_unlock))
					{
						App->trk->UnlockLog(lore_unlock);
						textbox->AddPanelToTextBox("New lore entry unlocked");
					}
				}

			}
		}
	}
	
	player_contact = iscontactingplayer;

	if (textboxexists && !App->gui->ElementExists(textbox))
	{
		textboxexists = false;
		App->trk->pl->speed_y = 0;
	}

	return true;
}

bool TextBoxObject::Render()
{
	if (player_contact == true)
	{
		if(App->inp->using_controller)
			App->ren->Blit(App->tex->Get_Texture("indicator"), collider->x + collider->w/2 - 24, collider->y - 48, &rectanglecontroller,-5);
		else
			App->ren->Blit(App->tex->Get_Texture("indicator"), collider->x + collider->w / 2 - 24, collider->y - 48, &rectanglekeyboard, -5);
	}

	return true;
}


TextBoxObject::~TextBoxObject()
{
}

void TextBoxObject::AddText(const char * text)
{
	strings.push_back(text);
}
