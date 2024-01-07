#pragma once

#ifndef __TEST_BALLDROP_SCENE_HEADER__
#define __TEST_BALLDROP_SCENE_HEADER__

#include <FatCore.h>
#include <sstream>
#include "Scene_IDs.h"
#include "render_2.h"

enum BALL_BOOL_RETURNS
{
	FOLLOW_CURSOR,
	CLICK
};

std::string toString(const int CONVERTED_INT)
{
	std::stringstream ss;
	ss << CONVERTED_INT;
	return ss.str();
}

class ball : public fatcore::sprite
{
public:
	ball(int weight, fatcore::cursor* cursor) : sprite("button.bmp", { 0, 150, 150, 300 }, { 0, 0, 150, 150 })
	{
		assignInput(fbc_input::MOUSE_CLICK_LEFT, "MOUSE_CLICK");

		m_physics.weight = weight;
		m_physics.gravityEffect = true;
		m_physics.momentumEffect = true;
		m_physics.momentumY = 1;
		m_physics.give = 1;

		m_cursor = cursor;
		m_frameCurrent = 0;
		m_framePrevious = 0;
		m_debounceCooldown = 0;

		m_debugDisplay = new fatcore::text_box("", "ariel", 30, { 550, 10, 750, 560 }, { 255, 255, 255 }, true);
	}
	void procFrame()
	{
		m_frameCurrent++;

		if (boolReturns[FOLLOW_CURSOR])
		{
			m_physics.Location.x = m_cursor->getRenderLoc().x - 62;
			m_physics.Location.y = m_cursor->getRenderLoc().y - 62;
			m_physics.Location.width = m_physics.Location.x + 150;
			m_physics.Location.height = m_physics.Location.y + 150;	
		}

		//prep debug display
		std::string debug;
		debug += "Ball X: " + toString(m_physics.Location.x) + "\n";
		debug += "Ball Y: " + toString(m_physics.Location.y) + "\n";
		debug += "Ball W: " + toString(m_physics.Location.width) + "\n";
		debug += "Ball H: " + toString(m_physics.Location.height) + "\n\n";

		debug += "Ball mX: " + toString(m_physics.momentumX) + "\n";
		debug += "Ball mY: " + toString(m_physics.momentumY) + "\n\n";
		
		debug += "Cursor mX: " + toString(m_cursor->getRenderLoc().x - m_cursorPrev.x) + "\n";
		debug += "Cursor mY: " + toString(m_cursor->getRenderLoc().y - m_cursorPrev.y) + "\n\n";

		debug += "fallStart: " + toString(m_physics.fallstart) + "\n";
		debug += "motionstart: " + toString(m_physics.motionstart) + "\n";

		m_debugDisplay->text = debug;

		fatcore::render2 rDev;
		rDev.setRenderLoc(fatcore::FRGD, m_uniqueVisualID, m_physics.Location);
	}
	~ball()
	{
		sprite::~sprite();

		delete m_debugDisplay;
	}
	void procCD()
	{
		m_debounceCooldown++;

		//reset FOLLOW_CURSOR
		boolReturns[CLICK] = false;

		//check if the ball is colliding with the cursor
		if (m_cursor != NULL)
		{
			if (fatcore::locIntersect(m_cursor->getRenderLoc(), m_physics.Location))
			{
				fatcore::input iDev;
				iDev.checkStatus(boolReturns[CLICK], m_inputs[0].uniqueID);
				if (boolReturns[CLICK] && (m_debounceCooldown > 30))
				{
					m_debounceCooldown = 0;
					boolReturns[FOLLOW_CURSOR] = !boolReturns[FOLLOW_CURSOR];

					if (!boolReturns[FOLLOW_CURSOR])
					{
						m_physics.momentumX = m_cursor->getRenderLoc().x - m_cursorPrev.x;
						m_physics.momentumY = m_cursor->getRenderLoc().y - m_cursorPrev.y;

						if (m_physics.momentumY == 0)
							m_physics.momentumY = 1;
					}
				}
			}

			if ((m_frameCurrent - 5 ) == m_framePrevious)
			{
				m_framePrevious = m_frameCurrent;
				m_cursorPrev = m_cursor->getRenderLoc();
			}

			if (boolReturns[FOLLOW_CURSOR])
			{
				m_physics.momentumX = 0;
				m_physics.momentumY = 0;
			}
		}

		//keep ball on the screen
		fatcore::render2 rDev;
		fatcore::physics_proc pyProc;
		if (m_physics.Location.x < 0)
		{
			fatcore::loc temp = m_physics.Location;
			m_physics.Location = { 0, temp.y, 150, temp.height };
			m_physics.momentumX = m_physics.momentumX * pyProc.calcGive(-0.25, m_physics.give);
		}
		if (m_physics.Location.width > rDev.getScreenResolution().width)
		{
			fatcore::loc temp = m_physics.Location;
			m_physics.Location = { (rDev.getScreenResolution().width - 150), temp.y, rDev.getScreenResolution().width, temp.height };
			m_physics.momentumX = m_physics.momentumX * pyProc.calcGive(-0.25, m_physics.give);
		}
		if (m_physics.Location.y < 0)
		{
			fatcore::loc temp = m_physics.Location;
			m_physics.Location = { temp.x, 0, temp.width, 150 };
			m_physics.momentumY = m_physics.momentumY * pyProc.calcGive(-0.25, m_physics.give);
		}
		if (m_physics.Location.height > rDev.getScreenResolution().height)
		{
			fatcore::loc temp = m_physics.Location;
			m_physics.Location = { temp.x, (rDev.getScreenResolution().height - 150), temp.width, rDev.getScreenResolution().height };
			m_physics.momentumY = m_physics.momentumY * pyProc.calcGive(-0.25, m_physics.give);
		}
	}

private:
	fatcore::cursor* m_cursor;
	fatcore::loc m_cursorPrev;
	fatcore::text_box* m_debugDisplay;
	unsigned long int m_frameCurrent;
	unsigned long int m_framePrevious;
	int m_debounceCooldown;
};

class ballDrop : public fatcore::scene
{
public:
	ballDrop() : scene(sceneID::TEST_BALLDROP)
	{
		m_cursor = new fatcore::cursor("pointer2.bmp", { 0, 0, 25, 25 }, { 0, 0, 25, 25 });
		m_ball = new ball(30, m_cursor);

		fatcore::input iDev;
		iDev.registerInput(fbc_input::ESC, m_escape);

		fatcore::render2 rDev;
		fatcore::physics_proc pProc;

		pProc.setgravpoint(rDev.getScreenResolution().height * 2);
	}
	~ballDrop()
	{
		delete m_cursor;
		delete m_ball;
	}
	void procFrame()
	{
		fatcore::input iDev;
		fatcore::sprite_management sMan;
		
		bool temp = false;
		iDev.checkStatus(temp, m_escape);
		if (temp)
		{
			fatcore::scene_proc sProc;
			sProc.setActiveScene(sceneID::TEST_MENU);
		}

		sMan.procFrame();
	}

private:
	fatcore::cursor* m_cursor;
	ball* m_ball;
	int m_escape;
};

#endif