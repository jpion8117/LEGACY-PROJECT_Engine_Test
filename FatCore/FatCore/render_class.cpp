/***************************************************************************************************
FatBoy Engine -- Render  -- render class deffinitions declared on L81 of render.h

Purpose: The basic rendering classes in this file will allow for all rendering to be abstracted
there for allowing easy porting to other systems.

render class definitions
fatcore:render

***************************************************************************************************/

#include "universal_includes.h"
#include "text_rendering.h"
#include "errors.h"
#include "debug.h"
#include "core.h"

//base file
#include "render.h"

/*****************************************************
static data member initilizations
*****************************************************/
bool fatcore::render::m_initialized = false;
std::vector<fatcore::object_foreground> fatcore::render::m_foregroundObjects;
std::vector<fatcore::object_background> fatcore::render::m_backgroundObjects;
int fatcore::render::m_deviceCount = 0;
fatcore::res fatcore::render::m_resolution;

//windows and directX specific data members
#ifdef _WIN32

HWND fatcore::render::m_window;
LPDIRECT3D9 fatcore::render::m_d3d = NULL;
LPDIRECT3DDEVICE9 fatcore::render::m_device = NULL;
LPD3DXSPRITE fatcore::render::m_sprite = NULL;
LPDIRECT3DSURFACE9 fatcore::render::m_backbuffer = NULL;

#endif

/*****************************************************
member function deffinitions
*****************************************************/
fatcore::render::render()
{
	//incriment the device count
	m_deviceCount++;
}
fatcore::render::~render()
{
	//deincriment the device count
	m_deviceCount--;

	//if __PRIMARY_RENDER_DEVICE__ is deleted prepair for shutdown
	if (m_deviceCount < 1)
	{
		shutdown();

		//set m_initialized to false to avoid accidental use after uninitialization has occured
		m_initialized = false;
	}
}
int fatcore::render::initialize()
{
	//create a result variable for error checking
	int result = 0;

	//check OS and call the appropriate initialization function(s)
#ifdef _WIN32 //Windows using DirectX
	result = initDirect3D();
#else
	return fbc_err::INIT_FAIL | fbc_err::UNSUPORTED_SYSTEM;
#endif

	if (result != fbc_err::OK) //initialization failed
	{
		std::cout << "[WARNING:] Render device initialization failed" << std::endl;
		return result;
	}

	//initialization was successful
	m_initialized = true; //system is now ready to render assets to the screen
	std::cout << "[INIT:] Render device initialzation complete." << std::endl;
	return fbc_err::OK;
}
void fatcore::render::shutdown()
{
	//check OS and call the appropriate startup function(s)
#ifdef _WIN32 //Windows using DirectX
	shutdownDirect3D();
#endif
}
int fatcore::render::render_scene()
{
	//check os and run appropriate render function
#ifdef _WIN32
	renderWIN32();
#else
	return fbc_err::UNSUPPORTED_SYSTEM;
#endif

	return fbc_err::OK;
}

