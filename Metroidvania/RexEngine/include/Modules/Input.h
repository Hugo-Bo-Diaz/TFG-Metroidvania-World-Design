#ifndef INPUT__H
#define INPUT__H

#include "PartsDef.h"
#include "Part.h"
#include "SDL\include\SDL.h"
#include <vector>

#define MAX_KEYS 300
//xbox style

enum JoyStickDirection
{
	J_PLUS,
	J_MINUS,
	J_NONE
};

struct joystick
{
	float x_axis;
	float y_axis;

	JoyStickDirection h_dir;
	JoyStickDirection v_dir;
};


enum ButtonState
{
	BUTTON_IDLE,
	BUTTON_DOWN,
	BUTTON_REPEAT,
	BUTTON_RELEASE
};

struct trigger
{
	bool is_down;
	float axis;
	bool is_pressed;
	bool is_released;
};

enum Keystate
{
	KEY_IDLE,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_RELEASE
};

enum Gameplay_buttons {
	BUTTON_1,
	BUTTON_2,
	BUTTON_3,
	BUTTON_4,

	START,
	SELECT,

	R_SHOULDER,
	L_SHOULDER,

	R_TRIGGER,
	L_TRIGGER,


	DPAD_LEFT,
	DPAD_RIGHT,
	DPAD_UP,
	DPAD_DOWN,

	LEFT,
	RIGHT,
	UP,
	DOWN,

	NUMBER_OF_BUTTONS
};

class DLL_EXPORT setup
{
public:

	uint inputs[18];
	bool keyboard;

};

class DLL_EXPORT Input : public Part
{
public:
	Input(EngineAPI& aAPI);
	~Input() {};
	bool Init();
	bool Loop(float dt);
	bool CleanUp();

	Keystate GetKey(int id) const { return keyboard[id]; }
	
	Keystate GetButton(int id) { return controller[id]; };

	Keystate GetInput(Gameplay_buttons id);

	void GetJoystick(bool left, float& x, float& y);

	float GetTrigger(bool left);

	bool GetTriggerPressed(bool left);
	bool GetTriggerDown(bool left);
	bool GetTriggerReleased(bool left);

	//0 horizontal 1 vertical

	bool using_controller=false;

	int current_setup = 1;
	std::vector<setup*> controller_setups;

	bool LoadConfig(pugi::xml_node&);
	bool CreateConfig(pugi::xml_node&);

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
};

#endif // !INPUT__H
