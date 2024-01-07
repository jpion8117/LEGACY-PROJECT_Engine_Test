/***************************************************************************************************
FatBoy Engine -- Platformer Module -- Charactor Base

Purpose: This class is the starting point for all charactors in the game (Player, NPC, and enimies
all fall into this catigory). All basic interactions will be handled through charactor pointers.
***************************************************************************************************/

#include <FatCore.h>
#include "Player Class.h"
#include "fbpm_spriteID.h"
#include "fbpm_error.h"

/******************************************************

MEMBER FUNCTIONS - ABILITY_ITEM_BASE

******************************************************/
void fbpm_core::ability_item_base::adjustHealth(fbpm_core::player* target, const unsigned int ADJUSTMENT)
{
	target->m_health += ADJUSTMENT;

	//dont let health exceed max power
	{
		if (target->m_health > target->m_MAX_HEALTH) target->m_health = target->m_MAX_HEALTH;
	}
}
void fbpm_core::ability_item_base::adjustPower(fbpm_core::player* target, const unsigned int ADJUSTMENT)
{
	target->m_power += ADJUSTMENT;

	//dont let power exceed max power
	{
		if (target->m_power > target->m_MAX_POWER) target->m_power = target->m_MAX_POWER;
	}
}
void fbpm_core::ability_item_base::makeInvincible(fbpm_core::player* target, const unsigned int FRAMECOUNT)
{
	target->m_invincibility += FRAMECOUNT;
}
void fbpm_core::ability_item_base::giveDamage(fbpm_core::player* target, const unsigned int AMOUNT, bool ignoreDT)
{
	if (ignoreDT)
	{
		target->m_health -= AMOUNT;
	}
	else
	{
		target->m_health -= static_cast<int>(target->m_DT * AMOUNT);
	}

	//triger death variable if player is dead
	if (target->m_health <= 0)
	{
		target->m_alive = false;
	}
}

/******************************************************

MEMBER FUNCTIONS - ABILITY

******************************************************/
fbpm_core::ability::ability(const fbpm_core::ABILITY_TYPE TYPE, const int COST)
{
	m_cost = COST;
	m_type = TYPE;
}

/******************************************************

MEMBER FUNCTIONS - ITEMS

******************************************************/
fbpm_core::item::item(const fbpm_core::ITEM_TYPE TYPE)
{
	m_type = TYPE;
}

