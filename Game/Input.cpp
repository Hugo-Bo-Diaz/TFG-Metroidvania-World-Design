#include "Input.h"

Input::Input()
{
	name = "Input";
}

bool Input::Init()
{
	bool ret = true;
	keyboard = new Keystate[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(Keystate) * MAX_KEYS);

	controller = new Keystate[NUMBER_OF_BUTTONS];
	memset(controller, BUTTON_IDLE, sizeof(Keystate) * NUMBER_OF_BUTTONS);

	left_joystick = new joystick();
	right_joystick = new joystick();

	left_trigger = new trigger();
	right_trigger = new trigger();

	printf("Init SDL input event system\n");
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		printf("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	SDL_StopTextInput();

	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
	{
		printf("Game controller could not initiate %s\n", SDL_GetError());
		ret = false;
	}

	SDLcontroller = SDL_GameControllerOpen(controller_id);

	if (SDLcontroller)
	{
		controller_active = true;
	}

	return ret;

}
bool Input::Loop(float dt)
{
	bool ret = true;

	static SDL_Event event;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			current_setup = 0;

			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_RELEASE;
			else
				keyboard[i] = KEY_IDLE;
		}
	}
	//controller stuff
	if (controller_active)
	{

		Uint8 buttons[SDL_CONTROLLER_BUTTON_MAX];
		for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
			buttons[i] = SDL_GameControllerGetButton(SDLcontroller, (SDL_GameControllerButton)i);
		}

		for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
		{
			if (buttons[i] == 1)
			{
				current_setup = 1;
				if (controller[i] == KEY_IDLE)
					controller[i] = KEY_DOWN;
				else
					controller[i] = KEY_REPEAT;
			}
			else
			{
				if (controller[i] == KEY_REPEAT || controller[i] == KEY_DOWN)
					controller[i] = KEY_RELEASE;
				else
					controller[i] = KEY_IDLE;
			}
		}

		float leftxpast = left_joystick->x_axis;
		float leftypast = left_joystick->y_axis;
		float rightxpast = right_joystick->x_axis;
		float rightypast = right_joystick->y_axis;

		left_joystick->x_axis = ((float)SDL_GameControllerGetAxis(SDLcontroller, SDL_CONTROLLER_AXIS_LEFTX) / 32767.0f);
		left_joystick->y_axis = ((float)SDL_GameControllerGetAxis(SDLcontroller, SDL_CONTROLLER_AXIS_LEFTY) / 32767.0f);

		right_joystick->x_axis = ((float)SDL_GameControllerGetAxis(SDLcontroller, SDL_CONTROLLER_AXIS_RIGHTX) / 32767.0f);
		right_joystick->y_axis = ((float)SDL_GameControllerGetAxis(SDLcontroller, SDL_CONTROLLER_AXIS_RIGHTY) / 32767.0f);


		//LEFT JOYSTICK
		//X
		if (abs( left_joystick->x_axis) < 0.3)
			left_joystick->h_dir = J_NONE;
		else 
		{
			if (left_joystick->x_axis > 0)
				left_joystick->h_dir = J_PLUS;
			else
				left_joystick->h_dir = J_MINUS;
		}
		//Y
		if (abs(left_joystick->y_axis) < 0.3)
			left_joystick->v_dir = J_NONE;
		else
		{
			if (left_joystick->y_axis > 0)
				left_joystick->v_dir = J_PLUS;
			else
				left_joystick->v_dir = J_MINUS;
		}



		//RIGHT JOYSTICK
		//X
		if (abs(right_joystick->x_axis) < 0.3)
			right_joystick->h_dir = J_NONE;
		else
		{
			if (right_joystick->x_axis > 0)
				right_joystick->h_dir = J_PLUS;
			else
				right_joystick->h_dir = J_MINUS;
		}
		//Y
		if (abs(right_joystick->y_axis) < 0.3)
			right_joystick->v_dir = J_NONE;
		else
		{
			if (right_joystick->y_axis > 0)
				right_joystick->v_dir = J_PLUS;
			else
				right_joystick->v_dir = J_MINUS;
		}

		left_trigger->is_pressed = false;
		left_trigger->is_released = false;

		bool leftTpastpressed =  left_trigger->is_down;

		left_trigger->axis = ((float)SDL_GameControllerGetAxis(SDLcontroller, SDL_CONTROLLER_AXIS_TRIGGERLEFT) / 32767.0f);
		if (left_trigger->axis > 0.5)
		{
			left_trigger->is_down = true;
			if(!leftTpastpressed)
			{
				left_trigger->is_pressed = true;
			}
		}
		else
		{
			left_trigger->is_down = false;
			if(leftTpastpressed)
			{
				left_trigger->is_released = true;
			}
		}

		right_trigger->is_pressed = false;
		right_trigger->is_released = false;
		bool rightTpastpressed = right_trigger->is_down;

		right_trigger->axis = ((float)SDL_GameControllerGetAxis(SDLcontroller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) / 32767.0f);
		if (right_trigger->axis > 0.5)
		{
			right_trigger->is_down = true;
			if (!rightTpastpressed)
			{
				right_trigger->is_pressed = true;
			}
		}
		else
		{
			right_trigger->is_down = false;
			if (rightTpastpressed)
			{
				right_trigger->is_released = true;
			}
		}

		//printf("%f %f\n", left_joystick->x_axis, leftxpast);
		
	}
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			ret = false;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				break;
			}
			break;		
		case SDL_CONTROLLERDEVICEREMOVED:
		{
			if (controller_id == event.cdevice.which) {
				controller_id = -1;
				controller_active = false;
				SDLcontroller = nullptr;
				SDL_GameControllerClose(SDLcontroller);
			}
		}
			break;
		case SDL_CONTROLLERDEVICEADDED:
		{
			if (!controller_active)
			{
				controller_id = event.cdevice.which;
				SDLcontroller = SDL_GameControllerOpen(controller_id);
				controller_active = true;
			}
		}
		break;
		}

		
	}


	return ret;
}


