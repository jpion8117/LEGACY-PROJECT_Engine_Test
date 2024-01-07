/***************************************************************************************************
FatBoy Engine -- Platformer Module -- Platform Class

Purpose: This class is a very simple but critical sprite, it defines the platforms the player 
uses to traverse the level.
***************************************************************************************************/

#include <FatCore.h>
#include "Platforms.h"
#include "fbpm_spriteID.h"

fbpm_core::platform::platform(const std::string FILENAME, const fatcore::loc INITIAL_SOURCE,
	const fatcore::loc INITIAL_RENDER, const int priority):
m_moving(false),
m_orginalLoc(INITIAL_RENDER),
m_destinationLoc(INITIAL_RENDER),
m_playerActivated(false),
sprite(FILENAME, INITIAL_SOURCE, INITIAL_RENDER, true, priority)
{
	m_CDGroupName = fbpm_spriteID::CD_PLATFORM;
	m_spriteClassName = fbpm_spriteID::S_PLATFORM;
}
fbpm_core::platform::platform(const fbpm_core::platform_config* CONFIG):
m_moving(CONFIG->moving),
m_reversable(CONFIG->reversable),
m_toDestinationLoc(m_moving),
m_orginalLoc(CONFIG->renderLoc),
m_destinationLoc(CONFIG->destinationLoc),
m_playerActivated(CONFIG->playerActivated),
m_speed(CONFIG->speed),
sprite(CONFIG->filename, CONFIG->sourceLoc, CONFIG->renderLoc, true, CONFIG->priority)
{
	m_CDGroupName = fbpm_spriteID::CD_PLATFORM;
	m_spriteClassName = fbpm_spriteID::S_PLATFORM;

	if (m_moving)
	{
		m_verticalDif = m_orginalLoc.y - m_destinationLoc.y;
		m_horizontalDif = m_orginalLoc.x - m_destinationLoc.x;

		if (m_horizontalDif < m_verticalDif)
		{
			int difference = m_verticalDif - m_horizontalDif;
			m_submoveSpeed = m_speed * (m_verticalDif / difference);

			m_horizontalSubmove = true;
			m_verticalSubmove = false;
		}
		else if (m_horizontalDif > m_verticalDif)
		{
			int difference = m_horizontalDif - m_verticalDif;
			m_submoveSpeed = m_speed * (m_horizontalDif / difference);

			m_horizontalSubmove = false;
			m_verticalSubmove = true;
		}
		else
		{
			m_submoveSpeed = m_speed;

			m_horizontalSubmove = false;
			m_verticalSubmove = false;
		}

		if (m_submoveSpeed < 1)
		{
			m_submoveSpeed = 1;
		}
	}
}
void fbpm_core::platform::procFrame()
{
	//return control if platform is stationary
	if (!m_moving) return;

	m_moveCount++;

	//begin processing platform motion
	if ((m_frameCount % m_speed) == 0)
	{
		if (m_toDestinationLoc)
		{
			if (m_verticalSubmove)
			{
				//perform vertical move if ready
				if ((m_moveCount % m_submoveSpeed) == 0)
				{
					if (m_physics.Location.y > m_destinationLoc.y)
					{
						m_physics.Location.y--;
						m_physics.Location.height--;
					}
					else if (m_physics.Location.y < m_destinationLoc.y)
					{
						m_physics.Location.y++;
						m_physics.Location.height++;
					}
				}

				//perform horizontal move
				if (m_physics.Location.x > m_destinationLoc.x)
				{
					m_physics.Location.x--;
					m_physics.Location.width--;
				}
				else if (m_physics.Location.x < m_destinationLoc.x)
				{
					m_physics.Location.x++;
					m_physics.Location.width++;
				}
			}
			else if (m_horizontalSubmove)
			{
				//perform horzontal move if ready
				if ((m_moveCount % m_submoveSpeed) == 0)
				{
					if (m_physics.Location.x > m_destinationLoc.x)
					{
						m_physics.Location.x--;
						m_physics.Location.width--;
					}
					else if (m_physics.Location.x < m_destinationLoc.x)
					{
						m_physics.Location.x++;
						m_physics.Location.width++;
					}
				}

				//perform vertical move
				if (m_physics.Location.y > m_destinationLoc.y)
				{
					m_physics.Location.y--;
					m_physics.Location.height--;
				}
				else if (m_physics.Location.y < m_destinationLoc.y)
				{
					m_physics.Location.y++;
					m_physics.Location.height++;
				}
			}
			else //equal distantce move
			{
				//horizontal move
				if (m_physics.Location.x > m_destinationLoc.x)
				{
					m_physics.Location.x--;
					m_physics.Location.width--;
				}
				else if (m_physics.Location.x < m_destinationLoc.x)
				{
					m_physics.Location.x++;
					m_physics.Location.width++;
				}

				//vertical move
				if (m_physics.Location.y > m_destinationLoc.y)
				{
					m_physics.Location.y--;
					m_physics.Location.height--;
				}
				else if (m_physics.Location.y < m_destinationLoc.y)
				{
					m_physics.Location.y++;
					m_physics.Location.height++;
				}
			}

			//reverse movement if platform has reached destination and is reversable
			if ((m_physics.Location.x == m_destinationLoc.x) &&
				(m_physics.Location.y == m_destinationLoc.y))
			{
				m_toDestinationLoc = false;
				m_moveCount = 0;
				m_frameCount = 0;
			}
		}
		else //reverse the platform
		{
			if (m_reversable)
			{
				fatcore::loc temp = m_destinationLoc;
				m_destinationLoc = m_orginalLoc;
				m_orginalLoc = temp;

				m_toDestinationLoc = true;
			}
		}
	}

	m_frameCount++;

	fatcore::render2 rDev;
	rDev.setRenderLoc(fatcore::FRGD, m_uniqueVisualID, m_physics.Location);
}
void fbpm_core::platform::procCD()
{
	if (m_playerActivated)
	{
		std::vector<fatcore::sprite*> players = getSpritesInCDGroup(fbpm_spriteID::CD_PLAYER);

		for (unsigned int i; i < players.size(); ++i)
		{
			if (players[i]->getRenderLoc().height >= m_physics.Location.y &&
				players[i]->getRenderLoc().x > m_physics.Location.x &&
				players[i]->getRenderLoc().x < m_physics.Location.width &&
				players[i]->getRenderLoc().height < m_physics.Location.height)
			{
				m_moving = true;
			}
		}
	}
}
