/***************************************************************************************************
FatBoy Engine -- Platformer Module -- Platform classes

Purpose: Define all classes related to platform including the platform class it's self and any
supporting classes used for interaction with the platform class
***************************************************************************************************/

#include <FatCore.h> //fatcore base component
#include "fbpm_error.h"

namespace fbpm_core
{
#ifndef __PLATFORM_HEADER__
#define __PLATFORM_HEADER__

	struct platform_config
	{
	public:
		std::string filename = "";
		fatcore::loc sourceLoc = { 0, 0, 0, 0 };
		fatcore::loc renderLoc = { 0, 0, 0, 0 };
		fatcore::loc destinationLoc = { 0, 0, 0, 0 };
		bool moving = false;
		bool reversable = true;
		bool playerActivated = false;
		int priority = 100;
		int speed = 5;
	};

	class platform : public fatcore::sprite
	{
	public:
		platform(const std::string FILENAME, const fatcore::loc INITIAL_SOURCE,
			const fatcore::loc INITIAL_RENDER, const int priority = 100);
		platform(const platform_config* CONFIG); //this must be used for advanced configurations
		virtual void procFrame();

	private:
		virtual void procCD();

		bool m_moving;
		bool m_reversable;
		bool m_playerActivated;
		bool m_toDestinationLoc;
		bool m_verticalSubmove;
		bool m_horizontalSubmove;
		int m_submoveSpeed;
		int m_verticalDif;
		int m_horizontalDif;
		int m_speed; //number of frames to pass before each move
		int m_frameCount = 0; //number of frames since platform creation
		int m_moveCount = 0; //number of movement operations since last sub move operation
		fatcore::loc m_destinationLoc; //used for moving platforms
		fatcore::loc m_orginalLoc;
	};

#endif
}