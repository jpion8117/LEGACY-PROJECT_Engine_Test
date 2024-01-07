/***************************************************************************************************
FatBoy Engine -- fatcore_scene

Purpose: defines the base class for a scene and the scene management object
***************************************************************************************************/

#include "universal_includes.h"
#include "sprites.h"

namespace fatcore
{
#ifndef _FATCORE_SCENE_HEADER_
#define _FATCORE_SCENE_HEADER_

	class scene
	{
	public:
		scene(const int SCENE_ID);
		virtual ~scene();
		virtual void procFrame() = 0;

	protected:
		const int m_SCENE_ID;	
	};

	class scene_proc
	{
	public:
		scene_proc();
		~scene_proc();
		void procFrame();
		void setActiveScene(const int SCENE_ID);
		
	protected:
		static scene* activeScene;
		static int scene_proc_count;
	};
#endif
}

#ifndef __SCENE_HEADER__
#define __SCENE_HEADER__

extern fatcore::scene* sceneLoader(const int SCENE_ID);

#endif
