/***************************************************************************************************
FatBoy Engine -- input

Purpose: this will define all objects needed for basic input and output operations, inputs will
be handled by a central input system that maps inputs to results. Inputs are devided into two
types digital or analog
***************************************************************************************************/

#include "universal_includes.h"
#include "errors.h"
#include "render.h"
#include "debug.h"
#include "core.h"
#include "input.h"

//input_handle Deffinitions
fatcore::input_handle::input_handle(const int UNIQUE_ID, const std::string ACTION_NAME)
{
	uniqueID = UNIQUE_ID;
	actionName = ACTION_NAME;
}
void fatcore::input_handle::quickSet(const int UNIQUE_ID, const std::string ACTION_NAME)
{
	uniqueID = UNIQUE_ID;
	actionName = ACTION_NAME;
}

//static data member initialization
int fatcore::input::inputCount = 0;
bool fatcore::input::initialized = false;
std::vector<fatcore::digital_input*> fatcore::input::digital_inputs;
std::vector<fatcore::analog_input*> fatcore::input::analog_inputs;

//windows static data member initialization
#ifdef _WIN32

LPDIRECTINPUT8 fatcore::input::m_Win32_dinput;
LPDIRECTINPUTDEVICE8 fatcore::input::m_Win32_keyboard;
LPDIRECTINPUTDEVICE8 fatcore::input::m_Win32_mouse;
DIMOUSESTATE fatcore::input::m_Win32_mouseState;
char fatcore::input::m_Win32_keys[256];
XINPUT_GAMEPAD fatcore::input::m_Win32_controlers[4];

#endif

