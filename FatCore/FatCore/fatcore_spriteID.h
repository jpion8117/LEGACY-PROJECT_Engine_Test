/***************************************************************************************************
FatBoy Engine -- sprite

Purpose: this defines the basic sprite objects used for handling all sprites displayed on the
screen.
***************************************************************************************************/

#pragma once

namespace fbc_spriteID
{
#ifndef _SPRITE_ID_HEADER_
#define _SPRITE_ID_HEADER_

	enum CD_GROUPS
	{
		CD_GROUPS_START = 42000,

		CDGROUP_CURSOR,
		CDGROUP_BUTTON,
		CDGROUP_BALL,

		CD_GROUP_END
	};

	enum SPRITE_CLASS
	{
		SPRITE_CLASS_CORE_START = 22000,

		CURSOR,
		BUTTON,
		BALL,

		SPRITE_CLASS_CORE_END
	};

#endif
}