/***************************************************************************************************
FatBoy Engine -- sprite

Purpose: this defines the basic sprite objects used for handling all sprites displayed on the 
screen. 
***************************************************************************************************/

#ifndef _SPRITE_HEADER_INCLUDES_
#define _SPRITE_HEADER_INCLUDES_

#include "fatcore_spriteID.h"
#include "core.h"
#include "errors.h"
#include "physics.h"
#include "sound.h"
#include "input.h"
#include <ctime>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

//windows includes
#ifdef _WIN32

#include <Windows.h>
#include <WinUser.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <Xinput.h>
#include "render_2.h"

#endif

#endif

namespace fatcore
{
#ifndef _SPRITE_HEADER_
#define _SPRITE_HEADER_

	class sprite_management;

	class sprite
	{
		friend class sprite_management;

	protected:
		sprite(const std::string FILENAME = "", const loc INITIAL_SOURCE = { 0, 0, 0, 0 }, 
			const loc INITIAL_RENDER = { 0, 0, 0, 0 }, const bool ACTIVE = true, const int priority = 100);
		virtual ~sprite();
		virtual void procCD() = 0;
		std::vector<sprite*> getSpritesInCDGroup(const int CD_GROUP_NAME);

		int m_registryLocation;
		int m_uniqueVisualID;
		int m_CDGroupName;	
		int m_spriteClassName;
		std::vector<input_handle> m_inputs;
		std::vector<sound_handle> m_sounds;
		loc m_sourceLocation;
		physics_attrb m_physics;
		bool m_activeSprite;

	public:
		virtual void procFrame() = 0;
		void assignInput(const int INPUT_CODE, const std::string ACTION_NAME);
		inline void deactivate();
		inline void activate();
		inline loc getRenderLoc() { return m_physics.Location; }
		inline void setPhysicsAttrb(const physics_attrb PHYSICS) { m_physics = PHYSICS; }
		inline void setPhysicsAttrb_gravityEffect(const bool GRAVITY_EFFECT) { m_physics.gravityEffect = GRAVITY_EFFECT; }
		inline void setPhysicsAttrb_momentumEffect(const bool MOMENTUM_EFFECT) { m_physics.momentumEffect = MOMENTUM_EFFECT; }
		inline void setPhysicsAttrb_weight(const int WEIGHT) { m_physics.weight = WEIGHT; }
		inline void setPhysicsAttrb_give(const int GIVE) { m_physics.give = GIVE; }
		inline void setPhysicsAttrb_momentumX(const int MOMENTUM_X) { m_physics.momentumX = MOMENTUM_X; }
		inline void setPhysicsAttrb_momentumY(const int MOMENTUM_Y) { m_physics.momentumY = MOMENTUM_Y; }
		inline physics_attrb getPhysicsAttrb() { return m_physics; }
		inline void setPriority(const int NEW_PRIORITY) { render2 rDev; rDev.setPriority(FRGD, m_uniqueVisualID, NEW_PRIORITY); }
		inline void setNewLoc(const fatcore::loc NEW_LOCATION) { m_physics.Location = NEW_LOCATION; }

		bool boolReturns [50];
		int intReturns [50];
	};

	class sprite_management
	{
		friend class sprite;

	public:
		sprite_management();
		~sprite_management();
		void procFrame();
		std::vector<sprite*> getSpritesInClass(const int SPRITE_GROUP);
		inline bool cleanUpComplete() { return m_cleanUpComplete; }

	private:
		void registerSprite(sprite* newSprite); 
		void releaseSprite(sprite* Sprite);

		static bool m_cleanUpComplete;
		static int m_instanceCount;
		static std::vector<sprite*> m_sprites;
	};

#endif
}