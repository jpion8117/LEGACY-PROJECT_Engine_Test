/***************************************************************************************************
FatBoy Engine -- Render

Purpose: The basic rendering classes in this file will allow for all rendering to be abstracted
there for allowing easy porting to other systems.
***************************************************************************************************/
#ifndef _RENDER_HEADER_INCLUDES_
#define _RENDER_HEADER_INCLUDES_

#define RENDER

#ifdef _WIN32

#define WIN32_EXTRA_LEAN

#endif

#include "core.h"
#include "errors.h"
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

#endif // _WIN32 check

#endif // _RENDER_HEADER_INCLUDES_

namespace fatcore
{
#ifndef FATBOY_ENGINE_RENDER
#define FATBOY_ENGINE_RENDER 
	class object_foreground
	{
	public:
		object_foreground(std::string filename, std::string callname, rgb transColor, int& uniqueID, const bool TOP_LEVEL = false);
		object_foreground(std::string filename, std::string callname, int& uniqueID, const bool TOP_LEVEL = false);
		~object_foreground(); //destructor will take any nessissary action to free memory when instance is deleted
		std::string getCallname();
		int getID();
		void render(const bool topLevel = false);
		void setRenderLoc(loc location);
		void setSourceLoc(loc location);
		void setColor(rgb color);
		void setTopLevel(const bool TOP_LEVEL);
		void keepTexture();
		void delTexture() { m_unloadTexture = true; }
		int generateID();
		inline void activate()  { m_active = true; }
		inline void deactivate()  { m_active = false; }

		//Windows and DirectX related member functions
		#ifdef _WIN32
		bool loadWin32(std::string filename, std::string callname, D3DCOLOR transColor); //loads images into texture files, returns false upon failure to load
		bool releaseTexture(); //frees memory allocated to stored textures when erased
		void renderWIN32();
		#endif

	protected:
		int m_ID;
		loc m_render_location;
		loc m_source_location;
		rgb m_renderColor;
		rgb m_transColor;
		std::string m_filename;
		std::string m_callname;
		bool m_unloadTexture;
		bool m_active;
		bool m_topLevel;
		bool m_failure; //safety switch, false when file is not loaded/ready to render

		//Windows and DirectX related data members
		#ifdef _WIN32
		LPDIRECT3DTEXTURE9 m_Texture;
		#endif
	};
	
	class object_background
	{
	public:
		object_background(std::string filename, std::string callname, int& uniqueID);
		~object_background(); //destructor will take any nessissary action to free memory when instance is deleted
		std::string getCallname(); //returns the call name of the file
		int getID();
		void render();
		void setRenderLoc(loc location);
		void setSourceLoc(loc location);
		void keepTexture();
		void delTexture() { m_UnloadTexture = true; }
		int generateID();
		inline void activate()  { m_active = true; }
		inline void deactivate()  { m_active = false; }

		//Windows and DirectX related member functions
#ifdef _WIN32
		bool loadWin32(); //loads images into texture files, returns false upon failure to load
		bool releaseSurface(); //frees memory allocated to stored surface when erased
		void renderWIN32();
#endif

	protected:
		int m_ID;
		std::string m_filename;
		std::string m_callname;
		bool m_failure;
		bool m_UnloadTexture;
		bool m_active;
		loc m_render_location;
		loc m_source_location;

		//Windows and DirectX related data members
#ifdef _WIN32
		LPDIRECT3DSURFACE9 m_surface;
#endif
	};

	class render2;

	enum assetTypes //deffinitions for what type of asset is being called
	{
		BKGD, //background asset
		FRGD //foreground asset
	};

	class render
	{
		friend class render2;

	public:

		render();
		~render();
		int initialize(); //will call system specific functions in order to prepair the system for rendering
		void shutdown(); //will act as a start point for system specific shutdown calls
		void setResolution(const int WIDTH, const int HEIGHT); //set the resolution, must be set before initialization
		int render_scene(); //renders all current objects to scene, will return error codes upon failure (see book)

		//asset management functions
		int setRenderLoc(const int ASSET_TYPE, const int UNIQUE_ID, const loc NEW_LOCATION); //set a new render location, use BKGD for backgroud and FRGD for foreground
		int setSourceLoc(const int ASSET_TYPE, const int UNIQUE_ID, const loc NEW_LOCATION); //set a new source location, use BKGD for backgroud and FRGD for foreground
		int setRenderColor(const int UNIQUE_ID, const rgb NEW_COLOR); //set a new render color for a foreground asset
		int loadBackground(const std::string FILENAME, const std::string CALLNAME, int& uniqueID); //load an object into database, returns error code upon failure (see book)
		int loadforeground(const std::string FILENAME, const std::string CALLNAME, int& uniqueID, const bool TOP_LEVEL = false);
		int loadforeground(const std::string FILENAME, const std::string CALLNAME, const rgb TRANS_COLOR, int& uniqueID, const bool TOP_LEVEL = false);
		int copy(const int ASSET_TYPE, const std::string CALLNAME, int& uniqueID); //makes a copy of an object already loaded, assigning a new uniqueID without reloading it from file
		int unload(const int ASSET_TYPE, const int UNIQUE_ID); //removes an object from memory, returns error code upon failure (see book)
		void activate(const int ASSET_TYPE, const int UNIQUE_ID); //activate an unactive texture
		void deactivate(const int ASSET_TYPE, const int UNIQUE_ID);
		res getResolution(); //returns the screen resolution for use elswhere

		//Windows and DirectX related member functions
		#ifdef _WIN32
		int initDirect3D(); //initializes D3D on windows systems, returns an error code upon failure (see book)
		void shutdownDirect3D(); //safely shutsdown D3D on windows systems, returns false upon failure
		void renderWIN32(); //render the scene using Windows and DirectX
		void renderWIN32(const assetTypes ASSET_TYPE); //render only forground or background assets with Win32/DirectX
		void setWindow(HWND window) { m_window = window; } //stores the window handle provided by windows
		HWND getWindow() { return m_window; } //retrieves the window handle for other parts of the program
		LPDIRECT3D9 getInterface() { return m_d3d; } //returns the Direct3D interface pointer for use elsewhere in the program
		LPDIRECT3DDEVICE9 getDevice() { return m_device; } //returns the Direct3D device pointer for use elsewhere in the program
		LPD3DXSPRITE getSprite() { return m_sprite; } //returns the Direct3D sprite pointer for use elsewhere in the program
		#endif

	protected:
		static bool m_initialized;
		static std::vector<object_foreground> m_foregroundObjects;
		static std::vector<object_background> m_backgroundObjects;
		static int m_deviceCount;
		static res m_resolution;

		//Windows and DirectX related data members
		#ifdef _WIN32
		static HWND m_window;
		static LPDIRECT3DSURFACE9 m_backbuffer;
		static LPDIRECT3D9 m_d3d;
		static LPDIRECT3DDEVICE9 m_device;
		static LPD3DXSPRITE m_sprite;
		#endif

	};

#endif
}