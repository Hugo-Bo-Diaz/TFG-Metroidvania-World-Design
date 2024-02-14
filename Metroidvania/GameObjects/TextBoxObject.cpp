#include "TextBoxObject.h"
#include "Input.h"
#include "../UIElements/UItextbox.h"
#include "ProgressTracker.h"
#include "Player.h"
#include "EntityIDs.h"
#include "../UIelementFunctions.h"
#include "../SceneProcessing.h"

TextBoxObject::TextBoxObject()
{
	rectanglekeyboard = {0,0,48,48};
	rectanglecontroller = { 48,0,48,48 };
	if (App->trk->BaseSaveSection->GetChild("LoreLogs") == nullptr)
	{
		App->trk->BaseSaveSection->AddNewChild("LoreLogs");
	}
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
			if ((*it)->type == PLAYER_ID)
			{
				iscontactingplayer = true;
				if (App->inp->GetInput(BUTTON_2) == BUTTON_DOWN && strings.size()>0)
				{
					textbox = new UItextbox(author.c_str(), strings[0].c_str(), c, 15, 4, 272, 420, 2, 0.2);
					App->gui->AddElement(textbox);

					int max_text = strings.size();
					if (max_text > 1)
					{
						for (int i = 1; i < max_text; ++i)
						{
							textbox->AddPanelToTextBox(strings[i].c_str());
						}
					}
					textboxexists = true;

					if (lore_unlock != -1 && App->trk->BaseSaveSection->GetChild("LoreLogs")->GetValue(std::to_string(lore_unlock).c_str()) == 0.0f)
					{
						App->trk->BaseSaveSection->GetChild("LoreLogs")->SetValue(std::to_string(lore_unlock).c_str(), 1.0f);
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
		MetroidVaniaSceneProcessor::GetInstance().pl->speed_y = 0;

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