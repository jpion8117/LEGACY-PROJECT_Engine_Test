/***************************************************************************************************
FatBoy Engine -- Render2

Purpose: Rev 2 of the rendering classes. Original code base for the render classes will be built
upon, extended, and improved, to add new functionality and run more effeciently
***************************************************************************************************/

#include "universal_includes.h"
#include "debug.h"
#include "errors.h"
#include "render_2.h"
#include "text_rendering.h"
#include <algorithm>

/***********************************************************************************

MEMBER FUNCTIONS - object_foreground_2

***********************************************************************************/
fatcore::object_foreground_2::object_foreground_2(const std::string FILENAME, const int UNIQUE_ID, 
	const int PRIORITY, const fatcore::rgb TRANSPARANCY_COLOR):
m_FILENAME(FILENAME),
m_UNIQUE_ID(UNIQUE_ID),
m_priority(PRIORITY),
m_transparancyColor(TRANSPARANCY_COLOR),
m_active(true),
m_sharedFile(false),
m_renderColor({ 255, 255, 255 }),
m_renderLocation({ 0, 0, 0, 0 }),
m_sourceLocation({ 0, 0, 0, 0 }),
m_scaleX(1.0),
m_scaleY(1.0),
m_rotation(0)
{
#ifdef _WIN32
	m_w32dx_file = NULL; //set later by render 2 system
#endif
}
fatcore::object_foreground_2::~object_foreground_2()
{
	m_active = false; //fuck it, better safe than corrupt

	//if this file is shared with another asset, don't release it
	if (m_sharedFile) return;

#ifdef _WIN32
	if (m_w32dx_file)
	{
		m_w32dx_file->Release();
		m_w32dx_file = NULL;
	}
#endif
}
bool fatcore::object_foreground_2::operator<(object_foreground_2& comp)
{
	if (this->m_priority < comp.m_priority)
	{
		return true;
	}

	return false;
}
bool fatcore::object_foreground_2::operator>(object_foreground_2& comp)
{
	if (this->m_priority > comp.m_priority)
	{
		return true;
	}

	return false;
}

/***********************************************************************************

MEMBER FUNCTIONS - object_background_2

***********************************************************************************/
fatcore::object_background_2::object_background_2(const std::string FILENAME, 
	const int UNIQUE_ID, const int PRIORITY) :
	m_FILENAME(FILENAME),
	m_UNIQUE_ID(UNIQUE_ID),
	m_priority(PRIORITY),
	m_active(true),
	m_sharedFile(false),
	m_renderLocation({ 0, 0, 0, 0 }),
	m_sourceLocation({ 0, 0, 0, 0 })
{
#ifdef _WIN32
	m_w32dx_file = NULL; //set later by render 2 system
#endif
}
fatcore::object_background_2::~object_background_2()
{
	m_active = false; //fuck it, better safe than corrupt

	//if this file is shared with another asset don't release it
	if (m_sharedFile) return;

#ifdef _WIN32
	if (m_w32dx_file)
	{
		m_w32dx_file->Release();
		m_w32dx_file = NULL;
	}
#endif
}
bool fatcore::object_background_2::operator<(object_background_2& comp)
{
	if (this->m_priority < comp.m_priority)
	{
		return true;
	}

	return false;
}
bool fatcore::object_background_2::operator>(object_background_2& comp)
{
	if (this->m_priority > comp.m_priority)
	{
		return true;
	}

	return false;
}

/***********************************************************************************

STATIC DATA MEMBERS - render2

***********************************************************************************/
int fatcore::render2::m_instanceCount = 0;
bool fatcore::render2::m_initialized = false;
bool fatcore::render2::m_sortForeground = true;
bool fatcore::render2::m_sortBackground = true;
fatcore::render fatcore::render2::m_LegacyRender;
std::vector<fatcore::object_foreground_2*> fatcore::render2::m_pForegroundObjects;
std::vector<fatcore::object_background_2*> fatcore::render2::m_pBackgroundObjects;
unsigned long int fatcore::render2::m_frameCount = 0;

#ifdef _WIN32
fatcore::DIRECTX_POINTERS_WIN32 fatcore::render2::m_w32dx_DirectX_Pointers;
#endif