void Input::GetJoystick(bool left, float& x, float& y)
{
	x = 0;
	y = 0;
	if(controller_setups[current_setup]->keyboard)
	{
		if (GetKey(controller_setups[current_setup]->inputs[LEFT]))
		{
			printf("left ");
			x -= 1;
		}
		if (GetKey(controller_setups[current_setup]->inputs[RIGHT]))
			x += 1;
		if (GetKey(controller_setups[current_setup]->inputs[DOWN]))
			y -= 1;
		if (GetKey(controller_setups[current_setup]->inputs[UP]))
			y += 1;
	}
	else
	{
		if (left)
		{
			x = left_joystick->x_axis;
			y = left_joystick->y_axis;
		}
		else
		{
			x = right_joystick->x_axis;
			y = right_joystick->y_axis;
		}
	}
};

float Input::GetTrigger(bool left)
{
	if (left)
	{
		return left_trigger->axis;
	}
	else
	{
		return right_trigger->axis;
	}
}
bool Input::GetTriggerPressed(bool left)
{
	if (left)
	{
		return left_trigger->is_pressed;
	}
	else
	{
		return right_trigger->is_pressed;
	}
}

bool Input::GetTriggerDown(bool left)
{
	if (left)
	{
		return left_trigger->is_down;
	}
	else
	{
		return right_trigger->is_down;
	}
}

bool Input::GetTriggerReleased(bool left)
{
	if (left)
	{
		return left_trigger->is_released;
	}
	else
	{
		return right_trigger->is_released;
	}
}

Keystate Input::GetInput(Gameplay_buttons id)
{

	setup* current = controller_setups[current_setup];
	

	if (current->keyboard)
	{
		return GetKey(current->inputs[id]);
	}
	else
	{
		return GetButton(current->inputs[id]);
	}

}



bool Input::LoadConfig(pugi::xml_node& config_node)
{
	pugi::xml_node controller_sets = config_node.child("controllers");
	pugi::xml_node set;
	for (set = controller_sets.first_child(); set; set = set.next_sibling())
	{
		setup* new_setup = new setup();
		new_setup->inputs[0] = set.child("b1").attribute("value").as_int();
		new_setup->inputs[1] = set.child("b2").attribute("value").as_int();
		new_setup->inputs[2] = set.child("b3").attribute("value").as_int();
		new_setup->inputs[3] = set.child("b4").attribute("value").as_int();

		new_setup->inputs[4] = set.child("start").attribute("value").as_int();
		new_setup->inputs[5] = set.child("select").attribute("value").as_int();

		new_setup->inputs[6] = set.child("sh_r").attribute("value").as_int();
		new_setup->inputs[7] = set.child("sh_l").attribute("value").as_int();

		new_setup->inputs[10] = set.child("dpad_l").attribute("value").as_int();
		new_setup->inputs[11] = set.child("dpad_r").attribute("value").as_int();
		new_setup->inputs[13] = set.child("dpad_d").attribute("value").as_int();
		new_setup->inputs[12] = set.child("dpad_u").attribute("value").as_int();

		new_setup->keyboard = set.child("keyboard").attribute("value").as_bool();

		if (new_setup->keyboard)
		{
			new_setup->inputs[14] = set.child("left").attribute("value").as_int();
			new_setup->inputs[15] = set.child("right").attribute("value").as_int();
			new_setup->inputs[17] = set.child("down").attribute("value").as_int();
			new_setup->inputs[16] = set.child("up").attribute("value").as_int();
		}
		controller_setups.push_back(new_setup);
	}

	return true;
}

