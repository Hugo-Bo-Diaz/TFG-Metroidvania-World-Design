#include "FirstDialogue.h"
#include "../UIElements/UItextbox.h"
#include "Application.h"
#include "Gui.h"
#include "../SceneProcessing.h"
#include "Camera.h"

FirstDialogue::FirstDialogue()
{
	texture = App->tex->Load_Texture("Assets/UI/black_square.png");

	textbox = new UItextbox("", "For those who seek power are blind to their surroundings", TextBoxColor::GREY, 15, 4, 272, 160, 2, 0.2);
	textbox->AddPanelToTextBox("only when it is able to serve all will it be balanced");
	textbox->AddPanelToTextBox("Never forget though never stop moving forward seeking to repare one's mistakes");
	App->gui->AddElement(textbox);
}

bool FirstDialogue::Loop(float dt)
{
	if (!App->gui->ElementExists(textbox))
	{
		App->phy->DeleteObject(this);
		MetroidVaniaSceneProcessor::GetInstance().canopenmenu = true;
	}

	return true;
}

bool FirstDialogue::Render()
{
	App->ren->Blit(texture, 0,0, nullptr,10, RenderQueue::RENDER_UI);

	return true;
}