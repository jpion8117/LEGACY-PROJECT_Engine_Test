/***************************************************************************************************
FatBoy Engine -- sprite

Purpose: this defines the basic sprite objects used for handling all charactors. (human or npc)
***************************************************************************************************/

#include "universal_includes.h"
#include "errors.h"
#include "debug.h"
#include "core.h"
#include "sprites.h"

//static variable definitions
int fatcore::sprite_management::m_instanceCount = 0;
bool fatcore::sprite_management::m_cleanUpComplete = false;
std::vector<fatcore::sprite*> fatcore::sprite_management::m_sprites;

fatcore::sprite_management::sprite_management()
{
	m_instanceCount++;
}
fatcore::sprite_management::~sprite_management()
{
	m_instanceCount--;

	if (m_instanceCount == 0)
	{
		//trigger cleanup flag
		m_cleanUpComplete = true;

		for (unsigned int i = 0; i < m_sprites.size(); ++i)
		{
			if (m_sprites[i]) delete m_sprites[i];
			m_sprites[i] = NULL;
		}
	}
}
void fatcore::sprite_management::procFrame()
{
	for (unsigned int i = 0; i < m_sprites.size(); ++i)
	{
		if (m_sprites[i] == NULL) continue;
		if (m_sprites[i]->m_activeSprite)
		{
			m_sprites[i]->procCD();
			m_sprites[i]->procFrame();
		}
	}
}
void fatcore::sprite_management::registerSprite(fatcore::sprite* newSprite)
{
	//check if there is an open slot in the registry
	for (unsigned int i = 0; i < m_sprites.size(); ++i)
	{
		if (m_sprites[i] == NULL)
		{
			m_sprites[i] = newSprite;
			newSprite->m_registryLocation = i;
			return;
		}
	}

	//if no empty slots were found, add the new sprite to the end
	m_sprites.push_back(newSprite);
	newSprite->m_registryLocation = m_sprites.size() - 1;	//set the sprite's registry location
}
void fatcore::sprite_management::releaseSprite(fatcore::sprite* Sprite)
{
	//save the registry location to a new local variable for use after deletion
	int temp = Sprite->m_registryLocation;

	//Nullify the vector entry previously occupied by the sprite
	m_sprites[temp] = NULL;
}
std::vector<fatcore::sprite*> fatcore::sprite_management::getSpritesInClass(const int SPRITE_GROUP)
{
	std::vector<sprite*> temp;

	for (unsigned int i = 0; i < m_sprites.size(); ++i)
	{
		if (!m_sprites[i]) continue;

		if ((m_sprites[i]->m_spriteClassName == SPRITE_GROUP) && m_sprites[i]->m_activeSprite)
			temp.push_back(m_sprites[i]);
	}

	return temp;
}

//primary sprite manager in charge of cleanup after program ends
fatcore::sprite_management __PRIMARY_SPRITE_MANAGER__;