bool Input::CreateConfig(pugi::xml_node& config_node)
{
	pugi::xml_node controller_sets = config_node.append_child("controllers");

	pugi::xml_node set_key = controller_sets.append_child("set");

		setup* keyboard_setup = new setup();
		keyboard_setup->inputs[0] = SDL_SCANCODE_Z; set_key.append_child("b1").append_attribute("value") = SDL_SCANCODE_Z;
		keyboard_setup->inputs[1] = SDL_SCANCODE_X; set_key.append_child("b2").append_attribute("value") = SDL_SCANCODE_X;
		keyboard_setup->inputs[2] = SDL_SCANCODE_C; set_key.append_child("b3").append_attribute("value") = SDL_SCANCODE_C;
		keyboard_setup->inputs[3] = SDL_SCANCODE_V; set_key.append_child("b4").append_attribute("value") = SDL_SCANCODE_V;

		keyboard_setup->inputs[4] = SDL_SCANCODE_KP_ENTER; set_key.append_child("start").append_attribute("value") = SDL_SCANCODE_KP_ENTER;
		keyboard_setup->inputs[5] = SDL_SCANCODE_BACKSPACE; set_key.append_child("select").append_attribute("value") = SDL_SCANCODE_BACKSPACE;

		keyboard_setup->inputs[6] = SDL_SCANCODE_A; set_key.append_child("sh_r").append_attribute("value") = SDL_SCANCODE_A;
		keyboard_setup->inputs[7] = SDL_SCANCODE_S; set_key.append_child("sh_r").append_attribute("value") = SDL_SCANCODE_S;

		keyboard_setup->inputs[10] = SDL_SCANCODE_O; set_key.append_child("dpad_l").append_attribute("value") = SDL_SCANCODE_O;
		keyboard_setup->inputs[11] = SDL_SCANCODE_P; set_key.append_child("dpad_r").append_attribute("value") = SDL_SCANCODE_P;
		keyboard_setup->inputs[13] = SDL_SCANCODE_K; set_key.append_child("dpad_d").append_attribute("value") = SDL_SCANCODE_K;
		keyboard_setup->inputs[12] = SDL_SCANCODE_J; set_key.append_child("dpad_u").append_attribute("value") = SDL_SCANCODE_J;

		keyboard_setup->keyboard = true; set_key.append_child("keyboard").append_attribute("value") = true;

		if (keyboard_setup->keyboard)
		{
			keyboard_setup->inputs[14] = SDL_SCANCODE_LEFT; set_key.append_child("left").append_attribute("value") = SDL_SCANCODE_LEFT;
			keyboard_setup->inputs[15] = SDL_SCANCODE_RIGHT; set_key.append_child("right").append_attribute("value") = SDL_SCANCODE_RIGHT;
			keyboard_setup->inputs[17] = SDL_SCANCODE_DOWN; set_key.append_child("down").append_attribute("value") = SDL_SCANCODE_DOWN;
			keyboard_setup->inputs[16] = SDL_SCANCODE_UP; set_key.append_child("up").append_attribute("value") = SDL_SCANCODE_UP;
		}

		controller_setups.push_back(keyboard_setup);


		pugi::xml_node set_con = controller_sets.append_child("set");

		setup* controller_setup = new setup();

		controller_setup->inputs[0] = SDL_CONTROLLER_BUTTON_A; set_con.append_child("b1").append_attribute("value") = SDL_CONTROLLER_BUTTON_A;
		controller_setup->inputs[1] = SDL_CONTROLLER_BUTTON_B; set_con.append_child("b2").append_attribute("value") = SDL_CONTROLLER_BUTTON_B;
		controller_setup->inputs[2] = SDL_CONTROLLER_BUTTON_Y; set_con.append_child("b3").append_attribute("value") = SDL_CONTROLLER_BUTTON_Y;
		controller_setup->inputs[3] = SDL_CONTROLLER_BUTTON_X; set_con.append_child("b4").append_attribute("value") = SDL_CONTROLLER_BUTTON_X;

		controller_setup->inputs[4] = SDL_CONTROLLER_BUTTON_START; set_con.append_child("start").append_attribute("value") = SDL_CONTROLLER_BUTTON_START;
		controller_setup->inputs[5] = SDL_CONTROLLER_BUTTON_BACK; set_con.append_child("select").append_attribute("value") = SDL_CONTROLLER_BUTTON_BACK;

		controller_setup->inputs[6] = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER; set_con.append_child("sh_r").append_attribute("value") = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
		controller_setup->inputs[7] = SDL_CONTROLLER_BUTTON_LEFTSHOULDER; set_con.append_child("sh_l").append_attribute("value") = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;

		controller_setup->inputs[10] = SDL_CONTROLLER_BUTTON_DPAD_LEFT; set_con.append_child("dpad_l").append_attribute("value") = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
		controller_setup->inputs[11] = SDL_CONTROLLER_BUTTON_DPAD_RIGHT; set_con.append_child("dpad_r").append_attribute("value") = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
		controller_setup->inputs[13] = SDL_CONTROLLER_BUTTON_DPAD_DOWN; set_con.append_child("dpad_d").append_attribute("value") = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
		controller_setup->inputs[12] = SDL_CONTROLLER_BUTTON_DPAD_UP; set_con.append_child("dpad_u").append_attribute("value") = SDL_CONTROLLER_BUTTON_DPAD_UP;

		controller_setup->keyboard = false; set_con.append_child("keyboard").append_attribute("value") = false;
		controller_setups.push_back(controller_setup);

		return true;
	
}

bool Input::CleanUp()
{
	bool ret = true;
	delete[] keyboard;
	printf("Quitting Input\n");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
	if (controller_active)
	{
		SDL_GameControllerClose(SDLcontroller);
	}
	active = false;

	return ret;
}