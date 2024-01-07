/***************************************************************************************************
FatBoy Engine -- input

Purpose: this will define all objects needed for basic input and output operations, inputs will
be handled by a central input system that maps inputs to results. Inputs are devided into two 
types digital or analog
***************************************************************************************************/

#include "universal_includes.h"
#include "core.h"
#include "errors.h"
#include <ctime>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#ifdef _WIN32

#include <Windows.h>
#include <WinUser.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <Xinput.h>

#endif

#pragma once

namespace fatcore
{
#ifndef INPUT_HEADER_FILE
#define INPUT_HEADER_FILE
	class input_handle
	{
	public:
		input_handle(const int UNIQUE_ID = 0, const std::string ACTION_NAME = "");
		void quickSet(const int UNIQUE_ID, const std::string ACTION_NAME);

		int uniqueID;
		std::string actionName;
	};

	class digital_input
	{
		friend class input;
	protected:
		digital_input(const int INPUT);
		void poll();
		inline void generateID() { uniqueID = rand(); }

		//windows polling methods
#ifdef _WIN32
		int pollWin32();
#endif

		int uniqueID;
		bool status;
		const int INPUT_CODE;
	};

	class analog_input
	{
		friend class input;
	protected:
		analog_input(const int INPUT);
		void poll();
		inline void generateID() { uniqueID = rand(); }

		//windows polling function
#ifdef _WIN32
		int pollWin32();
#endif

		int uniqueID;
		int status;
		const int INPUT_CODE;
	};

	class input
	{
	public:
		input();
		~input();
		void update();
		void shutdown();
		bool isInitialized();
		int initialize();
		int checkStatus(int& status, const int UNIQUE_ID); //check the status of an analog input via uniqueID, error code on failure
		int checkStatus(bool& status, const int UNIQUE_ID); //check the status of a digital input via uniqueID, error code on failure
		int registerInput(const int INPUT_ID, int& uniqueID); //will return an error code upon failure to register an input

#ifdef _WIN32
		void shutdownWin32();
		void updateWin32();
		bool controlerConnectedWin32();
		int initializeWin32();
		LPDIMOUSESTATE getMouseStateWin32();
		char * getKeyboardStateWin32();
		XINPUT_GAMEPAD* getGamepadStateWin32();
		LPDIRECTINPUTDEVICE8 getKeyboardWin32();
		LPDIRECTINPUTDEVICE8 getMouseWin32();
#endif

	private:
		static std::vector<analog_input*> analog_inputs;
		static std::vector<digital_input*> digital_inputs;
		static int inputCount;
		static bool initialized;

#ifdef _WIN32
		//windows specific data members
		static LPDIRECTINPUT8 m_Win32_dinput;
		static LPDIRECTINPUTDEVICE8 m_Win32_keyboard;
		static LPDIRECTINPUTDEVICE8 m_Win32_mouse;
		static DIMOUSESTATE m_Win32_mouseState;
		static XINPUT_GAMEPAD m_Win32_controlers[4];
		static char m_Win32_keys[256];
#endif
	};


#endif
}

namespace fbc_input
{
#ifndef __INPUT_CODES__
#define __INPUT_CODES__

	const int DEADZONE = 15000;

	enum INPUT_CODE
	{
		DIRECT_INPUT_DIGITAL_START = 100000,

		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		N1,
		N2,
		N3,
		N4,
		N5,
		N6,
		N7,
		N8,
		N9,
		N0,
		DASH,
		EQUALS,
		OBRACKET,
		CBRACKET,
		SEMI_COL,
		APOSTROPHE,
		FORWARDSLASH,
		ENTER,
		BACKSPACE,
		COMMA,
		PERIOD,
		BACKSLASH,
		RSHIFT,
		LSHIFT,
		TAB, 
		GRAVE,
		SPACE,
		LCONTROL,
		RCONTROL,
		LALT,
		RALT,
		ARROW_UP,
		ARROW_DOWN,
		ARROW_LEFT,
		ARROW_RIGHT,
		ESC, 
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		NUMPAD_1,
		NUMPAD_2,
		NUMPAD_3,
		NUMPAD_4,
		NUMPAD_5,
		NUMPAD_6,
		NUMPAD_7,
		NUMPAD_8,
		NUMPAD_9,
		NUMPAD_0,
		NUMPAD_SLASH,
		NUMPAD_ASTERISK,
		NUMPAD_MINUS,
		NUMPAD_PLUS,
		NUMPAD_ENTER,
		NUMPAD_DECIMAL,

		DIRECT_INPUT_DIGITAL_END,

		DIRECT_INPUT_DIGITAL_MOUSE_START,

		MOUSE_CLICK_RIGHT,
		MOUSE_CLICK_LEFT,

		DIRECT_INPUT_DIGITAL_MOUSE_END,

		DIRECT_INPUT_ANALOG_START,

		MOUSE_X,
		MOUSE_Y,

		DIRECT_INPUT_ANALOG_END,

		XINPUT_DIGITAL_START,

		XA,
		XB,
		XX,
		XY,
		START,
		BACK,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		RB,
		LB,
		LT_DIGITAL,
		RT_DIGITAL,
		LTHUMB,
		LTHUMB_UP,
		LTHUMB_DOWN,
		LTHUMB_LEFT,
		LTHUMB_RIGHT,
		RTHUMB,
		RTHUMB_UP,
		RTHUMB_DOWN,
		RTHUMB_LEFT,
		RTHUMB_RIGHT,

		XINPUT_DIGITAL_END,
	
		XINPUT_ANALOG_START,

		LEFT_X,
		LEFT_Y,
		RIGHT_X,
		RIGHT_Y,
		LT,
		RT,

		XINPUT_ANALOG_END
	};

#endif
}