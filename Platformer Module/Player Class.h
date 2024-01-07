/***************************************************************************************************
FatBoy Engine -- Platformer Module -- Charactor Base

Purpose: This class is the starting point for all charactors in the game (Player, NPC, and enimies
all fall into this catigory). All basic interactions will be handled through charactor pointers.
***************************************************************************************************/

#include <FatCore.h>

namespace fbpm_core
{
#ifndef __PLAYER_CLASS_HEADER__
#define __PLAYER_CLASS_HEADER__

	class enemy;
	class player;

	enum class ABILITY_TYPE
	{
		offencive, //any ability that effects an enemy or group of enemies
		passive    //any ability that effects the player
	};

	typedef ABILITY_TYPE ITEM_TYPE;

	//effects either players or enemies, has cost associated with it to be taken from the users energy
	class ability_item_base
	{
	public:
		//entity interface functions
		virtual ~ability_item_base() = 0 {}

		void adjustHealth(player* target, const unsigned int ADJUSTMENT);
		void adjustPower(player* target, const unsigned int ADJUSTMENT);
		void giveDamage(player* target, const unsigned int AMOUNT, bool ignoreDT = false);
		void makeInvincible(player* target, const unsigned int FRAMECOUNT);

	protected:
		ABILITY_TYPE m_type;
	};

	class ability : public ability_item_base
	{
	public:
		ability(const ABILITY_TYPE TYPE, const int COST);
		virtual int useOffencive(enemy* target, const int NUM_TARGETS = 0) = 0;
		virtual int usePassive(player* target) = 0;

	private:
		int m_cost;
	};

	class item : public ability_item_base
	{
	public:
		item(const ITEM_TYPE TYPE);
		virtual int useOffencive(enemy* target, const int NUM_TARGETS = 0) = 0;
		virtual int usePassive(player* target) = 0;
	};

	struct playerConfig
	{
	public:
		int entityClassID;
		int priority = 100;
		int health = 100;
		int power = 100;
		int maxHealth = 100;
		int maxPower = 100;
		int maxAbilities = -1;
		int maxInventory = -1;
		float damageThreshold = 0.0;
		std::string filename = "";
		fatcore::loc initial_render = { 0, 0, 0, 0 };
		fatcore::loc initial_source = { 0, 0, 0, 0 };
	};

	class player : public fatcore::sprite
	{
		friend class ability_item_base;

	public:
		player(const playerConfig* CONFIG);
		~player();
		virtual void procFrame();
		virtual void procAddFrame() = 0;
		virtual int useAbility(ability* targetAbility, enemy* target = NULL, const int NUM_TARGETS = 0);
		virtual int useItem(item* targetItem, enemy* target = NULL, const int NUM_TARGETS = 0);
		virtual int pickupAbility(ability* newAbility);
		virtual int pickupItem(item* newItem);


		inline int getID() { return m_entityID; }
		inline bool isAlive() { return m_alive; }
		inline int health() { return m_health; }
		
	protected:
		virtual void procCD();
		virtual void procAddCD() = 0;

		int m_entityClassID; //identifies what type of entity this instance is
		int m_entityID; //unique identifier assigned by the system during runtime to identify a single instance
		int m_health; //DUH! entity's health level
		unsigned int m_invincibility; //if this value is greater than 0 target cannot take damage for the respective number of frames
		const int m_MAX_HEALTH; //max health an entity can have
		bool m_alive; //entity's life status
		bool m_emFinalClean; //triggered if the primary entity_manager instance is deleted
		int m_power; //power used by abilities
		float m_DT; //damage threshold, expressed as a percentage of damage blocked EX: 0.23 means player only takes 77% of damage applied
		const int m_MAX_POWER; //max power level
		const int m_MAX_ABILITIES; //max number of abilities, 0 = abilities disabled, -1 = infinite
		const int m_MAX_INVENTORY; //max number of items, 0 = items disabled, -1 = infinite
		std::vector<ability*> m_abilities; //abilities the player can use indeffinitely as long as they have the power
		std::vector<item*> m_inventory; //single use abilities, power level is not relevent

	};

	class enemy : public player
	{
		friend class ability_item_base;
	};

	class entityInterface : public ability_item_base
	{
	public:
		~entityInterface() {};
	};

#endif
}