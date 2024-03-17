#ifndef INPUT_IMPL__H
#define INPUT_IMPL__H

#include "../include/Modules/Input.h"
#include "PartImpl.h"
#include "SDL\include\SDL.h"

class Input::InputImpl : public Part::Part_Impl
{
public:
	Input::InputImpl(Input* aInput) :mPartInst(aInput){};

	void SetInputInst(Input* aInput)
	{
		mPartInst = aInput;
	}

protected:
	bool Init();
	bool Loop(float dt);
	bool CleanUp();

	bool LoadConfig(pugi::xml_node& config_node);
	bool CreateConfig(pugi::xml_node& config_node);

private:
	Keystate* keyboard;

	Keystate* controller;
	joystick* left_joystick;
	joystick* right_joystick;

	trigger* left_trigger;
	trigger* right_trigger;

	SDL_GameController* SDLcontroller = nullptr;
	int controller_id = 0;
	bool controller_active = false;

	friend Input;
	Input* mPartInst;
	std::vector<setup*> controller_setups;
	int current_setup = 1;
	bool using_controller = false;
};
#endif