/******************************************************

MEMBER FUNCTIONS - PLAYER

******************************************************/
fbpm_core::player::player(const fbpm_core::playerConfig* CONFIG):
sprite(CONFIG->filename, CONFIG->initial_source, CONFIG->initial_render, true, CONFIG->priority),m_MAX_HEALTH(CONFIG->maxHealth),
m_MAX_POWER(CONFIG->maxPower),
m_MAX_ABILITIES(CONFIG->maxAbilities),
m_MAX_INVENTORY(CONFIG->maxInventory),
m_health(CONFIG->health),
m_power(CONFIG->power),
m_DT(CONFIG->damageThreshold),
m_invincibility(0),
m_alive(true),
m_emFinalClean(false)
{}
fbpm_core::player::~player()
{}
int fbpm_core::player::useAbility(fbpm_core::ability* targetAbility, fbpm_core::enemy* target, const int NUM_TARGETS)
{
	//passive ability
	int error = 0;
	if (!target)
	{
		error = targetAbility->usePassive(this);
	}
	//offencive ability
	else
	{
		error = targetAbility->useOffencive(target, NUM_TARGETS);
	}

	return error;
}
int fbpm_core::player::useItem(fbpm_core::item* targetItem, fbpm_core::enemy* target, const int NUM_TARGETS)
{
	//passive ability
	int error = 0;
	if (!target)
	{
		error = targetItem->usePassive(this);
	}
	//offencive ability
	else
	{
		error = targetItem->useOffencive(target, NUM_TARGETS);
	}

	return error;
}
int fbpm_core::player::pickupAbility(fbpm_core::ability* newAbility)
{
	//verify there is room for a new ability
	if (m_MAX_ABILITIES == 0)
	{
		return fbpm_err::ABILITIES_DISABLED;
	}
	else if (m_abilities.size() == m_MAX_ABILITIES)
	{
		//verify none of the entries are NULL pointers
		int nullPointers = 0;
		for (unsigned int i = 0; i < m_abilities.size(); ++i)
		{
			if (!m_abilities[i]) ++nullPointers;
		}

		if (nullPointers < 1) return fbpm_err::ABILITY_INVENTORY_FULL;
	}

	//add the new ability
	for (unsigned int i = 0; i < m_abilities.size(); ++i)
	{
		if (!m_abilities[i])
		{
			m_abilities[i] = newAbility;
			return fbpm_err::OK;
		}
	}

	//add to the end if an empty space was not found
	m_abilities.push_back(newAbility);
	return fbpm_err::OK;
}
int fbpm_core::player::pickupItem(fbpm_core::item* newItem)
{
	//verify there is room for a new ability
	if (m_MAX_ABILITIES == 0)
	{
		return fbpm_err::ITEMS_DISABLED;
	}
	else if (m_inventory.size() == m_MAX_ABILITIES)
	{
		//verify none of the entries are NULL pointers
		int nullPointers = 0;
		for (unsigned int i = 0; i < m_inventory.size(); ++i)
		{
			if (!m_inventory[i]) ++nullPointers;
		}

		if (nullPointers < 1) return fbpm_err::ITEM_INVENTORY_FULL;
	}

	//add the new ability
	for (unsigned int i = 0; i < m_inventory.size(); ++i)
	{
		if (!m_inventory[i])
		{
			m_inventory[i] = newItem;
			return fbpm_err::OK;
		}
	}

	//add to the end if an empty space was not found
	m_inventory.push_back(newItem);
	return fbpm_err::OK;
}
void fbpm_core::player::procCD()
{
	//check players location in realtion to platforms
	std::vector<fatcore::sprite*> platforms = getSpritesInCDGroup(fbpm_spriteID::CD_PLATFORM);
	
	bool chkFeet = true;
	bool chkHead = true;
	bool chkLeft = true;
	bool chkRight = true;

	//get width and height of sprite
	const int WIDTH = m_physics.Location.width - m_physics.Location.x;
	const int HEIGHT = m_physics.Location.height - m_physics.Location.y;

	for (unsigned int i = 0; i < platforms.size(); ++i)
	{
		if (m_physics.Location.height > platforms[i]->getRenderLoc().y && chkFeet)
		{
			m_physics.Location.height = platforms[i]->getRenderLoc().y;
			m_physics.Location.y = m_physics.Location.height - HEIGHT;
			chkFeet = false;
		}
		if (m_physics.Location.y < platforms[i]->getRenderLoc().height && chkHead)
		{
			m_physics.Location.y = platforms[i]->getRenderLoc().height;
			m_physics.Location.height = m_physics.Location.y + HEIGHT;
			chkHead = false;
		}
		if (m_physics.Location.width > platforms[i]->getRenderLoc().x && chkRight)
		{
			m_physics.Location.width = platforms[i]->getRenderLoc().x;
			m_physics.Location.x = m_physics.Location.width - WIDTH;
			chkRight = false;
		}
		if (m_physics.Location.x < platforms[i]->getRenderLoc().width && chkLeft)
		{
			m_physics.Location.x = platforms[i]->getRenderLoc().width;
			m_physics.Location.width = m_physics.Location.x + WIDTH;
			chkLeft = false;
		}
	}

	//run additional CD function
	this->procAddCD();
}
void fbpm_core::player::procFrame()
{
	if (m_invincibility > 0) m_invincibility--;

	procCD();

	this->procAddFrame();
}