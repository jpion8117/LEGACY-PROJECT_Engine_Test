#include <FatCore.h>
#include "Scene_IDs.h"
#include "Scene_Menu.h"
#include "Splash.h"
#include "Ball_Drop.h"

fatcore::scene* sceneLoader(const int SCENE_ID)
{
	switch (SCENE_ID)
	{
	case sceneID::TEST_MENU:
		return new testMenu;
	case sceneID::TEST_SPLASH:
		return new splash1;
	case sceneID::TEST_BALLDROP:
		return new ballDrop;
	default:
		return NULL;
	}
}