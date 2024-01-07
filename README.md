# Lightweight Adaptive Reusable Development for Assembling Stupid Stuff
## This project has been retired or would require major rewrites in order to be functionally revived due to its reliance on outdated and depreciated libraries that I've been unable to locate for several years.

This was a very large and ambitious pet project of mine many years ago. I was the sole developer on the project and did not use GitHub at the time but have decided to upload the remnants of the project here to showcase my C++ skills which are otherwise unrepresented by the rest of my GitHub portfolio. The project began as a personal challenge to myself. I wanted to apply everything I had been learning about C++ and the DirectX SDK by building a customizable, reusable, object-oriented toolkit to build simple 2D games. Though game engines exist that fit this role, I wanted to build it myself to see if I could. I had several goals for this project.
* All OS specific code was to be abstracted away from any game logic. This was done to ensure games I built with my "engine" could theoretically be ported to other systems in the future without changing any game logic.
* The project needed to have simple and easily defined components that could be extended to work with different types of games. Some of the components I developed were:
  * Scenes (scene in fatcore_scene.h) - Scenes controlled the entirety of a game’s actions at any given step. A scene would contain both background and foreground (interactable) objects and control the timings and placements of those objects through dedicated member functions for doing so.
  * Background Objects (object_background in render.h) - Background objects were objects in the game that would be rendered below anything the player could interact with. In the scene, it would render all the background objects first in the frame.
  * Foreground Objects (object-foreground in render.h) - Foreground objects designed to be interacted with or have some effect on the scene.
  * Sprites (defined in sprites.h) - These were an extension of the foreground objects? I think. It has been a very long time and there are many things that I would have done VERY differently today.
  * Sound handling - There was a way to register sound files that would be loaded and then triggered to play through the sound manager.
  * Input handling - On first glance, it looks like I handled input by mapping pre-defined actions (key A pressed, Up button pressed, etc.) to an input monitoring system that monitors those actions allowing the developer to poll this system instead of the OS specific one.
* Basic Collision detection and "physics" - I built a basic CD system and implemented a very rudimentary "physics" system that included a way to handle simple things like gravity and acceleration/deceleration.
* Built-in garbage collection - all classes had destructors that would free unused resources upon destruction and orchestration classes implemented a "last one out get the lights" policy that would free resources of any objects in their care if they were the last instance to be deleted.
* An entire module designed just for platformer games (it was supposed to be one of many).

Overall, I am very proud of what I was able to accomplish with no formal training at the time. I learned a lot from this project that I would eventually apply to other projects when I decided to return to school and pursue a formal education in software development. Some of the key takeaways I learned and applied in future projects are as follows.
* Comment code as you are writing! If I could go back in time and instill this one little nugget of knowledge, this writeup would probably look much different. Much of this has been me trying to remember what the past me did nearly 10 years ago.
* Sometimes, simpler is better. The input mapping class comes to mind here. A system that marks inputs to universal keywords would have been far more platform agnostic and probably simpler. I actually did something like this in a scaled down version of this that I wrote for my JavaScript class that handled rendering for HTML5 Canvas games. I mapped input actions such as "Move_Up" to the elements they controlled then triggered those actions when the appropriate key was pressed.
* Building your own tools is a fun challenge, but sometimes using something that has been tried and tested is just better.
