/***************************************************************************************************

FatBoy Engine -- Sound Management  -- All basic sound classes

Purpose: Handles all sounds in the game. Includes loading/unloading sound files and also handles
the OS specific actions needed to load and play sounds.

***************************************************************************************************/

#include "universal_includes.h"
#include "errors.h"
#include "debug.h"
#include "core.h"

//object header file
#include "sound.h"

std::vector<int> fatcore::sound_file::m_ID_registry;

fatcore::sound_file::sound_file(const std::string FILENAME, int& uniqueID):
m_filename(FILENAME)
{
	//generate a new ID
	generateID(uniqueID);

	//error checking
	int result = 0;
	
#ifdef _WIN32

	result = loadWin32();

#endif

	m_valid = (result == fbc_err::OK);
}
fatcore::sound_file::~sound_file()
{
#ifdef _WIN32

	//call the windows sound unloader
	unloadWin32();

#endif
}
void fatcore::sound_file::generateID(int& uniqueID)
{
START_ID_GEN: //return point if there is an ID conflict

	//pull a random number to use as an ID
	m_uniqueID = rand();

	//check if ID is unique
	for (unsigned int i = 0; i < m_ID_registry.size(); ++i)
	{
		if (m_ID_registry[i] == m_uniqueID)
		{
			goto START_ID_GEN;
		}
	}

	//add new ID to the registry
	m_ID_registry.push_back(m_uniqueID);

	//pass the uniqueID back up the chain to the calling functions
	uniqueID = m_uniqueID;
}

#ifdef _WIN32

int fatcore::sound_file::loadWin32()
{
	m_soundWin32 = NULL;

	//get the sound device to access the CSoundManager pointer
	sound soundDev;
	
	CSound* wave = NULL;

	char s[255];
	sprintf_s(s, "%s", m_filename.c_str());
	HRESULT result;
	result = soundDev.m_SoundManagerWin32->Create(&wave, s);
	if (result != DS_OK)
	{
		m_soundWin32 = NULL;
		return fbc_err::FAILED_TO_LOAD_AUDIO_FILE;
	}

	m_soundWin32 = wave;
	m_valid = true;
	return fbc_err::OK;
}
void fatcore::sound_file::unloadWin32()
{
	delete m_soundWin32;
	m_soundWin32 = NULL;
	m_valid = false;
}

#endif