//asset managment functions
int fatcore::render::setRenderLoc(const int ASSET_TYPE, const int UNIQUE_ID, const loc NEW_LOCATION)
{
	//ID search loop
	if (ASSET_TYPE == BKGD)
	{
		for (unsigned int i = 0; i < m_backgroundObjects.size(); ++i)
		{
			if (m_backgroundObjects[i].getID() == UNIQUE_ID)
			{
				m_backgroundObjects[i].setRenderLoc(NEW_LOCATION);
				return fbc_err::OK;
			}
		}
	}
	else if (ASSET_TYPE == FRGD)
	{
		for (unsigned int i = 0; i < m_foregroundObjects.size(); ++i)
		{
			if (m_foregroundObjects[i].getID() == UNIQUE_ID)
			{
				m_foregroundObjects[i].setRenderLoc(NEW_LOCATION);
				return fbc_err::OK;
			}
		}
	}

	//return error code if id not found
	return fbc_err::ID_NOT_FOUND;
}
int fatcore::render::setSourceLoc(const int ASSET_TYPE, const int UNIQUE_ID, const loc NEW_LOCATION)
{
	//ID search loop
	if (ASSET_TYPE == BKGD)
	{
		for (unsigned int i = 0; i < m_backgroundObjects.size(); ++i)
		{
			if (m_backgroundObjects[i].getID() == UNIQUE_ID)
			{
				m_backgroundObjects[i].setSourceLoc(NEW_LOCATION);
				return fbc_err::OK;
			}
		}
	}
	else if (ASSET_TYPE == FRGD)
	{
		for (unsigned int i = 0; i < m_foregroundObjects.size(); ++i)
		{
			if (m_foregroundObjects[i].getID() == UNIQUE_ID)
			{
				m_foregroundObjects[i].setSourceLoc(NEW_LOCATION);
				return fbc_err::OK;
			}
		}
	}

	//return error code if id not found
	return fbc_err::ID_NOT_FOUND;
}
int fatcore::render::setRenderColor(const int UNIQUE_ID, const rgb NEW_COLOR)
{
	//ID search loop
	for (unsigned int i = 0; i < m_foregroundObjects.size(); ++i)
	{
		if (m_foregroundObjects[i].getID() == UNIQUE_ID)
		{
			m_foregroundObjects[i].setColor(NEW_COLOR);
			return fbc_err::OK;
		}
	}

	//return error code if id not found
	return fbc_err::ID_NOT_FOUND;
}
void fatcore::render::setResolution(const int WIDTH, const int HEIGHT)
{
	m_resolution.set(HEIGHT, WIDTH);
}
fatcore::res fatcore::render::getResolution()
{
	return m_resolution;
}
int fatcore::render::loadBackground(const std::string FILENAME, const std::string CALLNAME, int& uniqueID)
{
	//create a temp background object and set it to keep its texture after deletion
	fatcore::object_background temp(FILENAME, CALLNAME, uniqueID);
	temp.keepTexture();

	//verify unique id is not taken and regenerate if nessissary
	unsigned int i = 0;
	while (i < m_backgroundObjects.size())
	{
		if (m_backgroundObjects[i].getID() == uniqueID)
		{
			//generate new ID
			m_backgroundObjects[i].generateID();

			//start search over
			i = 0;
			continue;
		}

		i++;
	}

	//add temp to the background objects vector
	m_backgroundObjects.push_back(temp);

	return fbc_err::OK;
}
int fatcore::render::loadforeground(const std::string FILENAME, const std::string CALLNAME, int& uniqueID, const bool TOP_LEVEL)
{
	//create a temp foreground object and set it to keep its texture after deletion
	fatcore::object_foreground temp(FILENAME, CALLNAME, uniqueID);
	temp.keepTexture();
	temp.setTopLevel(TOP_LEVEL);

	//verify unique id is not taken and regenerate if nessissary
	unsigned int i = 0;
	while (i < m_foregroundObjects.size())
	{
		if (m_foregroundObjects[i].getID() == uniqueID)
		{
			//generate new ID
			m_foregroundObjects[i].generateID();

			//start search over
			i = 0;
			continue;
		}

		i++;
	}

	//add temp to the background objects vector
	m_foregroundObjects.push_back(temp);

	return fbc_err::OK;
}
int fatcore::render::loadforeground(const std::string FILENAME, const std::string CALLNAME, const rgb TRANS_COLOR, int& uniqueID, const bool TOP_LEVEL)
{
	//create a temp foreground object and set it to keep its texture after deletion
	fatcore::object_foreground temp(FILENAME, CALLNAME, TRANS_COLOR, uniqueID);
	temp.keepTexture();
	temp.setTopLevel(TOP_LEVEL);

	//verify unique id is not taken and regenerate if nessissary
	unsigned int i = 0;
	while (i < m_foregroundObjects.size())
	{
		if (m_foregroundObjects[i].getID() == uniqueID)
		{
			//generate new ID
			m_foregroundObjects[i].generateID();

			//start search over
			i = 0;
			continue;
		}

		i++;
	}

	//add temp to the background objects vector
	m_foregroundObjects.push_back(temp);

	return fbc_err::OK;
}
int fatcore::render::copy(const int ASSET_TYPE, const std::string CALLNAME, int& uniqueID)
{
	//determine asset type and begin search loop
	if (ASSET_TYPE == BKGD)
	{
		for (unsigned int i = 0; i < m_backgroundObjects.size(); ++i)
		{
			if (CALLNAME == m_backgroundObjects[i].getCallname())
			{
				//create temp object for copy
				fatcore::object_background temp = m_backgroundObjects[i];

				//generate new uniqueID for the copied texture
				temp.generateID();
				
				unsigned int y = 0;
				while (y < m_backgroundObjects.size())
				{
					//if an id conflict is found, generate another and restart search
					if (temp.getID() == m_backgroundObjects[y].getID())
					{
						temp.generateID();
						y = 0;
						continue;
					}

					++y;
				}

				//pass the new unique id to the calling function
				uniqueID = temp.getID();

				//add temp to the background objects vector
				temp.keepTexture();
				m_backgroundObjects.push_back(temp);
				return fbc_err::OK;
			}
		}
	}
	else if (ASSET_TYPE == FRGD)
	{
		for (unsigned int i = 0; i < m_foregroundObjects.size(); ++i)
		{
			if (CALLNAME == m_foregroundObjects[i].getCallname())
			{
				//create temp object for copy
				fatcore::object_foreground temp = m_foregroundObjects[i];

				//generate new uniqueID for the copied texture
				temp.generateID();
				
				unsigned int y = 0;
				while (y < m_foregroundObjects.size())
				{
					//if an id conflict is found, generate another and restart search
					if (temp.getID() == m_foregroundObjects[y].getID())
					{
						temp.generateID();
						y = 0;
						continue;
					}

					++y;
				}

				//pass the new unique id to the calling function
				uniqueID = temp.getID();

				//add temp to the background objects vector
				temp.keepTexture();
				m_foregroundObjects.push_back(temp);
				return fbc_err::OK;
			}
		}
	}

	return fbc_err::CALLNAME_NOT_FOUND;
}
int fatcore::render::unload(const int ASSET_TYPE, const int UNIQUE_ID)
{
	//determine what type of asset is being unloaded
	if (ASSET_TYPE == BKGD)
	{
		//create variable to host the location of the asset we are deleteing
		int pos = 0;
		bool matchFound = false;

		//begin search loop to find ID
		for (unsigned int i = 0; i < m_backgroundObjects.size(); ++i)
		{
			if (UNIQUE_ID == m_backgroundObjects[i].getID())
			{
				pos = i;
				matchFound = true;
				m_backgroundObjects[i].delTexture();
			}
		}

		//make sure asset is not in use by other assets
		for (unsigned int i = 0; i < m_backgroundObjects.size(); ++i)
		{
			//if any other assets with that callname are registered do not unload
			if ((m_backgroundObjects[pos].getCallname() == m_backgroundObjects[i].getCallname()) && (i != pos))
			{
				m_backgroundObjects[pos].keepTexture();
			}
		}

		//delete the object from the vector if a match was found
		if (matchFound)
		{
			m_backgroundObjects.erase((m_backgroundObjects.begin() + pos));
			return fbc_err::OK;
		}

		//if no match was found
		return fbc_err::ID_NOT_FOUND;
	}
	else if (ASSET_TYPE == FRGD)
	{
		//create variable to host the location of the asset we are deleteing
		int pos = 0;
		bool matchFound = false;

		//begin search loop to find ID
		for (unsigned int i = 0; i < m_foregroundObjects.size(); ++i)
		{
			if (UNIQUE_ID == m_foregroundObjects[i].getID())
			{
				pos = i;
				matchFound = true;
				m_foregroundObjects[i].delTexture();
			}
		}

		//make sure asset is not in use by other assets
		for (unsigned int i = 0; i < m_foregroundObjects.size(); ++i)
		{
			//if any other assets with that callname are registered do not unload
			if ((m_foregroundObjects[pos].getCallname() == m_foregroundObjects[i].getCallname()) && (i != pos))
			{
				m_foregroundObjects[pos].keepTexture();
			}
		}

		//delete the object from the vector if a match was found
		if (matchFound)
		{
			m_foregroundObjects.erase((m_foregroundObjects.begin() + pos));
			return fbc_err::OK;
		}

		//if no match was found
		return fbc_err::ID_NOT_FOUND;

	}

	//if ID was not found return error code
	return fbc_err::ID_NOT_FOUND;
}
void fatcore::render::activate(const int ASSET_TYPE, const int UNIQUE_ID)
{
	if (ASSET_TYPE == BKGD)
	{
		for (unsigned int i = 0; i < m_backgroundObjects.size(); ++i)
		{
			if (UNIQUE_ID == m_backgroundObjects[i].getID())
			{
				m_backgroundObjects[i].activate();
			}
		}
	}
	else if (ASSET_TYPE == FRGD)
	{
		for (unsigned int i = 0; i < m_foregroundObjects.size(); ++i)
		{
			if (UNIQUE_ID == m_foregroundObjects[i].getID())
			{
				m_foregroundObjects[i].activate();
			}
		}
	}
}
void fatcore::render::deactivate(const int ASSET_TYPE, const int UNIQUE_ID)
{
	if (ASSET_TYPE == BKGD)
	{
		for (unsigned int i = 0; i < m_backgroundObjects.size(); ++i)
		{
			if (UNIQUE_ID == m_backgroundObjects[i].getID())
			{
				m_backgroundObjects[i].deactivate();
			}
		}
	}
	else if (ASSET_TYPE == FRGD)
	{
		for (unsigned int i = 0; i < m_foregroundObjects.size(); ++i)
		{
			if (UNIQUE_ID == m_foregroundObjects[i].getID())
			{
				m_foregroundObjects[i].deactivate();
			}
		}
	}
}

