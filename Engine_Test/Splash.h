#pragma once

#ifndef __TEST_SPLASH_SCENE_HEADER__
#define __TEST_SPLASH_SCENE_HEADER__

#include <FatCore.h>
#include "Scene_IDs.h"

class testMenu;

class splash1 : public fatcore::scene
{
public:
	splash1() :
		scene(sceneID::TEST_SPLASH), frameCount(0), message("Welcome!"), displayed("")
	{
		tbox = new fatcore::text_box(displayed, "Times New Roman", 256, { 50, 50, 0, 0 }, { 255, 0, 0 });
	}
	~splash1()
	{
		if (tbox) delete tbox;
	}
	void procFrame()
	{
		frameCount++;

		if ((frameCount % 20) == 0)
		{
			if (displayed == message)
			{
				fatcore::scene_proc sProc;
				sProc.setActiveScene(sceneID::TEST_MENU);
				return;
			}
			else
			{
				displayed = displayed + message[(displayed.size())];
				tbox->text = displayed;
			}
		}
		
	}

private:
	unsigned int frameCount;
	std::string message;
	std::string displayed;
	fatcore::text_box* tbox;
};

#endif