/***************************************************************************************************
FatBoy Engine -- fatcore_scene

Purpose: defines the base class for a scene and the scene management object
***************************************************************************************************/

#include "universal_includes.h"
#include "render_2.h"
#include "fatcore_scene.h" //Class declarations

//Deffinitions for scene base class
fatcore::scene::scene(const int SCENE_ID) :
m_SCENE_ID(SCENE_ID)
{}
fatcore::scene::~scene() {}

//static member definitions
int fatcore::scene_proc::scene_proc_count = 0;
fatcore::scene* fatcore::scene_proc::activeScene = NULL;

//scene processor deffinitions
fatcore::scene_proc::scene_proc()
{
	//incriment count of active scene processors
	scene_proc_count++;
}
fatcore::scene_proc::~scene_proc()
{
	//deincriment count of active scene processors
	scene_proc_count--;

	//last one performs cleanup
	if (scene_proc_count < 1)
	{
		delete activeScene;
		activeScene = NULL;
	}
}
void fatcore::scene_proc::procFrame()
{
	//end game if no scene is loaded
	if (!activeScene) game_running = false;

	//system interface devices
	fatcore::render2 renderDev;
	fatcore::input inputDev;
	fatcore::sound soundDev;
	fatcore::physics_proc pProc;

	//poll input devices for update
	inputDev.update();

	//process physics
	pProc.procframe();

	//process scene frame
	activeScene->procFrame();

	//render results
	renderDev.render_frame();
}
void fatcore::scene_proc::setActiveScene(const int SCENE_ID)
{
	//If there is a current scene active, delete it
	if (activeScene)
	{
		delete activeScene;
		activeScene = NULL;
	}

	//Load the new active scene
	activeScene = sceneLoader(SCENE_ID);
}

//Placeholder
fatcore::scene_proc __SCENE_PROC_GARBAGE_COLLECT__;