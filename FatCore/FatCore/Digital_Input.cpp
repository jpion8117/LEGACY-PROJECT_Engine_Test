/***************************************************************************************************
FatBoy Engine -- input

Purpose: this will define all objects needed for basic input and output operations, inputs will
be handled by a central input system that maps inputs to results. Inputs are devided into two
types digital or analog
***************************************************************************************************/

#include "universal_includes.h"
#include "errors.h"
#include "debug.h"
#include "core.h"
#include "input.h"

fatcore::digital_input::digital_input(const int INPUT):
INPUT_CODE(INPUT),
status(false),
uniqueID(0)
{
	//generate a unique id for the newly registered input, conflicts will be handled by the input manager,
	//and new ids will be assigned in the event of a conflict
	generateID();
}
void fatcore::digital_input::poll()
{
	status = false; //reset the input status
	int result = 0; //error code variable

#ifdef _WIN32
	if (INPUT_CODE > fbc_input::DIRECT_INPUT_DIGITAL_START && INPUT_CODE < fbc_input::XINPUT_ANALOG_END)
	{
		result = pollWin32();
	}
#endif

	//if  unsucessful, determine error code
	if (result != fbc_err::OK)
	{
		//break to determine failure code
	}
}

#ifdef _WIN32
int fatcore::digital_input::pollWin32()
{
	fatcore::input inputDev;

	//match input code to input
	switch (INPUT_CODE)
	{
		/********************************************
				  WINDOWS DIRECT INPUT
				  *********************************************/
	case fbc_input::A:
		if (inputDev.getKeyboardStateWin32()[DIK_A] & 0x80) status = true;
		break;
	case fbc_input::B:
		if (inputDev.getKeyboardStateWin32()[DIK_B] & 0x80) status = true;
		break;
	case fbc_input::C:
		if (inputDev.getKeyboardStateWin32()[DIK_C] & 0x80) status = true;
		break;
	case fbc_input::D:
		if (inputDev.getKeyboardStateWin32()[DIK_D] & 0x80) status = true;
		break;
	case fbc_input::E:
		if (inputDev.getKeyboardStateWin32()[DIK_E] & 0x80) status = true;
		break;
	case fbc_input::F:
		if (inputDev.getKeyboardStateWin32()[DIK_F] & 0x80) status = true;
		break;
	case fbc_input::G:
		if (inputDev.getKeyboardStateWin32()[DIK_G] & 0x80) status = true;
		break;
	case fbc_input::H:
		if (inputDev.getKeyboardStateWin32()[DIK_H] & 0x80) status = true;
		break;
	case fbc_input::I:
		if (inputDev.getKeyboardStateWin32()[DIK_I] & 0x80) status = true;
		break;
	case fbc_input::J:
		if (inputDev.getKeyboardStateWin32()[DIK_J] & 0x80) status = true;
		break;
	case fbc_input::K:
		if (inputDev.getKeyboardStateWin32()[DIK_K] & 0x80) status = true;
		break;
	case fbc_input::L:
		if (inputDev.getKeyboardStateWin32()[DIK_L] & 0x80) status = true;
		break;
	case fbc_input::M:
		if (inputDev.getKeyboardStateWin32()[DIK_M] & 0x80) status = true;
		break;
	case fbc_input::N:
		if (inputDev.getKeyboardStateWin32()[DIK_N] & 0x80) status = true;
		break;
	case fbc_input::O:
		if (inputDev.getKeyboardStateWin32()[DIK_O] & 0x80) status = true;
		break;
	case fbc_input::P:
		if (inputDev.getKeyboardStateWin32()[DIK_P] & 0x80) status = true;
		break;
	case fbc_input::Q:
		if (inputDev.getKeyboardStateWin32()[DIK_Q] & 0x80) status = true;
		break;
	case fbc_input::R:
		if (inputDev.getKeyboardStateWin32()[DIK_R] & 0x80) status = true;
		break;
	case fbc_input::S:
		if (inputDev.getKeyboardStateWin32()[DIK_S] & 0x80) status = true;
		break;
	case fbc_input::T:
		if (inputDev.getKeyboardStateWin32()[DIK_T] & 0x80) status = true;
		break;
	case fbc_input::U:
		if (inputDev.getKeyboardStateWin32()[DIK_U] & 0x80) status = true;
		break;
	case fbc_input::V:
		if (inputDev.getKeyboardStateWin32()[DIK_V] & 0x80) status = true;
		break;
	case fbc_input::W:
		if (inputDev.getKeyboardStateWin32()[DIK_W] & 0x80) status = true;
		break;
	case fbc_input::X:
		if (inputDev.getKeyboardStateWin32()[DIK_X] & 0x80) status = true;
		break;
	case fbc_input::Y:
		if (inputDev.getKeyboardStateWin32()[DIK_Y] & 0x80) status = true;
		break;
	case fbc_input::Z:
		if (inputDev.getKeyboardStateWin32()[DIK_Z] & 0x80) status = true;
		break;
	case fbc_input::N1:
		if (inputDev.getKeyboardStateWin32()[DIK_1] & 0x80) status = true;
		break;
	case fbc_input::N2:
		if (inputDev.getKeyboardStateWin32()[DIK_2] & 0x80) status = true;
		break;
	case fbc_input::N3:
		if (inputDev.getKeyboardStateWin32()[DIK_3] & 0x80) status = true;
		break;
	case fbc_input::N4:
		if (inputDev.getKeyboardStateWin32()[DIK_4] & 0x80) status = true;
		break;
	case fbc_input::N5:
		if (inputDev.getKeyboardStateWin32()[DIK_5] & 0x80) status = true;
		break;
	case fbc_input::N6:
		if (inputDev.getKeyboardStateWin32()[DIK_6] & 0x80) status = true;
		break;
	case fbc_input::N7:
		if (inputDev.getKeyboardStateWin32()[DIK_7] & 0x80) status = true;
		break;
	case fbc_input::N8:
		if (inputDev.getKeyboardStateWin32()[DIK_8] & 0x80) status = true;
		break;
	case fbc_input::N9:
		if (inputDev.getKeyboardStateWin32()[DIK_9] & 0x80) status = true;
		break;
	case fbc_input::N0:
		if (inputDev.getKeyboardStateWin32()[DIK_0] & 0x80) status = true;
		break;
	case fbc_input::DASH:
		if (inputDev.getKeyboardStateWin32()[DIK_MINUS] & 0x80) status = true;
		break;
	case fbc_input::EQUALS:
		if (inputDev.getKeyboardStateWin32()[DIK_EQUALS] & 0x80) status = true;
		break;
	case fbc_input::OBRACKET:
		if (inputDev.getKeyboardStateWin32()[DIK_LBRACKET] & 0x80) status = true;
		break;
	case fbc_input::CBRACKET:
		if (inputDev.getKeyboardStateWin32()[DIK_RBRACKET] & 0x80) status = true;
		break;
	case fbc_input::SEMI_COL:
		if (inputDev.getKeyboardStateWin32()[DIK_SEMICOLON] & 0x80) status = true;
		break;
	case fbc_input::APOSTROPHE:
		if (inputDev.getKeyboardStateWin32()[DIK_APOSTROPHE] & 0x80) status = true;
		break;
	case fbc_input::FORWARDSLASH:
		if (inputDev.getKeyboardStateWin32()[DIK_SLASH] & 0x80) status = true;
		break;
	case fbc_input::ENTER:
		if (inputDev.getKeyboardStateWin32()[DIK_RETURN] & 0x80) status = true;
		break;
	case fbc_input::BACKSPACE:
		if (inputDev.getKeyboardStateWin32()[DIK_BACK] & 0x80) status = true;
		break;
	case fbc_input::COMMA:
		if (inputDev.getKeyboardStateWin32()[DIK_COMMA] & 0x80) status = true;
		break;
	case fbc_input::PERIOD:
		if (inputDev.getKeyboardStateWin32()[DIK_PERIOD] & 0x80) status = true;
		break;
	case fbc_input::BACKSLASH:
		if (inputDev.getKeyboardStateWin32()[DIK_BACKSLASH] & 0x80) status = true;
		break;
	case fbc_input::RSHIFT:
		if (inputDev.getKeyboardStateWin32()[DIK_RSHIFT] & 0x80) status = true;
		break;
	case fbc_input::LSHIFT:
		if (inputDev.getKeyboardStateWin32()[DIK_LSHIFT] & 0x80) status = true;
		break;
	case fbc_input::TAB:
		if (inputDev.getKeyboardStateWin32()[DIK_TAB] & 0x80) status = true;
		break;
	case fbc_input::GRAVE:
		if (inputDev.getKeyboardStateWin32()[DIK_GRAVE] & 0x80) status = true;
		break;
	case fbc_input::SPACE:
		if (inputDev.getKeyboardStateWin32()[DIK_SPACE] & 0x80) status = true;
		break;
	case fbc_input::LCONTROL:
		if (inputDev.getKeyboardStateWin32()[DIK_LCONTROL] & 0x80) status = true;
		break;
	case fbc_input::RCONTROL:
		if (inputDev.getKeyboardStateWin32()[DIK_RCONTROL] & 0x80) status = true;
		break;
	case fbc_input::LALT:
		if (inputDev.getKeyboardStateWin32()[DIK_LMENU] & 0x80) status = true;
		break;
	case fbc_input::RALT:
		if (inputDev.getKeyboardStateWin32()[DIK_RMENU] & 0x80) status = true;
		break;
	case fbc_input::ARROW_UP:
		if (inputDev.getKeyboardStateWin32()[DIK_UP] & 0x80) status = true;
		break;
	case fbc_input::ARROW_DOWN:
		if (inputDev.getKeyboardStateWin32()[DIK_DOWN] & 0x80) status = true;
		break;
	case fbc_input::ARROW_RIGHT:
		if (inputDev.getKeyboardStateWin32()[DIK_RIGHT] & 0x80) status = true;
		break;
	case fbc_input::ARROW_LEFT:
		if (inputDev.getKeyboardStateWin32()[DIK_LEFT] & 0x80) status = true;
		break;
	case fbc_input::ESC:
		if (inputDev.getKeyboardStateWin32()[DIK_ESCAPE] & 0x80) status = true;
		break;
	case fbc_input::F1:
		if (inputDev.getKeyboardStateWin32()[DIK_F1] & 0x80) status = true;
		break;
	case fbc_input::F2:
		if (inputDev.getKeyboardStateWin32()[DIK_F2] & 0x80) status = true;
		break;
	case fbc_input::F3:
		if (inputDev.getKeyboardStateWin32()[DIK_F3] & 0x80) status = true;
		break;
	case fbc_input::F4:
		if (inputDev.getKeyboardStateWin32()[DIK_F4] & 0x80) status = true;
		break;
	case fbc_input::F5:
		if (inputDev.getKeyboardStateWin32()[DIK_F5] & 0x80) status = true;
		break;
	case fbc_input::F6:
		if (inputDev.getKeyboardStateWin32()[DIK_F6] & 0x80) status = true;
		break;
	case fbc_input::F7:
		if (inputDev.getKeyboardStateWin32()[DIK_F7] & 0x80) status = true;
		break;
	case fbc_input::F8:
		if (inputDev.getKeyboardStateWin32()[DIK_F8] & 0x80) status = true;
		break;
	case fbc_input::F9:
		if (inputDev.getKeyboardStateWin32()[DIK_F9] & 0x80) status = true;
		break;
	case fbc_input::F10:
		if (inputDev.getKeyboardStateWin32()[DIK_F10] & 0x80) status = true;
		break;
	case fbc_input::F11:
		if (inputDev.getKeyboardStateWin32()[DIK_F11] & 0x80) status = true;
		break;
	case fbc_input::F12:
		if (inputDev.getKeyboardStateWin32()[DIK_F12] & 0x80) status = true;
		break;
	case fbc_input::NUMPAD_0:
		if (inputDev.getKeyboardStateWin32()[DIK_NUMPAD0] & 0x80) status = true;
		break;
	case fbc_input::NUMPAD_1:
		if (inputDev.getKeyboardStateWin32()[DIK_NUMPAD1] & 0x80) status = true;
		break;
	case fbc_input::NUMPAD_2:
		if (inputDev.getKeyboardStateWin32()[DIK_NUMPAD2] & 0x80) status = true;
		break;
	case fbc_input::NUMPAD_3:
		if (inputDev.getKeyboardStateWin32()[DIK_NUMPAD3] & 0x80) status = true;
		break;
	case fbc_input::NUMPAD_4:
		if (inputDev.getKeyboardStateWin32()[DIK_NUMPAD4] & 0x80) status = true;
		break;
	case fbc_input::NUMPAD_5:
		if (inputDev.getKeyboardStateWin32()[DIK_NUMPAD5] & 0x80) status = true;
		break;
	case fbc_input::NUMPAD_6:
		if (inputDev.getKeyboardStateWin32()[DIK_NUMPAD6] & 0x80) status = true;
		break;
	case fbc_input::NUMPAD_7:
		if (inputDev.getKeyboardStateWin32()[DIK_NUMPAD7] & 0x80) status = true;
		break;
	case fbc_input::NUMPAD_8:
		if (inputDev.getKeyboardStateWin32()[DIK_NUMPAD8] & 0x80) status = true;
		break;
	case fbc_input::NUMPAD_9:
		if (inputDev.getKeyboardStateWin32()[DIK_NUMPAD9] & 0x80) status = true;
		break;
	case fbc_input::NUMPAD_SLASH:
		if (inputDev.getKeyboardStateWin32()[DIK_DIVIDE] & 0x80) status = true;
		break;
	case fbc_input::NUMPAD_ASTERISK:
		if (inputDev.getKeyboardStateWin32()[DIK_MULTIPLY] & 0x80) status = true;
		break;
	case fbc_input::NUMPAD_MINUS:
		if (inputDev.getKeyboardStateWin32()[DIK_SUBTRACT] & 0x80) status = true;
		break;
	case fbc_input::NUMPAD_PLUS:
		if (inputDev.getKeyboardStateWin32()[DIK_ADD] & 0x80) status = true;
		break;
	case fbc_input::NUMPAD_ENTER:
		if (inputDev.getKeyboardStateWin32()[DIK_NUMPADENTER] & 0x80) status = true;
		break;
	case fbc_input::NUMPAD_DECIMAL:
		if (inputDev.getKeyboardStateWin32()[DIK_DECIMAL] & 0x80) status = true;
		break;
		

		/***************************************************
		WINDOWS DirectInput (mouse buttons)
		***************************************************/

	case fbc_input::MOUSE_CLICK_LEFT:
		if (inputDev.getMouseStateWin32()->rgbButtons[0] & 0x80) status = true;
		break;
	case fbc_input::MOUSE_CLICK_RIGHT:
		if (inputDev.getMouseStateWin32()->rgbButtons[1] & 0x80) status = true;
		break;

		/***************************************************
		                 WINDOWS XINPUT
		***************************************************/

	case fbc_input::XA:
		if (inputDev.controlerConnectedWin32()) 
			status = (inputDev.getGamepadStateWin32()[0].wButtons & XINPUT_GAMEPAD_A);
		break;
	case fbc_input::XB:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].wButtons & XINPUT_GAMEPAD_B);
		break;
	case fbc_input::XY:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].wButtons & XINPUT_GAMEPAD_Y);
		break;
	case fbc_input::XX:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].wButtons & XINPUT_GAMEPAD_X);
		break;
	case fbc_input::START:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].wButtons & XINPUT_GAMEPAD_START);
		break;
	case fbc_input::BACK:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].wButtons & XINPUT_GAMEPAD_BACK);
		break;
	case fbc_input::UP:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].wButtons & XINPUT_GAMEPAD_DPAD_UP);
		break;
	case fbc_input::DOWN:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
		break;
	case fbc_input::LEFT:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
		break;
	case fbc_input::RIGHT:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
		break;
	case fbc_input::RB:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
		break;
	case fbc_input::LB:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
		break;
	case fbc_input::LT_DIGITAL:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].bLeftTrigger > 128);
		break;
	case fbc_input::RT_DIGITAL:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].bRightTrigger > 128);
		break;
	case fbc_input::LTHUMB:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
		break;
	case fbc_input::LTHUMB_UP:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].sThumbLY > fbc_input::DEADZONE);
		break;
	case fbc_input::LTHUMB_DOWN:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].sThumbLY < (-1 * fbc_input::DEADZONE));
		break;
	case fbc_input::LTHUMB_LEFT:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].sThumbLX < (-1 * fbc_input::DEADZONE));
		break;
	case fbc_input::LTHUMB_RIGHT:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].sThumbLX > fbc_input::DEADZONE);
		break;
	case fbc_input::RTHUMB:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
		break;
	case fbc_input::RTHUMB_UP:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].sThumbRY > fbc_input::DEADZONE);
		break;
	case fbc_input::RTHUMB_DOWN:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].sThumbRY < (-1 * fbc_input::DEADZONE));
		break;
	case fbc_input::RTHUMB_LEFT:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].sThumbRX < (-1 * fbc_input::DEADZONE));
		break;
	case fbc_input::RTHUMB_RIGHT:
		if (inputDev.controlerConnectedWin32())
			status = (inputDev.getGamepadStateWin32()[0].sThumbRX > fbc_input::DEADZONE);
		break;
	}

	return fbc_err::OK;
}
#endif