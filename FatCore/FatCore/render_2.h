/***************************************************************************************************
FatBoy Engine -- Render2

Purpose: Rev 2 of the rendering classes. Original code base for the render classes will be built 
upon, extended, and improved, to add new functionality and run more effeciently
***************************************************************************************************/

#include "render.h"

namespace fatcore
{
#ifndef _RENDER_HEADER_2_
#define _RENDER_HEADER_2_

#ifdef _WIN32
	
	struct DIRECTX_POINTERS_WIN32
	{
		HWND window;
		LPDIRECT3DSURFACE9 backbuffer;
		LPDIRECT3D9 d3d;
		LPDIRECT3DDEVICE9 device;
		LPD3DXSPRITE sprite;
	};

	void w32dx_configWindow(render2* rDev, HWND window);

#endif
	class render2;

	class object_foreground_2
	{
		friend class render2;
	public:
		bool operator>(object_foreground_2& comp);
		bool operator<(object_foreground_2& comp);

	private:
		object_foreground_2(const std::string FILENAME, const int UNIQUE_ID,
			const int PRIORITY = 0, const rgb TRANSPARANCY_COLOR = { 0, 0, 0 });
		~object_foreground_2();

		const std::string m_FILENAME;
		const int m_UNIQUE_ID;
		int m_priority;
		bool m_active;
		bool m_sharedFile;
		rgb m_renderColor;
		rgb m_transparancyColor;
		loc m_renderLocation;
		loc m_sourceLocation;
		float m_scaleX;
		float m_scaleY;
		float m_rotation;

#ifdef _WIN32
		LPDIRECT3DTEXTURE9 m_w32dx_file;
#endif
	};

	class object_background_2
	{
		friend class render2;
	public:
		bool operator>(object_background_2& comp);
		bool operator<(object_background_2& comp);

	private:
		object_background_2(const std::string FILENAME, const int UNIQUE_ID, const int PRIORITY = 0);
		~object_background_2();

		const std::string m_FILENAME;
		const int m_UNIQUE_ID;
		int m_priority;
		bool m_active;
		bool m_sharedFile;
		loc m_renderLocation;
		loc m_sourceLocation;

#ifdef _WIN32
		LPDIRECT3DSURFACE9 m_w32dx_file;
#endif
	};
	
	class render2
	{
		friend class text_render;
	public:
		render2();
		~render2();
		fbc_err::CODE initialize();
		fbc_err::CODE render_frame();
		void setScreenResolution(const int WIDTH, const int HEIGHT);
		res getScreenResolution();
		inline unsigned long int getFrameNum() { return m_frameCount; }

		//universal asset managemnet functions
		fbc_err::CODE loadBackground(const std::string FILENAME, int& uniqueID, const int PRIORITY = 100);
		fbc_err::CODE loadForeground(const std::string FILENAME, int& uniqueID, const int PRIORITY = 100);
		fbc_err::CODE loadForeground(const std::string FILENAME, int& uniqueID, const rgb TRANSPARANT_COLOR, const int PRIORITY = 100);
		fbc_err::CODE purge(const assetTypes ASSET_TYPE, const int UNIQUE_ID);
		fbc_err::CODE setRenderLoc(const assetTypes ASSET_TYPE, const int UNIQUE_ID, const loc NEW_LOCATION);
		fbc_err::CODE setSourceLoc(const assetTypes ASSET_TYPE, const int UNIQUE_ID, const loc NEW_LOCATION);
		fbc_err::CODE setPriority(const assetTypes ASSET_TYPE, const int UNIQUE_ID, const int NEW_PRIORITY);
		void setActivationStatus(const assetTypes ASSET_TYPE, const int UNIQUE_ID, const bool ACTIVATION_STATUS);

		//foreground only functions
		fbc_err::CODE setRenderColor(const int UNIQUE_ID, const rgb NEW_COLOR); //only supports foreground objects
		fbc_err::CODE setScaleX(const int UNIQUE_ID, const float SCALE);
		fbc_err::CODE setScaleY(const int UNIQUE_ID, const float SCALE);
		fbc_err::CODE setRotation(const int UNIQUE_ID, const float ROTATION);

	private:

		//Windows and DirectX functions and data members
#ifdef _WIN32
		friend void w32dx_configWindow(render2* rDev, HWND window);

		static DIRECTX_POINTERS_WIN32 m_w32dx_DirectX_Pointers;

		fbc_err::CODE w32dx_render();
		fbc_err::CODE w32dx_load(object_background_2* file);
		fbc_err::CODE w32dx_load(object_foreground_2* file);
		fbc_err::CODE w32dx_initialize();
		void w32dx_shutdown();
		void w32dx_updateWindowsPointers();
		
#endif

		object_background_2* searchB(const int UNIQUE_ID);
		object_background_2* searchB(object_background_2* file);
		object_foreground_2* searchF(const int UNIQUE_ID);
		object_foreground_2* searchF(object_foreground_2* file);
		void sort();

		static render m_LegacyRender; //legacy render object
		static std::vector<object_foreground_2*> m_pForegroundObjects;
		static std::vector<object_background_2*> m_pBackgroundObjects;
		static bool m_initialized;
		static bool m_sortBackground;
		static bool m_sortForeground;
		static int m_instanceCount;
		static unsigned long int m_frameCount;
	};

#endif
}