/***********************************************************************************

MEMBER FUNCTIONS - render2

***********************************************************************************/
fatcore::render2::render2()
{
	m_instanceCount++;
}
fatcore::render2::~render2()
{
	m_instanceCount--;

	//auto cleanup
	if (m_instanceCount < 1)
	{
		m_initialized = false;

		for (unsigned int i = 0; i < m_pBackgroundObjects.size(); ++i)
		{
			if (m_pBackgroundObjects[i])
			{
				delete m_pBackgroundObjects[i];
				m_pBackgroundObjects[i] = NULL;
			}
		}
		for (unsigned int i = 0; i < m_pForegroundObjects.size(); ++i)
		{
			if (m_pForegroundObjects[i])
			{
				delete m_pForegroundObjects[i];
				m_pForegroundObjects[i] = NULL;
			}
		}
	}
}
fbc_err::CODE fatcore::render2::initialize()
{
	//check if m_legacyRender was initialized yet
	if (!m_LegacyRender.m_initialized)
	{
		//set the window handle for legacy render (w32dx)
#ifdef _WIN32

		m_LegacyRender.setWindow(m_w32dx_DirectX_Pointers.window);

#endif

		// intialize using legacy render
		int result = m_LegacyRender.initialize();
		if (result != fbc_err::OK)
		{
			return fbc_err::LEGACY_RENDER_FUNCTION_FAIL;
		}
	}

	//retrieve pointers from legacy renderer
#ifdef _WIN32
	w32dx_updateWindowsPointers();
#endif

	//initialization exicuted successfully
	m_initialized = true;
	return fbc_err::OK;
}
fbc_err::CODE fatcore::render2::render_frame()
{	
	//failsafe
	int attemptNum = 0;
	while (!m_initialized)
	{
		attemptNum++;
		if (attemptNum > 3) game_running = false;

		initialize();
		if (m_initialized) break;
	}

	//sort the assets if needed
	sort();

	//increment frame count
	m_frameCount++;

#ifdef _WIN32

	w32dx_render();

#else

	return fbc_err::UNSUPPORTED_SYSTEM;

#endif

	return fbc_err::OK;
}
void fatcore::render2::setScreenResolution(const int WIDTH, const int HEIGHT)
{
	m_LegacyRender.setResolution(WIDTH, HEIGHT);
}
fatcore::object_background_2* fatcore::render2::searchB(fatcore::object_background_2* file)
{
	for (unsigned int i = 0; i < m_pBackgroundObjects.size(); ++i)
	{
		//skip NULL pointers
		if (!m_pBackgroundObjects[i]) continue;

		//if an ID match is found, set the pointer and end search
		if ((m_pBackgroundObjects[i]->m_FILENAME == file->m_FILENAME) &&
			 m_pBackgroundObjects[i]->m_UNIQUE_ID != file->m_UNIQUE_ID)
		{
			return m_pBackgroundObjects[i];
		}
	}

	//if search completed without finding a match pass a NULL pointer back
	return NULL;
}
fatcore::object_background_2* fatcore::render2::searchB(const int UNIQUE_ID)
{
	for (unsigned int i = 0; i < m_pBackgroundObjects.size(); ++i)
	{
		//skip NULL pointers
		if (!m_pBackgroundObjects[i]) continue;
		
		//if an ID match is found, set the pointer and end search
		if (m_pBackgroundObjects[i]->m_UNIQUE_ID == UNIQUE_ID)
		{
			return m_pBackgroundObjects[i];
		}
	}

	//if search completed without finding a match pass a NULL pointer back
	return NULL;
}
fatcore::object_foreground_2* fatcore::render2::searchF(fatcore::object_foreground_2* file)
{
	for (unsigned int i = 0; i < m_pForegroundObjects.size(); ++i)
	{
		//skip NULL pointers
		if (!m_pForegroundObjects[i]) continue;

		//if an ID match is found, set the pointer and end search
		if ((m_pForegroundObjects[i]->m_FILENAME == file->m_FILENAME) &&
			(m_pForegroundObjects[i]->m_transparancyColor == file->m_transparancyColor) &&
			(m_pForegroundObjects[i]->m_UNIQUE_ID != file->m_UNIQUE_ID))
		{
			return m_pForegroundObjects[i];
		}
	}

	//if search completed without finding a match pass a NULL pointer back
	return NULL;
}
fatcore::object_foreground_2* fatcore::render2::searchF(const int UNIQUE_ID)
{
	for (unsigned int i = 0; i < m_pForegroundObjects.size(); ++i)
	{
		//skip NULL pointers
		if (!m_pForegroundObjects[i]) continue;

		//if an ID match is found, set the pointer and end search
		if (m_pForegroundObjects[i]->m_UNIQUE_ID == UNIQUE_ID)
		{
			return m_pForegroundObjects[i];
		}
	}

	//if search completed without finding a match pass a NULL pointer back
	return NULL;
}
fatcore::res fatcore::render2::getScreenResolution()
{
	return m_LegacyRender.getResolution();
}
fbc_err::CODE fatcore::render2::loadBackground(const std::string FILENAME, int& uniqueID, const int PRIORITY)
{
	//failsafe
	int attemptNum = 0;
	while (!m_initialized)
	{
		attemptNum++;
		if (attemptNum > 3) game_running = false;

		initialize();
		if (m_initialized) break;
	}

	//error checking variable
	fbc_err::CODE result;

	//tell renderer to resort
	m_sortBackground = true;

	//create a temporary pointer to an object_background2
	object_background_2* temp = NULL;

	//generate a new unique ID for the file
	uniqueID = rand();

	//verify ID is unique, and regenerate if nessissary
	temp = searchB(uniqueID);
	while (temp)
	{
		uniqueID = rand();
		temp = searchB(uniqueID);
	}

	//create the instance
	temp = new object_background_2(FILENAME, uniqueID, PRIORITY);

	//call system specific loading function
#ifdef _WIN32

	result = w32dx_load(temp);
	
	//verify load
	if (result != fbc_err::OK)
	{
		return result;
	}

#endif

	//find an unused pointer or add the new one to the end of the vector
	for (unsigned int i = 0; i < m_pBackgroundObjects.size(); ++i)
	{
		//if an unused pointer is found, assign temp to the space
		if (!m_pBackgroundObjects[i])
		{
			m_pBackgroundObjects[i] = temp;
			return fbc_err::OK;
		}
	}

	//if no unused pointers were available, add temp to the end of the vector
	m_pBackgroundObjects.push_back(temp);
	return fbc_err::OK;
}
fbc_err::CODE fatcore::render2::loadForeground(const std::string FILENAME, int& uniqueID, const int PRIORITY)
{
	//failsafe
	int attemptNum = 0;
	while (!m_initialized)
	{
		attemptNum++;
		if (attemptNum > 3) game_running = false;

		initialize();
		if (m_initialized) break;
	}

	//error checking variable
	fbc_err::CODE result;

	//tell renderer to resort
	m_sortForeground = true;

	//create a temporary pointer to an object_background2
	object_foreground_2* temp = NULL;

	//generate a new unique ID for the file
	uniqueID = rand();

	//verify ID is unique, and regenerate if nessissary
	temp = searchF(uniqueID);
	while (temp)
	{
		uniqueID = rand();
		temp = searchF(uniqueID);
	}

	//create the instance
	temp = new object_foreground_2(FILENAME, uniqueID, PRIORITY);

	//call system specific loading function
#ifdef _WIN32

	result = w32dx_load(temp);

	//verify load
	if (result != fbc_err::OK)
	{
		return result;
	}

#endif

	//find an unused pointer or add the new one to the end of the vector
	for (unsigned int i = 0; i < m_pForegroundObjects.size(); ++i)
	{
		//if an unused pointer is found, assign temp to the space
		if (!m_pForegroundObjects[i])
		{
			m_pForegroundObjects[i] = temp;
			return fbc_err::OK;
		}
	}

	//if no unused pointers were available, add temp to the end of the vector
	m_pForegroundObjects.push_back(temp);
	return fbc_err::OK;
}
fbc_err::CODE fatcore::render2::loadForeground(const std::string FILENAME, int& uniqueID, fatcore::rgb TRANSPARANCY_COLOR, const int PRIORITY)
{
	//failsafe
	int attemptNum = 0;
	while (!m_initialized)
	{
		attemptNum++;
		if (attemptNum > 3) game_running = false;

		initialize();
		if (m_initialized) break;
	}

	//error checking variable
	fbc_err::CODE result;

	//tell renderer to resort
	m_sortForeground = true;

	//create a temporary pointer to an object_background2
	object_foreground_2* temp = NULL;

	//generate a new unique ID for the file
	uniqueID = rand();

	//verify ID is unique, and regenerate if nessissary
	temp = searchF(uniqueID);
	while (temp)
	{
		uniqueID = rand();
		temp = searchF(uniqueID);
	}

	//create the instance
	temp = new object_foreground_2(FILENAME, uniqueID, PRIORITY, TRANSPARANCY_COLOR);

	//call system specific loading function
#ifdef _WIN32

	result = w32dx_load(temp);

	//verify load
	if (result != fbc_err::OK)
	{
		return result;
	}

#endif

	//find an unused pointer or add the new one to the end of the vector
	for (unsigned int i = 0; i < m_pForegroundObjects.size(); ++i)
	{
		//if an unused pointer is found, assign temp to the space
		if (!m_pForegroundObjects[i])
		{
			m_pForegroundObjects[i] = temp;
			return fbc_err::OK;
		}
	}

	//if no unused pointers were available, add temp to the end of the vector
	m_pForegroundObjects.push_back(temp);
	return fbc_err::OK;
}
fbc_err::CODE fatcore::render2::purge(const fatcore::assetTypes ASSET_TYPE, const int UNIQUE_ID)
{
	//failsafe
	int attemptNum = 0;
	while (!m_initialized)
	{
		attemptNum++;
		if (attemptNum > 3) game_running = false;

		initialize();
		if (m_initialized) break;
	}

	//determine the asset type and search the appropriate vector
	if (ASSET_TYPE == BKGD)
	{
		object_background_2* file = NULL;
		file = searchB(UNIQUE_ID);
		if (!file)
		{
			return fbc_err::ID_NOT_FOUND;
		}

		//search for other assets using the same file (and transparancy key for FRGD assets)
		object_background_2* sharedFile = searchB(file);
		
		//flag the shared file bool as true if a match was found
		if (sharedFile)
		{
			file->m_sharedFile = true;
		}
		else
		{
			file->m_sharedFile = false;
		}

		for (unsigned int i = 0; i < m_pBackgroundObjects.size(); ++i)
		{
			if (!m_pBackgroundObjects[i]) continue;

			if (m_pBackgroundObjects[i]->m_UNIQUE_ID == file->m_UNIQUE_ID)
			{
				//delete the asset from memory
				delete file;
				file = NULL;

				//set the pointer to NULL
				m_pBackgroundObjects[i] = NULL;

				//break search loop
				break;
			}
		}
	}
	else if (ASSET_TYPE == FRGD)
	{
		object_foreground_2* file = searchF(UNIQUE_ID);
		if (!file)
		{
			return fbc_err::ID_NOT_FOUND;
		}

		//search for other assets using the same file (and transparancy key for FRGD assets)
		object_foreground_2* sharedFile = searchF(file);

		//flag the shared file bool as true if a match was found
		if (sharedFile)
		{
			file->m_sharedFile = true;
		}
		else
		{
			file->m_sharedFile = false;
		}

		for (unsigned int i = 0; i < m_pForegroundObjects.size(); ++i)
		{
			if (!m_pForegroundObjects[i]) continue;

			if (m_pForegroundObjects[i]->m_UNIQUE_ID == file->m_UNIQUE_ID)
			{
				//delete the asset from memory
				delete file;
				file = NULL;

				//set the pointer to NULL
				m_pForegroundObjects[i] = NULL;

				//break search loop
				break;
			}
		}
	}
	
	//if all went well, return success code
	return fbc_err::OK;
}
fbc_err::CODE fatcore::render2::setRenderLoc(const fatcore::assetTypes ASSET_TYPE, const int UNIQUE_ID, const fatcore::loc NEW_LOCATION)
{
	//failsafe
	int attemptNum = 0;
	while (!m_initialized)
	{
		attemptNum++;
		if (attemptNum > 3) game_running = false;

		initialize();
		if (m_initialized) break;
	}

	//determine asset type
	if (ASSET_TYPE == fatcore::BKGD)
	{
		//find search for the asset
		object_background_2* file = searchB(UNIQUE_ID);

		//verify file was located
		if (!file) //file was not found
		{
			return fbc_err::ID_NOT_FOUND;
		}

		//file was found, set location
		file->m_renderLocation = NEW_LOCATION;
	}
	else if (ASSET_TYPE == fatcore::FRGD)
	{
		//find search for the asset
		object_foreground_2* file = searchF(UNIQUE_ID);

		//verify file was located
		if (!file) //file was not found
		{
			return fbc_err::ID_NOT_FOUND;
		}

		//file was found, set location
		file->m_renderLocation = NEW_LOCATION;
	}
	else
	{
		//how in the ACTUAL FUCK DID YOU GET HERE!!!!
		return fbc_err::GENERIC_FAILURE;
	}

	//no errors
	return fbc_err::OK;
}
fbc_err::CODE fatcore::render2::setSourceLoc(const fatcore::assetTypes ASSET_TYPE, const int UNIQUE_ID, const fatcore::loc NEW_LOCATION)
{
	//failsafe
	int attemptNum = 0;
	while (!m_initialized)
	{
		attemptNum++;
		if (attemptNum > 3) game_running = false;

		initialize();
		if (m_initialized) break;
	}

	//determine asset type
	if (ASSET_TYPE == fatcore::BKGD)
	{
		//find search for the asset
		object_background_2* file = searchB(UNIQUE_ID);

		//verify file was located
		if (!file) //file was not found
		{
			return fbc_err::ID_NOT_FOUND;
		}

		//file was found, set location
		file->m_sourceLocation = NEW_LOCATION;
	}
	else if (ASSET_TYPE == fatcore::FRGD)
	{
		//search for the asset
		object_foreground_2* file = searchF(UNIQUE_ID);

		//verify file was located
		if (!file) //file was not found
		{
			return fbc_err::ID_NOT_FOUND;
		}

		//file was found, set location
		file->m_sourceLocation = NEW_LOCATION;
	}
	else
	{
		//how in the ACTUAL FUCK DID YOU GET HERE!!!!
		return fbc_err::GENERIC_FAILURE;
	}

	//no errors
	return fbc_err::OK;
}
fbc_err::CODE fatcore::render2::setPriority(const fatcore::assetTypes ASSET_TYPE, const int UNIQUE_ID, const int NEW_PRIORITY)
{
	//failsafe
	int attemptNum = 0;
	while (!m_initialized)
	{
		attemptNum++;
		if (attemptNum > 3) game_running = false;

		initialize();
		if (m_initialized) break;
	}

	//determine asset type
	if (ASSET_TYPE == fatcore::BKGD)
	{
		//find search for the asset
		object_background_2* file = searchB(UNIQUE_ID);

		//verify file was located
		if (!file) //file was not found
		{
			return fbc_err::ID_NOT_FOUND;
		}

		//file was found, set location
		file->m_priority = NEW_PRIORITY;

		//tell renderer to resort
		m_sortBackground = true;
	}
	else if (ASSET_TYPE == fatcore::FRGD)
	{
		//find search for the asset
		object_foreground_2* file = searchF(UNIQUE_ID);

		//verify file was located
		if (!file) //file was not found
		{
			return fbc_err::ID_NOT_FOUND;
		}

		//file was found, set location
		file->m_priority = NEW_PRIORITY;

		//tell renderer to resort
		m_sortForeground = true;
	}
	else
	{
		//how in the ACTUAL FUCK DID YOU GET HERE!!!!
		return fbc_err::GENERIC_FAILURE;
	}

	//no errors
	return fbc_err::OK;
}
void fatcore::render2::setActivationStatus(const fatcore::assetTypes ASSET_TYPE, const int UNIQUE_ID, const bool ACTIVATION_STATUS)
{
	//failsafe
	int attemptNum = 0;
	while (!m_initialized)
	{
		attemptNum++;
		if (attemptNum > 3) game_running = false;

		initialize();
		if (m_initialized) break;
	}

	//determine asset type
	if (ASSET_TYPE == fatcore::BKGD)
	{
		//find search for the asset
		object_background_2* file = searchB(UNIQUE_ID);

		//verify file was located
		if (!file) //file was not found
		{
			return;
		}

		//file was found, set location
		file->m_active = ACTIVATION_STATUS;
	}
	else if (ASSET_TYPE == fatcore::FRGD)
	{
		//find search for the asset
		object_foreground_2* file = searchF(UNIQUE_ID);

		//verify file was located
		if (!file) //file was not found
		{
			return;
		}

		//file was found, set location
		file->m_active = ACTIVATION_STATUS;
	}
	else
	{
		//how in the ACTUAL FUCK DID YOU GET HERE!!!!
		return;
	}
}
void fatcore::render2::sort()
{

	while (m_sortBackground)
	{
		//skip if no assets are loaded
		if (m_pBackgroundObjects.size() < 1)
		{
			m_sortBackground = false;
			break;
		}

		//sorting sub loop
		for (unsigned int i = 0; i < m_pBackgroundObjects.size(); ++i)
		{
			//ignore NULL pointers
			if (!m_pBackgroundObjects[i]) continue;
			int j = i + 1;
			if (j == m_pBackgroundObjects.size()) continue;
			if (!m_pBackgroundObjects[j])
			{
				j++;
				if (!m_pBackgroundObjects[j]) continue;
			}

			//check priority and adjust if needed
			if (m_pBackgroundObjects[i]->m_priority > m_pBackgroundObjects[j]->m_priority)
			{
				object_background_2* temp = m_pBackgroundObjects[i];
				m_pBackgroundObjects[i] = m_pBackgroundObjects[j];
				m_pBackgroundObjects[j] = temp;
			}
		}

		//break loop if sorting is complete
		bool keepSorting = false;
		for (unsigned int i = 0; i < m_pBackgroundObjects.size(); ++i)
		{
			if (!m_pBackgroundObjects[i]) continue;
			for (unsigned int j = m_pBackgroundObjects.size() - 1; j >= 1; --j)
			{
				if (!m_pBackgroundObjects[j]) continue;
				if (j == i) break;

				if (m_pBackgroundObjects[i]->m_priority > m_pBackgroundObjects[j]->m_priority)
				{
					keepSorting = true;
				}
			}
		}

		m_sortBackground = keepSorting;
	}

	//main sorting loop
	while (m_sortForeground)
	{
		//skip if no assets are loaded
		if (m_pForegroundObjects.size() < 1)
		{
			m_sortForeground = false;
			break;
		}

		//sorting sub loop
		for (unsigned int i = 0; i < m_pForegroundObjects.size(); ++i)
		{
			//ignore NULL pointers
			if (!m_pForegroundObjects[i]) continue;
			int j = i + 1;
			if (j == m_pForegroundObjects.size()) continue;
			if (!m_pForegroundObjects[j])
			{
				j++;
				if (j == m_pForegroundObjects.size()) continue;
				if (!m_pForegroundObjects[j]) continue;
			}

			//check priority and adjust if needed
			if (m_pForegroundObjects[i]->m_priority > m_pForegroundObjects[j]->m_priority)
			{
				object_foreground_2* temp = m_pForegroundObjects[i];
				m_pForegroundObjects[i] = m_pForegroundObjects[j];
				m_pForegroundObjects[j] = temp;
			}
		}

		//break loop if sorting is complete
		bool keepSorting = false;
		for (unsigned int i = 0; i < m_pForegroundObjects.size(); ++i)
		{
			if (!m_pForegroundObjects[i]) continue;
			for (unsigned int j = m_pForegroundObjects.size() - 1; j >= 1; --j)
			{
				if (!m_pForegroundObjects[j]) continue;
				if (j == i) break;

				if (m_pForegroundObjects[i]->m_priority > m_pForegroundObjects[j]->m_priority)
				{
					keepSorting = true;
				}
			}
		}

		m_sortForeground = keepSorting;
	}
}

