
#pragma once

#ifndef __TEST_MENU_SCENE_HEADER__
#define __TEST_MENU_SCENE_HEADER__

#include <FatCore.h>
#include "Scene_IDs.h"

class testMenu : public fatcore::scene
{
public:
	testMenu() :
		scene(sceneID::TEST_MENU)
	{
		testButton = new fatcore::button("button.bmp", { 0, 0, 150, 150 },
		{ 0, 150, 150, 300 }, { 300, 300, 450, 450 });
		cursor = new fatcore::cursor("pointer2.bmp", { 0, 0, 25, 25 }, { 0, 0, 25, 25 });
		exitButton = new fatcore::button("button.bmp", { 0, 0, 150, 150 },
		{ 0, 150, 150, 300 }, { 300, 150, 450, 300 });
		testButton2 = new fatcore::button("button.bmp", { 0, 0, 150, 150 },
		{ 0, 150, 150, 300 }, { 300, 450, 450, 600 });
		fatcore::button* deletedButton = new fatcore::button("button.bmp", { 0, 0, 150, 150 },
		{ 0, 150, 150, 300 }, { 0, 0, 450, 450 });

		testButton->setLabel("Bouncy Ball", "ariel", 30, { 0, 255, 0 });
		exitButton->setLabel("End Test", "ariel", 30, { 0, 255, 0 });
		testButton2->setLabel("replay splash", "ariel", 25, { 0, 0, 255 });

		testButton->setPriority(960);
		exitButton->setPriority(101);
		testButton2->setPriority(11);

		if (deletedButton)
		{
			delete deletedButton;
			deletedButton = NULL;
		}
		std::cout << "Test Menu Loaded /n";
	}
	~testMenu()
	{
		fatcore::sprite_management sMan;
		if (sMan.cleanUpComplete()) return;

		if (testButton) delete testButton;
		if (exitButton) delete exitButton;
		if (testButton2) delete testButton2;
		if (cursor) delete cursor;
	}
	void procFrame()
	{
		fatcore::sprite_management spriteMan;
		spriteMan.procFrame();

		if (testButton->boolReturns[fatcore::BUTTON_CLICKED])
		{
			fatcore::scene_proc sProc;
			sProc.setActiveScene(sceneID::TEST_BALLDROP);
			return;
		}
		if (exitButton->boolReturns[fatcore::BUTTON_CLICKED])
		{
			fatcore::game_running = false;
		}
		if (testButton2->boolReturns[fatcore::BUTTON_CLICKED])
		{
			fatcore::scene_proc sProc;
			sProc.setActiveScene(sceneID::TEST_SPLASH);
		}
	}

private:
	fatcore::button* testButton;
	fatcore::button* testButton2;
	fatcore::button* exitButton;
	fatcore::cursor* cursor;
};

#endif