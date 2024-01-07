/***************************************************************************************************
FatBoy Engine -- main

Purpose: this file serves as the main program entry point specific to the system the code is being
compiled on
***************************************************************************************************/

#include <core.h>
#include <debug.h>
#include <input.h>
#include <render.h>
#include <sound.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sprites.h>
#include <fatcore_spriteID.h>
#include <menu-support.h>
#include <render_2.h>


/**************************************************************************************************
Windows with DirectX
**************************************************************************************************/
#ifdef _WIN32

#include <Windows.h>

bool holdCursor = true;

//Windows event handler
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
		fatcore::input iDev;

	switch (msg)
	{
	case WM_DESTROY:
		fatcore::game_running = false;
		PostQuitMessage(0);
		return 0;
	case WM_SETFOCUS:
		if (iDev.isInitialized())
		{
			iDev.getKeyboardWin32()->Acquire();
			iDev.getMouseWin32()->Acquire();
		}
		holdCursor = true;
		while (ShowCursor(false) >= 0);
		break;
	case WM_KILLFOCUS:
		holdCursor = false;
		while (ShowCursor(true) <= 0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//main windows entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//display the console window if the debug switch is active
	if (fbc_debug::SHOW_DEBUG_CONSOLE)
	{
		AllocConsole();
		freopen("CONOUT$", "wb", stdout);
	}

	//create a render object and obtain resolution from it
	fatcore::render2 device;
	fatcore::res resolution = device.getScreenResolution();

	//set window's properties
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = fatcore::game_title.c_str();
	wc.hIconSm = NULL;
	RegisterClassEx(&wc);

	//create a new window
	HWND window = CreateWindow(fatcore::game_title.c_str(), fatcore::game_title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
		resolution.width, resolution.height, NULL, NULL, hInstance, NULL);
	if (window == 0) return 0;

	//display the window
	ShowWindow(window, nCmdShow);
	UpdateWindow(window);

	//hide the system cursor
	while (ShowCursor(false) >= 0);

	//pass the window handle to the render manager
	fatcore::w32dx_configWindow(&device, window);

	//run game initilization code
	int result = fatcore::Game_Init();
	if (result != fbc_err::OK) return result;

	//main message loop
	MSG message;
	while (fatcore::game_running)
	{
		//keep cursor lined up
		RECT windowCoords;
		GetWindowRect(window, &windowCoords);
		
		if (holdCursor) SetCursorPos((windowCoords.left + 150), (windowCoords.top + 150));

		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		fatcore::Game_Loop();
	}

	fatcore::Game_Shutdown();

	return 0;
}

#endif