fbc_err::CODE fatcore::render2::setRenderColor(const int UNIQUE_ID, const rgb NEW_COLOR)
{
	//failsafe
	int attemptNum = 0;
	while (!m_initialized)
	{
		attemptNum++;
		if (attemptNum > 3) game_running = false;

		initialize();
		if (m_initialized) break;
	}

	//search for the asset
	object_foreground_2* file = searchF(UNIQUE_ID);

	//verify file was found
	if (!file) return fbc_err::ID_NOT_FOUND;

	//change setting
	file->m_renderColor = NEW_COLOR;
}
fbc_err::CODE fatcore::render2::setScaleX(const int UNIQUE_ID, const float SCALE)
{
	//failsafe
	int attemptNum = 0;
	while (!m_initialized)
	{
		attemptNum++;
		if (attemptNum > 3) game_running = false;

		initialize();
		if (m_initialized) break;
	}

	//search for the asset
	object_foreground_2* file = searchF(UNIQUE_ID);

	//verify file was found
	if (!file) return fbc_err::ID_NOT_FOUND;

	//change setting
	file->m_scaleX = SCALE;
}
fbc_err::CODE fatcore::render2::setScaleY(const int UNIQUE_ID, const float SCALE)
{
	//failsafe
	int attemptNum = 0;
	while (!m_initialized)
	{
		attemptNum++;
		if (attemptNum > 3) game_running = false;

		initialize();
		if (m_initialized) break;
	}

	//search for the asset
	object_foreground_2* file = searchF(UNIQUE_ID);

	//verify file was found
	if (!file) return fbc_err::ID_NOT_FOUND;

	//change setting
	file->m_scaleX = SCALE;
}
fbc_err::CODE fatcore::render2::setRotation(const int UNIQUE_ID, const float ROTATION)
{
	//failsafe
	int attemptNum = 0;
	while (!m_initialized)
	{
		attemptNum++;
		if (attemptNum > 3) game_running = false;

		initialize();
		if (m_initialized) break;
	}

	//search for the asset
	object_foreground_2* file = searchF(UNIQUE_ID);

	//verify file was found
	if (!file) return fbc_err::ID_NOT_FOUND;

	//change setting
	file->m_rotation = ROTATION;
}