fatcore::input::input()
{
	//incremnet input counter
	inputCount++;
}
fatcore::input::~input()
{
	//deincrement input counter
	inputCount--;

	//if input count reaches 0, run shutdown function
	if (inputCount == 0)
	{
		shutdown();
	}
}
void fatcore::input::update()
{
	//failsafe, if not initialized function wont run
	if (!initialized) return;

	//windows update function
#ifdef _WIN32
	
	updateWin32();

#endif
}
void fatcore::input::shutdown()
{
	//free memory from all inputs
	for (unsigned int i = 0; i < analog_inputs.size(); ++i) //analog inputs
	{
		delete analog_inputs[i];
		analog_inputs[i] = NULL;
	}
	for (unsigned int i = 0; i < digital_inputs.size(); ++i) //digital inputs
	{
		delete digital_inputs[i];
		digital_inputs[i] = NULL;
	}

	//run system specific functions
#ifdef _WIN32

	shutdownWin32();

#endif
}
bool fatcore::input::isInitialized() { return initialized; }
int fatcore::input::initialize()
{
	//error checking
	int result = 0;

	//run system specific functions
#ifdef _WIN32

	result = initializeWin32(); 

#endif

	if (result == fbc_err::OK)
	{
		initialized = true;
		std::cout << "[INIT:] Input device initialzation complete." << std::endl;
		return fbc_err::OK;
	}

	std::cout << "[WARNING:] Input device initialzation failed. ERROR: " << result << std::endl;
	return fbc_err::INIT_FAIL;
}
int fatcore::input::checkStatus(int& status, const int UNIQUE_ID)
{
	//failsafe, if not initialized function won't continue
	if (!initialized)
	{
		std::cout << "[ERROR:] input status check fail, input system not initialized." << std::endl; 
		return fbc_err::INPUT_NOT_INITIALIZED;
	}
	for (unsigned int i = 0; i < analog_inputs.size(); ++i)
	{
		if (analog_inputs[i]->uniqueID == UNIQUE_ID)
		{
			analog_inputs[i]->poll();
			status = analog_inputs[i]->status;
			return fbc_err::OK;
		}
	}

	return fbc_err::INPUT_NOT_SUPPORTED_BY_FUNCTION;
}
int fatcore::input::checkStatus(bool& status, const int UNIQUE_ID)
{
	//failsafe, if not initialized function won't continue
	if (!initialized)
	{
		std::cout << "[ERROR:] input status check fail, input system not initialized." << std::endl;
		return fbc_err::INPUT_NOT_INITIALIZED;
	}
	for (unsigned int i = 0; i < digital_inputs.size(); ++i)
	{
		if (digital_inputs[i]->uniqueID == UNIQUE_ID)
		{
			digital_inputs[i]->poll();
			status = digital_inputs[i]->status;
			return fbc_err::OK;
		}
	}

	return fbc_err::INPUT_NOT_SUPPORTED_BY_FUNCTION;
}
int fatcore::input::registerInput(const int INPUT_ID, int& uniqueID)
{
	//failsafe, if not initialized function won't continue
	if (!initialized) return fbc_err::INPUT_NOT_INITIALIZED;

	//verify input type
	//analog inputs
	if ((INPUT_ID > fbc_input::DIRECT_INPUT_ANALOG_START && INPUT_ID < fbc_input::DIRECT_INPUT_ANALOG_END) ||
		(INPUT_ID > fbc_input::XINPUT_ANALOG_START && INPUT_ID < fbc_input::XINPUT_ANALOG_END))
	{
		for (unsigned int i = 0; i < analog_inputs.size(); ++i) //analog inputs
		{
			if (analog_inputs[i]->INPUT_CODE == INPUT_ID)
			{
				uniqueID = analog_inputs[i]->uniqueID;
				return fbc_err::OK;
			}
		}

		//create the input entry
		fatcore::analog_input* temp = new fatcore::analog_input(INPUT_ID);
		
		//verify newly registered input has a uniqueID
		bool idIsUnique = false;
		while (!idIsUnique)
		{
			bool duplicateFound = false;

			//search all registered ids for a duplicate
			for (unsigned int i = 0; i < analog_inputs.size(); ++i)
			{
				if (analog_inputs[i]->uniqueID == temp->uniqueID)
				{
					temp->generateID();
					duplicateFound = true;
				}
			}

			if (duplicateFound)	continue;
			else idIsUnique = true;
		}

		uniqueID = temp->uniqueID;
		analog_inputs.push_back(temp);
		return fbc_err::OK;
	}
	//digital inputs
	else if ((INPUT_ID > fbc_input::DIRECT_INPUT_DIGITAL_START && INPUT_ID < fbc_input::DIRECT_INPUT_DIGITAL_END) ||
		(INPUT_ID > fbc_input::DIRECT_INPUT_DIGITAL_MOUSE_START && INPUT_ID < fbc_input::DIRECT_INPUT_DIGITAL_MOUSE_END) ||
		(INPUT_ID > fbc_input::XINPUT_DIGITAL_START && INPUT_ID < fbc_input::XINPUT_DIGITAL_END))
	{
		//check if input is already registered, pass uniqueID back if so
		for (unsigned int i = 0; i < digital_inputs.size(); ++i) //analog inputs
		{
			if (digital_inputs[i]->INPUT_CODE == INPUT_ID)
			{
				uniqueID = digital_inputs[i]->uniqueID;
				return fbc_err::OK;
			}
		}

		//create the input entry
		fatcore::digital_input* temp = new fatcore::digital_input(INPUT_ID);

		//verify newly registered input has a uniqueID
		bool idIsUnique = false;
		while (!idIsUnique)
		{
			bool duplicateFound = false;

			//search all registered ids for a duplicate
			for (unsigned int i = 0; i < digital_inputs.size(); ++i)
			{
				if (digital_inputs[i]->uniqueID == temp->uniqueID)
				{
					temp->generateID();
					duplicateFound = true;
				}
			}

			if (duplicateFound)	continue;
			else idIsUnique = true;
		}

		uniqueID = temp->uniqueID;
		digital_inputs.push_back(temp);
		return fbc_err::OK;
	}

	return fbc_err::INPUT_NOT_SUPPORTED_BY_FUNCTION;
}

