/***************************************************************************************************
FatBoy Engine -- Platformer Module -- spriteID

Purpose: this file defines the various spriteIDs used by all sprites that are part of the module.
The game spriteIDs would therefore begin where these ones end, defined by the constants,
fbpm_spriteID::GAME_SPRITES_BEGIN, and fbpm_spriteID::GAME_CD_GROUPS_BEGIN.
***************************************************************************************************/

#include <FatCore.h>
#include <fatcore_spriteID.h>

namespace fbpm_spriteID
{
#ifndef __FBPM_SPRITE_ID_HEADER__
#define __FBPM_SPRITE_ID_HEADER__
	
	enum SPRITE_ID
	{
		SPRITE_ID_START = fbc_spriteID::SPRITE_CLASS_CORE_END,

		S_PLATFORM,
		S_PROJECTILE,

		SPRITE_ID_END
	};

	enum SPRITE_CD_GROUP
	{
		SPRITE_CD_GROUP_START = fbc_spriteID::CD_GROUP_END,

		CD_PLAYER,
		CD_ENTITY,
		CD_PLATFORM,
		CD_PROJECTILE_ENEMY,
		CD_PROJECTILE_PLAYER,
		CD_ITEM,
		CD_INTERACTIVE_OBJECT,

		SPRITE_CD_GROUP_END
	};

	const int GAME_SPRITES_BEGIN = SPRITE_ID_END;
	const int GAME_CD_GROUP_BEGIN = SPRITE_CD_GROUP_END;

#endif
}