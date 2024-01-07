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

fatcore::analog_input::analog_input(const int INPUT):
INPUT_CODE(INPUT),
status(0),
uniqueID(0)
{
	//generate a unique id for the newly registered input, conflicts will be handled by the input manager,
	//and new ids will be assigned in the event of a conflict
	generateID();
}
void fatcore::analog_input::poll()
{
	status = 0; //reset the input status
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

int fatcore::analog_input::pollWin32()
{
	fatcore::input inputDev;

	//check for code and update status
	switch (INPUT_CODE)
	{
		//Direct input analog (mouse)
	case fbc_input::MOUSE_X:
		status = static_cast<int>(inputDev.getMouseStateWin32()->lX);
		break;
	case fbc_input::MOUSE_Y:
		status = static_cast<int>(inputDev.getMouseStateWin32()->lY);
		break;

		//Xinput analog
	case fbc_input::LEFT_X:
		if (static_cast<int>(inputDev.getGamepadStateWin32()[0].sThumbLX) > fbc_input::DEADZONE ||
			static_cast<int>(inputDev.getGamepadStateWin32()[0].sThumbLX) < (-1 * fbc_input::DEADZONE)) 
			status = static_cast<int>(inputDev.getGamepadStateWin32()[0].sThumbLX);
		break;
	case fbc_input::LEFT_Y:
		if (static_cast<int>(inputDev.getGamepadStateWin32()[0].sThumbLY) > fbc_input::DEADZONE ||
			static_cast<int>(inputDev.getGamepadStateWin32()[0].sThumbLY) < (-1 * fbc_input::DEADZONE))
			status = static_cast<int>(inputDev.getGamepadStateWin32()[0].sThumbLY);
		break;
	case fbc_input::RIGHT_X:
		if (static_cast<int>(inputDev.getGamepadStateWin32()[0].sThumbRX) > fbc_input::DEADZONE ||
			static_cast<int>(inputDev.getGamepadStateWin32()[0].sThumbRX) < (-1 * fbc_input::DEADZONE))
			status = static_cast<int>(inputDev.getGamepadStateWin32()[0].sThumbRX);
		break;
	case fbc_input::RIGHT_Y:
		if (static_cast<int>(inputDev.getGamepadStateWin32()[0].sThumbRY) > fbc_input::DEADZONE ||
			static_cast<int>(inputDev.getGamepadStateWin32()[0].sThumbRY) < (-1 * fbc_input::DEADZONE))
			status = static_cast<int>(inputDev.getGamepadStateWin32()[0].sThumbRY);
		break;
	case fbc_input::RT:
		status = static_cast<int>(inputDev.getGamepadStateWin32()[0].bRightTrigger);
		break;
	case fbc_input::LT:
		status = static_cast<int>(inputDev.getGamepadStateWin32()[0].bLeftTrigger);
		break;
	}

	return fbc_err::OK;
}

#endif