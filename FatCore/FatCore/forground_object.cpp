/***************************************************************************************************
FatBoy Engine -- Render  -- foreground object deffinitions declared on l33 of render.h

Purpose: The basic rendering classes in this file will allow for all rendering to be abstracted
there for allowing easy porting to other systems.

foreground object Definitions
fatcore:object_foreground

***************************************************************************************************/

#include "render.h"
#include <cstdlib>
#include <ctime>

fatcore::object_foreground::object_foreground(std::string filename, std::string callname, rgb transColor, int& uniqueID, const bool TOP_LEVEL)
{
	//generate a uniqueID to register this texture and pass it back to the function creating it
	uniqueID = generateID();
	
	//initialize data members
	m_filename = filename;
	m_callname = callname;
	m_transColor = transColor;
	m_renderColor = fatcore::makeColor(255, 255, 255);
	m_failure = true;
	m_active = true;
	m_topLevel = TOP_LEVEL;
	
	//load the file into memory
#ifdef _WIN32 //Windows with directX
	bool success = loadWin32(filename, callname, D3DCOLOR_XRGB(transColor.r, transColor.g, transColor.b));
	if (success)	{	m_failure = false;	}
#endif
}
fatcore::object_foreground::object_foreground(std::string filename, std::string callname, int& uniqueID, const bool TOP_LEVEL)
{
	//generate a uniqueID to register this texture and pass it back to the function creating it
	uniqueID = generateID();
	
	//initialize data members
	m_filename = filename;
	m_callname = callname;
	m_transColor = makeColor(0, 0, 0);
	m_renderColor = fatcore::makeColor(255, 255, 255);
	m_failure = true;
	m_active = true;
	m_topLevel = TOP_LEVEL;
	
	//load the file into memory
#ifdef _WIN32 //Windows with directX
	bool success = loadWin32(filename, callname, D3DCOLOR_XRGB(0,0,0));
	if (success)	{	m_failure = false;	}
#endif
}
fatcore::object_foreground::~object_foreground()
{
	//skip unloading of file(s) if requested by render management system
	if (!m_unloadTexture) return;

	//release the direct3D texture (windows systems)
#ifdef _WIN32
	bool success = releaseTexture();
	if (!success)	{ m_failure = true; }
#endif
}
std::string fatcore::object_foreground::getCallname()
{
	return m_callname;
}
void fatcore::object_foreground::render(const bool topLevel)
{
	if (!m_active) return;

	//Windows with DirectX
#ifdef _WIN32
	if (m_topLevel == topLevel) renderWIN32();
#endif
}
void fatcore::object_foreground::setRenderLoc(loc location)
{
	m_render_location = location;
}
void fatcore::object_foreground::setSourceLoc(loc location)
{
	m_source_location = location;
}
void fatcore::object_foreground::setColor(rgb color)
{
	m_renderColor = color;
}
int fatcore::object_foreground::generateID()
{
	//generate a random ID number (conflicts will be handled by render engine)
	m_ID = rand();

	//return the new ID number
	return m_ID;
}
int fatcore::object_foreground::getID()
{
	return m_ID;
}
void fatcore::object_foreground::keepTexture()
{
	m_unloadTexture = false;
}
void fatcore::object_foreground::setTopLevel(const bool TOP_LEVEL)
{
	m_topLevel = TOP_LEVEL;
}

/*************************************************************************************************************

Windows related member functions and data members

*************************************************************************************************************/

bool fatcore::object_foreground::loadWin32(std::string filename, std::string callname, D3DCOLOR transColor)
{
#ifndef _WIN32
	return false;
#endif
	//create a render device to gain access to all the nessissary D3D objects needed
	fatcore::render device;

	//get file width and height
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);

	//Create texture by loading from file
	m_Texture = NULL;
	D3DXCreateTextureFromFileEx(device.getDevice(), filename.c_str(), info.Width, info.Height, 1, D3DPOOL_DEFAULT, 
		D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, transColor, &info, NULL, &m_Texture);
	if (result != D3D_OK) return false;

	//if image loaded correctly return true
	return true;
}
bool fatcore::object_foreground::releaseTexture()
{
#ifndef _WIN32
	return false;
#endif

	if (m_Texture == NULL)	return true;

	m_Texture->Release();
	m_Texture = NULL;

	return true;
}
void fatcore::object_foreground::renderWIN32()
{
	//verify texture and trigger failure if nessissary
	if (m_Texture == NULL)
	{
		m_failure = true;
		return;
	}

	//create render object to access sprite pointer
	fatcore::render device;
	LPD3DXSPRITE sprite = device.getSprite();

	//render object to set cords
	D3DXVECTOR3 position((float)m_render_location.x, (float)m_render_location.y, 0);
	RECT source = { m_source_location.x, m_source_location.y, m_source_location.width, m_source_location.height };
	sprite->Draw(m_Texture, &source, NULL, &position, D3DCOLOR_XRGB(m_renderColor.r, m_renderColor.g, m_renderColor.b));
}