//windows functions
#ifdef _WIN32
void fatcore::input::shutdownWin32()
{
	if (initialized)
	{
		initialized = false;
		
		//release the direct input device
		if (m_Win32_dinput)
		{
			m_Win32_dinput->Release();
			m_Win32_dinput = NULL;
		}

		//release the keyboard
		if (m_Win32_keyboard)
		{
			m_Win32_keyboard->Unacquire();
			m_Win32_keyboard->Release();
			m_Win32_keyboard = NULL;
		}

		//release the mouse
		if (m_Win32_mouse)
		{
			m_Win32_mouse->Unacquire();
			m_Win32_mouse->Release();
			m_Win32_mouse = NULL;
		}
	}
}
void fatcore::input::updateWin32()
{
	//update mouse
	m_Win32_mouse->GetDeviceState(sizeof(m_Win32_mouseState), (LPVOID)&m_Win32_mouseState);

	//update keyboard
	m_Win32_keyboard->GetDeviceState(sizeof(m_Win32_keys), (LPVOID)&m_Win32_keys);

	//update xinput controller
	ZeroMemory(&m_Win32_controlers[0], sizeof(XINPUT_STATE));

	XINPUT_STATE state;
	DWORD result = XInputGetState(0, &state);

	if (result == 0) m_Win32_controlers[0] = state.Gamepad;
}
int fatcore::input::initializeWin32()
{
	//create a render device to obtain window handle
	fatcore::render renderDevice;

	//create a HRESULT object to handle errors
	HRESULT result;

	//initialize the direct input device
	result = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8,
								(void**)&m_Win32_dinput, NULL);
	if (result != D3D_OK) return fbc_err::INIT_FAIL;

	//iniitialize and aquire the keyboard
	result = m_Win32_dinput->CreateDevice(GUID_SysKeyboard, &m_Win32_keyboard, NULL);
	if (result != D3D_OK) return fbc_err::INIT_FAIL;

	result = m_Win32_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (result != D3D_OK) return fbc_err::INIT_FAIL;

	result = m_Win32_keyboard->SetCooperativeLevel(renderDevice.getWindow(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (result != D3D_OK) return fbc_err::INIT_FAIL;

	result = m_Win32_keyboard->Acquire();
	if (result != D3D_OK) return fbc_err::INIT_FAIL;

	//initialize and aquire the mouse
	result = m_Win32_dinput->CreateDevice(GUID_SysMouse, &m_Win32_mouse, NULL);
	if (result != D3D_OK) return fbc_err::INIT_FAIL;

	result = m_Win32_mouse->SetDataFormat(&c_dfDIMouse);
	if (result != D3D_OK) return fbc_err::INIT_FAIL;

	result = m_Win32_mouse->SetCooperativeLevel(renderDevice.getWindow(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (result != D3D_OK) return fbc_err::INIT_FAIL;

	result = m_Win32_mouse->Acquire();
	if (result != D3D_OK) return fbc_err::INIT_FAIL;

	LPDIRECT3DDEVICE9 device = renderDevice.getDevice();
	device->ShowCursor(false);

	
	return fbc_err::OK;
}
bool fatcore::input::controlerConnectedWin32()
{
	XINPUT_CAPABILITIES caps;
	ZeroMemory(&caps, sizeof(XINPUT_CAPABILITIES));
	XInputGetCapabilities(0, XINPUT_FLAG_GAMEPAD, &caps);
	if (caps.Type != 0) return false;
	
	return true;
}
LPDIMOUSESTATE fatcore::input::getMouseStateWin32() { return &m_Win32_mouseState; }
char* fatcore::input::getKeyboardStateWin32() { return m_Win32_keys; }
XINPUT_GAMEPAD* fatcore::input::getGamepadStateWin32() { return m_Win32_controlers; }
LPDIRECTINPUTDEVICE8 fatcore::input::getKeyboardWin32() { return m_Win32_keyboard; }
LPDIRECTINPUTDEVICE8 fatcore::input::getMouseWin32() { return m_Win32_mouse; }

#endif

/********************************************************************
                   INPUT DEVICE PLACEHOLDER
********************************************************************/
fatcore::input __INPUT_PLACEHOLDER__;