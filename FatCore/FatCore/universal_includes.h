/***************************************************************************************************
FatBoy Engine

Purpose: primary include file, include this file for access to all universal includes that are
not directly associated with the engine
***************************************************************************************************/

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