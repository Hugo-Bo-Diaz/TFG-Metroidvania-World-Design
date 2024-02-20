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
	texture = App->tex->Load_Texture("Assets/UI/indicator.png");

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
			App->ren->Blit(texture, collider->x + collider->w/2 - 24, collider->y - 48, &rectanglecontroller,-5);
		else
			App->ren->Blit(texture, collider->x + collider->w / 2 - 24, collider->y - 48, &rectanglekeyboard, -5);
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

GameObject* TextBoxObject::Factory(std::list<ObjectProperty*>& aProperties)
{
	TextBoxObject* textbox = new TextBoxObject();

	for (std::list<ObjectProperty*>::iterator it = aProperties.begin(); it != aProperties.end(); ++it)
	{
		if ((*it)->name.compare("text") == 0)
		{
			textbox->AddText((*it)->str_value.c_str());
		}
		else if ((*it)->name.compare("author") == 0)
		{
			textbox->author = (*it)->str_value;
		}
		else if ((*it)->name.compare("lore") == 0)
		{
			textbox->lore_unlock = (*it)->num_value;
		}
	}

	return textbox;
}
