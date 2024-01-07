/***************************************************************************************************
FatBoy Engine -- debug

Purpose: this file serves as a registry of debug bool switches. all switches are in the namespace
fbc_debug and have descriptions located here discribing they're functions.
***************************************************************************************************/

#pragma once

namespace fbc_debug
{
#ifndef DEBUG_FILE
#define DEBUG_FILE

	//general debug
	const bool WINDOWED_MODE = true;		//run program in windowed mode as opposed to fullscreen for debug purposes (windows systems)
	const bool SHOW_DEBUG_CONSOLE = true;	//run program with the debug console on for diagnostics
#endif
}