#ifndef INPUT__H
#define INPUT__H

#include "PartsDef.h"
#include "Part.h"

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
//modules that handles the input
class DLL_EXPORT Input : public Part
{
public:
	Input(EngineAPI& aAPI);
	~Input() {};

	//returns the state of the input
	Keystate GetKey(int id);
	//returns the state of the input
	Keystate GetButton(int id);
	//returns the state of the input
	Keystate GetInput(Gameplay_buttons id);

	//returns the state of the input, it recieves a boolean, true if the value is desired from the left joystick
	void GetJoystick(bool left, float& x, float& y);
	//returns the state of the input, it recieves a boolean, true if the value is desired from the left trigger
	float GetTrigger(bool left);

	//returns true if the trigger was pressed, it recieves a boolean, true if the value is desired from the left trigger
	bool GetTriggerPressed(bool left);
	//returns true if the trigger was pressed, it recieves a boolean, true if the value is desired from the left trigger
	bool GetTriggerDown(bool left);
	//returns true if the trigger was pressed, it recieves a boolean, true if the value is desired from the left trigger
	bool GetTriggerReleased(bool left);

	//returns true if the input module is using a controller
	bool IsUsingController();

	//0 horizontal 1 vertical
	void AddControllerSetup(setup*);
	void RemoveControllerSetup(setup*);

private:

	class InputImpl;
};

#endif // !INPUT__H
