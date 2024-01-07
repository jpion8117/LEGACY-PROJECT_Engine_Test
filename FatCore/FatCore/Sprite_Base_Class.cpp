/***************************************************************************************************
FatBoy Engine -- sprite

Purpose: this defines the basic sprite objects used for handling all charactors. (human or npc)
***************************************************************************************************/

#include "universal_includes.h"
#include "errors.h"
#include "debug.h"
#include "core.h"
#include "sprites.h"
#include "render_2.h"

fatcore::sprite::sprite(const std::string FILENAME,	const loc INITIAL_SOURCE, 
	const loc INITIAL_RENDER, const bool ACTIVE, const int PRIORITY) :
m_sourceLocation(INITIAL_SOURCE)
{
	//set default physics profile
	m_physics.gravityEffect = false;
	m_physics.momentumEffect = false;
	m_physics.weight = 0;
	m_physics.give = 0;
	m_physics.momentumX = 0;
	m_physics.momentumY = 0;
	m_physics.Location = INITIAL_RENDER;
	m_physics.fallstart = 0;
	m_physics.motionstart = 0;

	//load asset from file
	fatcore::render2 renderDev;
	renderDev.loadForeground(FILENAME, m_uniqueVisualID, PRIORITY);
	renderDev.setSourceLoc(fatcore::FRGD, m_uniqueVisualID, INITIAL_SOURCE);
	renderDev.setRenderLoc(fatcore::FRGD, m_uniqueVisualID, INITIAL_RENDER);

	//initialize return pools
	for (unsigned int i = 0; i < 50; ++i)
	{
		boolReturns[i] = false;
		intReturns[i] = 0;
	}

	//add this sprite to the registry
	fatcore::sprite_management spriteMan;
	spriteMan.registerSprite(this);

	//set sprite assets to proper active state
	if (!ACTIVE)
	{
		deactivate();
	}
}
fatcore::sprite::~sprite()
{
	//purge visual assets
	render2 rendMan;
	rendMan.purge(FRGD, m_uniqueVisualID);

	//purge audio assets
	sound soundMan;
	for (unsigned int i = 0; i < m_sounds.size(); ++i)
	{
		soundMan.stop(m_sounds[i].uniqueID);
		soundMan.purge(m_sounds[i].uniqueID);
	}

	//unregister the sprite
	fatcore::sprite_management sMan;
	sMan.releaseSprite(this);
}
void fatcore::sprite::assignInput(const int INPUT_CODE, const std::string ACTION_NAME)
{
	fatcore::input inputDev;
	int uniqueID = 0; 

	//register the input
	int result = inputDev.registerInput(INPUT_CODE, uniqueID);
	if (result != fbc_err::OK)
	{
		//diagnose problem then throw exception ending program with error
	}

	//log the input into the inputs available
	fatcore::input_handle temp(uniqueID, ACTION_NAME);
	m_inputs.push_back(temp);
}
std::vector<fatcore::sprite*> fatcore::sprite::getSpritesInCDGroup(const int CD_GROUP_NAME)
{
	//create the objects nessissary to perform the search and store the data
	std::vector<sprite*> temp;		//usage: stores pointers to sprites matching search criteria
	sprite_management spriteMan;	//usage: used to access data from all sprites

	//search for active sprites in the CD group specified
	for (unsigned int i = 0; i < spriteMan.m_sprites.size(); ++i)
	{
		//skip NULL pointers
		if (!spriteMan.m_sprites[i]) continue;

		if (spriteMan.m_sprites[i]->m_CDGroupName == CD_GROUP_NAME &&
			spriteMan.m_sprites[i]->m_activeSprite)
		{
			temp.push_back(spriteMan.m_sprites[i]);
		}
	}

	//return findings to calling function
	return temp;
}
void fatcore::sprite::activate()
{
	m_activeSprite = true;

	render2 renDev;
	renDev.setActivationStatus(FRGD, m_uniqueVisualID, m_activeSprite);
}
void fatcore::sprite::deactivate()
{
	m_activeSprite = false;

	render2 renDev;
	renDev.setActivationStatus(FRGD, m_uniqueVisualID, m_activeSprite);
}
//void fatcore::sprite::setPriority(const int NEW_PRIORITY)