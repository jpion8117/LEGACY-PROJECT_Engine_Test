/***************************************************************************************************
FatBoy Engine

Purpose: primary include file, include this file for access to all subsequent FatBoy Engine 
componet files. 
***************************************************************************************************/

#pragma once

#ifndef _MAIN_FATCORE_HEADER_
#define _MAIN_FATCORE_HEADER_

#include "core.h"
#include "debug.h"
#include "errors.h"
#include "render.h"
#include "render_2.h"
#include "input.h"
#include "sprites.h"
#include "sound.h"
#include "physics.h"
#include "menu-support.h"
#include "text_rendering.h"
#include "fatcore_scene.h"

#ifdef _WIN32

//libraries nessissary for DirectX and Windows
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "xinput.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr9.lib")

#endif

#endif