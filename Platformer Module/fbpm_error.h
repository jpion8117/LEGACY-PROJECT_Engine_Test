/***************************************************************************************************
FatBoy Engine -- Platformer Module -- error registry

Purpose: Define all the errors used by the platformer module
***************************************************************************************************/

#include <FatCore.h> //fatcore base component
#include <errors.h>

namespace fbpm_err
{
	enum CODE_FBPM
	{
		OK = 42, //success, no errors

		FBPM_ERR_START = fbc_err::FBC_ERR_END,

		GEN_ERR_START,

		GENERIC_FAILURE, //a generic, non breaking error occured
		INIT_FAIL, //an initialization function has failed
		UNSUPORTED_SYSTEM, //compiled on an currently unsupported system
		FILE_LOAD_FAIL, //failure occured while loading a file
		ID_NOT_FOUND, //requested ID number not found, resource may need to be reloaded

		GEN_ERR_END,

		ABILITY_ITEM_ERRORS_START,

		ABILITY_INVENTORY_FULL, //the player (or other entity) has reached the max abilities and attempted to add a new one
		ITEM_INVENTORY_FULL, //the player (or other entity) has reached the max items and attempted to add a new one
		ABILITIES_DISABLED, //abilities are disabled for this entity
		ITEMS_DISABLED, //items are disabled for this entity

		ABILITY_ITEM_ERRORS_END,

		FBPM_ERR_END,
	};
}