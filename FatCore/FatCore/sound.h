/***************************************************************************************************

FatBoy Engine -- Sound Management  -- All basic sound classes

Purpose: Handles all sounds in the game. Includes loading/unloading sound files and also handles
the OS specific actions needed to load and play sounds.

***************************************************************************************************/

#pragma once

#include "core.h"
#include "errors.h"
#include "render.h"
#include <ctime>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#ifdef _WIN32

#include "DirectSound.h"
#include <Windows.h>
#include <WinUser.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <Xinput.h>

#endif

namespace fatcore
{
#ifndef _FATCORE_SOUND_HEADER_
#define _FATCORE_SOUND_HEADER_

	class sound_file
	{
		friend class sound;

	protected:
		//member functions
		sound_file(const std::string FILENAME, int& uniqueID);
		~sound_file();
		void generateID(int& uniqueID);

		//data members
		std::string m_filename;
		static std::vector<int> m_ID_registry;
		int m_uniqueID;
		bool m_valid;

#ifdef _WIN32

		//windows member functions
		int loadWin32();
		void unloadWin32();

		//windows data members
		CSound *m_soundWin32;

#endif
	};

	class sound_handle
	{
	public:
		sound_handle(const std::string DESCRIPTION, const int UNIQUE_ID);

		int uniqueID;
		std::string description;
	};

	class sound
	{
		friend class sound_file;
	public:
		sound();
		~sound();
		int initialize(); //must be initialized AFTER the render device or init WILL fail
		void shutdown(); //last one out, get the lights
		int play(const int UNIQUE_ID, const bool LOOP = false);
		int stop(const int UNIQUE_ID);
		int load(const std::string FILENAME, int& uniqueID); 
		int purge(const int UNIQUE_ID);

	private:
		static int m_soundDevices;
		static bool m_initialized;
		static std::vector<sound_file*> m_soundFiles;

#ifdef _WIN32

	private:
		int playWin32(sound_file* soundFile, const bool LOOP);
		int stopWin32(sound_file* soundFile);
		void shutdownWin32();
		int intializeWin32();
		static CSoundManager* m_SoundManagerWin32;

#endif
	};

#endif
}