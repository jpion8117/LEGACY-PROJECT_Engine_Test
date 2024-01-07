/***************************************************************************************************
FatBoy Engine -- Game_Entry

Purpose: this file serves as the universal game loop entry point as well as the universal
intitialization function(s)
***************************************************************************************************/

//#pragma comment(lib, "FatCore.lib")

#include <FatCore.h>
#include <render_2.h>
#include <core.h>
#include <string>
#include <vector>
#include "Scene_Menu.h"
#include <fatcore_scene.h>
#include "Splash.h"

//global variable initialization
bool fatcore::game_running = true;
std::string fatcore::game_title = "Engine Test";

//universal initilization function
int fatcore::Game_Init()
{
	//seed the random number generator
	srand(static_cast<unsigned int>(time(0)));

	fatcore::input inputDev;
	fatcore::render2 renderDev;
	fatcore::sound soundDev;

	renderDev.initialize();
	inputDev.initialize();
	soundDev.initialize();

	//load first scene
	fatcore::scene_proc sceneProc;
	sceneProc.setActiveScene(sceneID::TEST_SPLASH);

	//if everything initialized without error return success code
	return fbc_err::OK;
}

//universal game loop processing
void fatcore::Game_Loop()
{	
	//process the frame
	fatcore::scene_proc sceneProc;
	sceneProc.procFrame();
}

//universal shutdown
void fatcore::Game_Shutdown()
{
}