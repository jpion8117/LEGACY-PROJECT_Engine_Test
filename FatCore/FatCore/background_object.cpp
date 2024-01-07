/***************************************************************************************************

FatBoy Engine -- Render  -- background object deffinitions declared on L57 of render.h

Purpose: The basic rendering classes in this file will allow for all rendering to be abstracted
there for allowing easy porting to other systems.

background object Definitions
fatcore:object_background

***************************************************************************************************/

#include "render.h"
#include <cstdlib>
#include <ctime> 

fatcore::object_background::object_background(std::string filename, std::string callname, int& uniqueID)
{
	//generate ID and pass it back to the calling function
	uniqueID = generateID();

	//initialize variables
	m_filename = filename;
	m_callname = callname;
	m_failure = true;
	m_UnloadTexture = true;
	m_active = true;

	//system specific calls
#ifdef _WIN32
	bool success = loadWin32();
	if (success) { m_failure = false; }
#endif
}
fatcore::object_background::~object_background()
{
	//skip unloading of file(s) if requested by render management system
	if (!m_UnloadTexture) return;

	//system specific calls
#ifdef _WIN32
	releaseSurface();
#endif
}
std::string fatcore::object_background::getCallname()	
{
	return m_callname;	
}
int fatcore::object_background::getID()
{
	return m_ID;
}
void fatcore::object_background::render()
{
	//skip rendering if unactive
	if (!m_active) return;

	//Windows with DirectX
#ifdef _WIN32
	renderWIN32();
#endif
}
void fatcore::object_background::setRenderLoc(loc location)
{
	m_render_location = location;
}
void fatcore::object_background::setSourceLoc(loc location)
{
	m_source_location = location;
}
int fatcore::object_background::generateID()
{
	//generate a random ID number (conflicts will be handled by render engine)
	m_ID = rand();

	//return the new ID number
	return m_ID;
}
void fatcore::object_background::keepTexture()
{
	m_UnloadTexture = false;
}
//void fatcore::object_background::activate() { m_active = true; }
//void fatcore::object_background::deactivate() { m_active = false; }

/***************************************************************************************************

Windows related member functions and data members

***************************************************************************************************/

bool fatcore::object_background::loadWin32()
{
	//if called from any other system return false
#ifndef _WIN32
	return false;
#endif

	//create a render device to gain access to the D3D and windows objects
	fatcore::render device;
	LPDIRECT3DDEVICE9 d3ddev = device.getDevice();

	//get image dimensions from file
	D3DXIMAGE_INFO info;
	HRESULT result;
	result = D3DXGetImageInfoFromFile(m_filename.c_str(), &info);
	if (result != D3D_OK) { return false; }

	//copy the info into the m_source_location
	m_source_location.set(0, 0, info.Width, info.Height);

	//create the surface
	result = d3ddev->CreateOffscreenPlainSurface(info.Width, info.Height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_surface, NULL);
	if (result != D3D_OK) { return false; }

	//load surface from file into newly created surface
	result = D3DXLoadSurfaceFromFile(m_surface, NULL, NULL, m_filename.c_str(), NULL, D3DX_DEFAULT, D3DCOLOR_XRGB(0, 0, 0), NULL);
	if (result != D3D_OK) { return false; }

	return true;
}
bool fatcore::object_background::releaseSurface()
{
	//return false if called from any other system other than windows
#ifndef _WIN32
	return false;
#endif
	m_surface->Release();

	return true;
}
void fatcore::object_background::renderWIN32()
{
	//return without completeing if function is run on non windows os
#ifndef _WIN32
	return;
#endif

	//get the nessissary pointer(s) from main rendering object
	fatcore::render device;
	LPDIRECT3DDEVICE9 d3ddev = device.getDevice();
	LPDIRECT3DSURFACE9 backbuffer;
	HRESULT result = d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	if (result != D3D_OK) return;

	//create RECT structs for source and dest locations
	RECT source = { m_source_location.x, m_source_location.y, m_source_location.width, m_source_location.height };
	RECT dest = { m_render_location.x, m_render_location.y, m_render_location.width, m_render_location.height };

	//render using strech rect
	d3ddev->StretchRect(m_surface, &source, backbuffer, &dest, D3DTEXF_NONE);
}