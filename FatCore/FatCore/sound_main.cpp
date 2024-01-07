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

//static data member initialization
int fatcore::sound::m_soundDevices = 0;
bool fatcore::sound::m_initialized = false;
std::vector<fatcore::sound_file*> fatcore::sound::m_soundFiles;

fatcore::sound::sound()
{
	//incriment device count
	m_soundDevices++;
}
fatcore::sound::~sound()
{
	//deincriment device count
	--m_soundDevices;

	//last one out, get the lights
	if (m_soundDevices == 0)
	{
		shutdown();
	}
}
int fatcore::sound::initialize()
{
	//error checking variable
	int result;

#ifdef _WIN32

	result = intializeWin32();

#endif

	if (result == fbc_err::OK)
	{
		m_initialized = true;
		return fbc_err::OK;
	}

	return fbc_err::INIT_FAIL;
}
void fatcore::sound::shutdown()
{
#ifdef _WIN32

	shutdownWin32();

#endif
}
int fatcore::sound::load(const std::string FILENAME, int& uniqueID)
{
	//failsafe
	if (!m_initialized)
		return fbc_err::AUDIO_DEVICE_NOT_READY;

	//create the sound file and load from memory
	sound_file* temp = new sound_file(FILENAME, uniqueID);

	//verify file loaded
	if (!temp->m_valid)
	{
		return fbc_err::FAILED_TO_LOAD_AUDIO_FILE;
	}

	m_soundFiles.push_back(temp);
	return fbc_err::OK;
}
int fatcore::sound::play(const int UNIQUE_ID, const bool LOOP)
{
	//failsafe if not initialized
	if (!m_initialized)
		return fbc_err::AUDIO_DEVICE_NOT_READY;

	sound_file* soundFile = NULL; //pointer to correct sound file

	//begin search loop for referenced file
	for (unsigned int i = 0; i < m_soundFiles.size(); ++i)
	{
		if (m_soundFiles[i]->m_uniqueID == UNIQUE_ID)
		{
			soundFile = m_soundFiles[i];
			break;
		}
	}

	if (soundFile == NULL)
		return fbc_err::ID_NOT_FOUND;

	int result = 0; //error checking variable

	//call the system specific function
#ifdef _WIN32

	result = playWin32(soundFile, LOOP);

#endif

	if (result != fbc_err::OK)
		return fbc_err::UNABLE_TO_PLAY_FILE;

	return fbc_err::OK;
}
int fatcore::sound::stop(const int UNIQUE_ID)
{

	//failsafe if not initialized
	if (!m_initialized)
		return fbc_err::AUDIO_DEVICE_NOT_READY;

	sound_file* soundFile = NULL; //pointer to correct sound file

	//begin search loop for referenced file
	for (unsigned int i = 0; i < m_soundFiles.size(); ++i)
	{
		if (m_soundFiles[i]->m_uniqueID == UNIQUE_ID)
		{
			soundFile = m_soundFiles[i];
			break;
		}
	}

	if (soundFile == NULL)
		return fbc_err::ID_NOT_FOUND;

	int result = 0; //error checking variable

	//call the system specific function
#ifdef _WIN32

	result = stopWin32(soundFile);

#endif

	if (result != fbc_err::OK)
		return fbc_err::GENERIC_FAILURE;

	return fbc_err::OK;
}
int fatcore::sound::purge(const int UNIQUE_ID)
{

	//failsafe if not initialized
	if (!m_initialized)
		return fbc_err::AUDIO_DEVICE_NOT_READY;

	sound_file* soundFile = NULL; //pointer to correct sound file
	unsigned int locFile = 0;

	//begin search loop for referenced file
	for (unsigned int i = 0; i < m_soundFiles.size(); ++i)
	{
		if (m_soundFiles[i]->m_uniqueID == UNIQUE_ID)
		{
			soundFile = m_soundFiles[i];
			locFile = i;
			break;
		}
	}

	if (soundFile == NULL)
		return fbc_err::ID_NOT_FOUND;

	delete soundFile;
	soundFile = NULL;
	m_soundFiles.erase((m_soundFiles.begin() + locFile));

	return fbc_err::OK;
}

#ifdef _WIN32

CSoundManager* fatcore::sound::m_SoundManagerWin32 = NULL;

int fatcore::sound::playWin32(sound_file* soundFile, const bool LOOP)
{
	if (soundFile->m_valid)
	{
		if (LOOP)
		{
			soundFile->m_soundWin32->Play(0, DSBPLAY_LOOPING);
			return fbc_err::OK;
		}
		else
		{
			soundFile->m_soundWin32->Play();
			return fbc_err::OK;
		}
	}

	return fbc_err::UNABLE_TO_PLAY_FILE;
}
int fatcore::sound::stopWin32(sound_file* soundFile)
{
	if (soundFile->m_valid)
	{
		soundFile->m_soundWin32->Stop();
		return fbc_err::OK;
	}

	return fbc_err::GENERIC_FAILURE;
}
int fatcore::sound::intializeWin32()
{
	HRESULT result; //windows error checking
	render renderDev; //needed for window handle

	m_SoundManagerWin32 = new CSoundManager();

	//initialize direct sound
	result = m_SoundManagerWin32->Initialize(renderDev.getWindow(), DSSCL_PRIORITY);
	if (result != DS_OK) return fbc_err::INIT_FAIL;

	//set the primary buffer format
	result = m_SoundManagerWin32->SetPrimaryBufferFormat(2, 22050, 16);
	if (result != DS_OK) return fbc_err::INIT_FAIL;

	return fbc_err::OK;
}
void fatcore::sound::shutdownWin32()
{
	if (m_SoundManagerWin32)
	{
		delete m_SoundManagerWin32;
		m_SoundManagerWin32 = NULL;
		m_initialized = false;
	}
}

#endif

//sound device place holder
fatcore::sound __PRIMARY_SOUND_DEVICE__;