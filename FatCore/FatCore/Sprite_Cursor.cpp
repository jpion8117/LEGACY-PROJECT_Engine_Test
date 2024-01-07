/***************************************************************************************************
FatBoy Engine -- sprite

Purpose: this defines the basic sprite objects used for handling all charactors. (human or npc)
***************************************************************************************************/

#include "universal_includes.h"
#include "errors.h"
#include "debug.h"
#include "core.h"
#include "menu-support.h"
#include "render_2.h"

fatcore::cursor::cursor(const std::string FILENAME, const loc INITIAL_SOURCE, 
	const loc INITIAL_RENDER, const bool ACTIVE) :
	sprite(FILENAME, INITIAL_SOURCE, INITIAL_RENDER, ACTIVE, 60000)
{
	assignInput(fbc_input::MOUSE_X, DIN_X);
	assignInput(fbc_input::MOUSE_Y, DIN_Y);
	assignInput(fbc_input::LEFT_X, XIN_X);
	assignInput(fbc_input::LEFT_Y, XIN_Y);

	m_CDGroupName = fbc_spriteID::CDGROUP_CURSOR;
	m_spriteClassName = fbc_spriteID::CURSOR;
}

void fatcore::cursor::procFrame()
{
	fatcore::input inputDev;
	fatcore::render2 renderDev;

	for (unsigned int i = 0; i < m_inputs.size(); ++i)
	{
		int status = 0;
		int result = inputDev.checkStatus(status, m_inputs[i].uniqueID);
		if (result != fbc_err::OK)
		{
			//stuff
		}
		
		if (m_inputs[i].actionName == DIN_X)
		{
			m_physics.Location.x = m_physics.Location.x + status;
			m_physics.Location.width = m_physics.Location.width + status;
		}
		else if (m_inputs[i].actionName == DIN_Y)
		{
			m_physics.Location.y = m_physics.Location.y + status;
			m_physics.Location.height = m_physics.Location.height + status;
		}
		else if (m_inputs[i].actionName == XIN_X)
		{
			m_physics.Location.x = m_physics.Location.x + (status / 7500);
			m_physics.Location.width = m_physics.Location.width + (status / 7500);
		}
		else if (m_inputs[i].actionName == XIN_Y)
		{
			m_physics.Location.y = m_physics.Location.y + ((status * -1) / 7500);
			m_physics.Location.height = m_physics.Location.height + ((status * -1) / 7500);
		}
	}

	renderDev.setRenderLoc(fatcore::FRGD, m_uniqueVisualID, m_physics.Location);
}
void fatcore::cursor::procCD()
{
	render renderDev;
	resolution _res = renderDev.getResolution();

	if (m_physics.Location.x < 0)
	{
		m_physics.Location.x = 0;
		m_physics.Location.width = m_sourceLocation.width - m_sourceLocation.x;
	}
	if (m_physics.Location.y < 0)
	{
		m_physics.Location.y = 0;
		m_physics.Location.height = m_sourceLocation.height - m_sourceLocation.y;
	}
	if (m_physics.Location.width > _res.width)
	{
		m_physics.Location.x = _res.width - m_sourceLocation.width;
		m_physics.Location.width = _res.width;
	}
	if (m_physics.Location.height > _res.height)
	{
		m_physics.Location.y = _res.height - m_sourceLocation.height;
		m_physics.Location.height = _res.height;

	}
	
}