/*************************************************************************************************************

Windows related member functions and data members

*************************************************************************************************************/

#ifdef _WIN32

int fatcore::render::initDirect3D()
{
	//initialize Direct3D
	m_d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_d3d)
	{
		MessageBox(m_window, "Error initializing Direct3D", "Error", 0);
		return fbc_err::D3D_FAIL;
	}

	//set Direct3D presentation parameters
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = m_resolution.width;
	d3dpp.BackBufferHeight = m_resolution.height;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.hDeviceWindow = m_window;

	//create Direct3D device
	HRESULT result = m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_device);
	if (!m_device)
	{
		MessageBox(m_window, "Error initializing Direct3D Device" , "Error", 0);
		return fbc_err::D3D_FAIL;
	}

	//get pointer to the backbuffer
	result = m_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_backbuffer);
	if (result != D3D_OK) return fbc_err::INIT_FAIL;

	//create sprite object
	result = D3DXCreateSprite(m_device, &m_sprite);
	if (result != D3D_OK) return fbc_err::INIT_FAIL;

	return fbc_err::OK;
}
void fatcore::render::shutdownDirect3D()
{
	//free memory
	if (m_d3d)	m_d3d->Release();
	if (m_device) m_device->Release();
}
void fatcore::render::renderWIN32()
{
	if (m_device->BeginScene())
	{
		//prepair for rendering
		m_device->ColorFill(m_backbuffer, NULL, D3DCOLOR_XRGB(0,0,0));
		m_sprite->Begin(D3DXSPRITE_ALPHABLEND);

		//render background objects
		for (unsigned int i = 0; i < m_backgroundObjects.size(); ++i)
		{
			m_backgroundObjects[i].render();
		}

		//render foreground objects
		for (unsigned int i = 0; i < m_foregroundObjects.size(); ++i)
		{
			m_foregroundObjects[i].render();
		}

		//render text
		text_render textMan;
		textMan.render();

		//render foreground objects (Top Level)
		for (unsigned int i = 0; i < m_foregroundObjects.size(); ++i)
		{
			m_foregroundObjects[i].render(true);
		}

		//present scene
		m_sprite->End();
		m_device->EndScene();
		m_device->Present(NULL, NULL, NULL, NULL);
	}
}
void fatcore::render::renderWIN32(const assetTypes ASSET_TYPE)
{
	if (ASSET_TYPE == BKGD)
	{
		for (unsigned int i = 0; i < m_backgroundObjects.size(); ++i)
		{
			m_backgroundObjects[i].render();
		}
	}
	else if (ASSET_TYPE == FRGD)
	{
		for (unsigned int i = 0; i < m_foregroundObjects.size(); ++i)
		{
			m_foregroundObjects[i].render();
		}
	}
}
#endif

/****************************************************
primary render device
****************************************************/
fatcore::render __PRIMARY_RENDER_DEVICE__;
