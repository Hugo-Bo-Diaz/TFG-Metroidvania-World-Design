#include "FirstDialogue.h"
#include "Gui.h"
#include "UItextbox.h"
#include "Application.h"
#include "Camera.h"

FirstDialogue::FirstDialogue()
{
	t =  (UItextbox*)App->gui->AddTextBox("", "For those who seek power are blind to their surroundings", TextBoxColor::GREY, 15, 4, 272, 160, 2, 0.2);
		t->AddPanelToTextBox("only when it is able to serve all will it be balanced");
		
	t->AddPanelToTextBox("Never forget though never stop moving forward seeking to repare one's mistakes");
}

bool FirstDialogue::Loop(float dt)
{
	if (!App->gui->ElementExists(t))
	{
		App->phy->DeleteObject(this);
	}

	return true;
}

bool FirstDialogue::Render()
{
	App->ren->BlitUI(App->tex->Get_Texture("black_square"), 0,0, nullptr,10);

	return true;
}