/***********************************************************************************

WINDOWS/DIRECTX MEMBER FUNCTIONS - render2

***********************************************************************************/
#ifdef _WIN32

//friend function, not part of render2 class
void fatcore::w32dx_configWindow(render2* rDev, HWND window)
{
	rDev->m_w32dx_DirectX_Pointers.window = window;
}

fbc_err::CODE fatcore::render2::w32dx_render()
{
	//local pointers
	LPDIRECT3DDEVICE9 localDevice = m_w32dx_DirectX_Pointers.device; 
	LPDIRECT3DSURFACE9 localBackbuffer = m_w32dx_DirectX_Pointers.backbuffer;
	LPD3DXSPRITE localSprite = m_w32dx_DirectX_Pointers.sprite;
	
	//prepair rendering
	if (localDevice->BeginScene())
	{
		localDevice->ColorFill(localBackbuffer, NULL, D3DCOLOR_XRGB(0, 0, 0));
		localSprite->Begin(D3DXSPRITE_ALPHABLEND);

		//render legacy background first (lowest priority)
		m_LegacyRender.renderWIN32(fatcore::BKGD);

		//render background2 assets by priority
		for (unsigned int i = 0; i < m_pBackgroundObjects.size(); ++i)
		{
			//skip any NULL pointers
			if (!m_pBackgroundObjects[i]) continue;

			//get local values
			object_background_2* temp = m_pBackgroundObjects[i];
			loc tempSource = temp->m_sourceLocation;
			loc tempDest = temp->m_renderLocation;

			//convert fatcore universal loc to windows RECT
			RECT source = { tempSource.x, tempSource.y, tempSource.width, tempSource.height };
			RECT dest = { tempDest.x, tempDest.y, tempDest.width, tempDest.height };

			//render using stretch rect
			localDevice->StretchRect(temp->m_w32dx_file, &source, localBackbuffer, &dest, D3DTEXF_NONE);
		}

		//render legacy foreground first (lowest priority)
		m_LegacyRender.renderWIN32(fatcore::FRGD);

		int lastPriority = 0;
		bool textRendered = false;
		//render foreground2 assets by priority
		for (unsigned int i = 0; i < m_pForegroundObjects.size(); ++i)
		{
			//skip any NULL pointers
			if (!m_pForegroundObjects[i]) continue;

			//get local values
			object_foreground_2* temp = m_pForegroundObjects[i];
			loc tempSource = temp->m_sourceLocation;
			
			//treat text boxes as priority 1000
			if (temp->m_priority >= 1000 && !textRendered)
			{
				text_render tRen;
				tRen.render();
				textRendered = true;
			}

			//convert fatcore universal loc to windows RECT
			RECT source = { tempSource.x, tempSource.y, tempSource.width, tempSource.height };

			D3DXVECTOR2 scale(temp->m_scaleX, temp->m_scaleY);
			D3DXVECTOR2 trans(temp->m_renderLocation.x, temp->m_renderLocation.y);
			D3DXVECTOR2 center(static_cast<float>((temp->m_renderLocation.width - temp->m_renderLocation.x) * temp->m_scaleX) / 2,
							   static_cast<float>((temp->m_renderLocation.height - temp->m_renderLocation.y) * temp->m_scaleY) / 2);

			D3DXMATRIX mat;
			D3DXMatrixTransformation2D(&mat, NULL, 0, &scale, &center, temp->m_rotation, &trans);
			localSprite->SetTransform(&mat);

			rgb tempColor = temp->m_renderColor;
			D3DCOLOR renderColor = D3DCOLOR_XRGB(tempColor.r, tempColor.g, tempColor.b);

			localSprite->Draw(temp->m_w32dx_file, &source, NULL, NULL, renderColor);
			
			lastPriority = temp->m_priority;
		}

		if (!textRendered)
		{
			text_render tRen;
			tRen.render();
		}

		//present scene
		localSprite->End();
		localDevice->EndScene();
		localDevice->Present(NULL, NULL, NULL, NULL);
	}


	return fbc_err::OK;
}
fbc_err::CODE fatcore::render2::w32dx_load(fatcore::object_background_2* file)
{
	//failsafe
	int attemptNum = 0;
	while (!m_initialized)
	{
		attemptNum++;
		if (attemptNum > 3) game_running = false;

		initialize();
		if (m_initialized) break;
	}

	//verify file has not already been loaded and copy it if it has
	object_background_2* dupFile = searchB(file);

	if (dupFile)
	{
		file->m_w32dx_file = dupFile->m_w32dx_file;
		return fbc_err::OK;
	}
	
	//error checking
	HRESULT result;

	//get image dimensions from file
	D3DXIMAGE_INFO info;
	result = D3DXGetImageInfoFromFile(file->m_FILENAME.c_str(), &info);
	if (result != D3D_OK) return fbc_err::FILE_LOAD_FAIL;

	//copy info into the source location member
	file->m_sourceLocation.set( 0, 0, info.Width, info.Height );

	//create the surface3
	result = m_w32dx_DirectX_Pointers.device->CreateOffscreenPlainSurface(info.Width, info.Height, D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT, &file->m_w32dx_file, NULL);

	//load image from file
	result = D3DXLoadSurfaceFromFile(file->m_w32dx_file, NULL, NULL, file->m_FILENAME.c_str(), NULL, D3DX_DEFAULT,
		D3DCOLOR_XRGB(0, 0, 0), NULL);
	if (result != D3D_OK) return fbc_err::FILE_LOAD_FAIL;

	//no errors
	return fbc_err::OK;
}
fbc_err::CODE fatcore::render2::w32dx_load(fatcore::object_foreground_2* file)
{
	//failsafe
	int attemptNum = 0;
	while (!m_initialized)
	{
		attemptNum++;
		if (attemptNum > 3) game_running = false;

		initialize();
		if (m_initialized) break;
	}

	//if file is already loaded, make a copy instead
	object_foreground_2* dupFile = searchF(file);
	if (dupFile)
	{
		file->m_w32dx_file = dupFile->m_w32dx_file;
		return fbc_err::OK;
	}

	//windows error checking
	HRESULT result;

	//get file width and height
	D3DXIMAGE_INFO info;
	result = D3DXGetImageInfoFromFile(file->m_FILENAME.c_str(), &info);
	if (result != D3D_OK) return fbc_err::FILE_LOAD_FAIL;

	//create texture by loading from file
	file->m_w32dx_file = NULL;
	D3DCOLOR transColor = D3DCOLOR_XRGB(file->m_transparancyColor.r, 
										file->m_transparancyColor.g, 
										file->m_transparancyColor.b);

	result = D3DXCreateTextureFromFileEx(m_w32dx_DirectX_Pointers.device, file->m_FILENAME.c_str(), info.Width, info.Height, 1, D3DPOOL_DEFAULT,
		D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, transColor, &info, NULL, &file->m_w32dx_file);
	if (result != D3D_OK) return fbc_err::FILE_LOAD_FAIL;

	//success
	return fbc_err::OK;
}
void fatcore::render2::w32dx_updateWindowsPointers()
{
	m_w32dx_DirectX_Pointers.backbuffer = m_LegacyRender.m_backbuffer;
	m_w32dx_DirectX_Pointers.d3d = m_LegacyRender.m_d3d;
	m_w32dx_DirectX_Pointers.device = m_LegacyRender.m_device;
	m_w32dx_DirectX_Pointers.sprite = m_LegacyRender.m_sprite;
	m_w32dx_DirectX_Pointers.window = m_LegacyRender.m_window;
}

#endif

/***********************************************************************************

PRIMARY INSTANCE - render2

***********************************************************************************/
fatcore::render2 __PRIMARY_RENDER_2_INSTANCE__;