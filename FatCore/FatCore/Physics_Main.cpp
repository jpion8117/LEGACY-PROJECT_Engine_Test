/***************************************************************************************************
FatBoy Engine -- physics

Purpose: this will define the majority of the objects and functions that encapsulate the physics
module of the Fatboy Engine
***************************************************************************************************/

#include "universal_includes.h"
#include "physics.h"

bool fatcore::locIntersect(const loc LOC1, const loc LOC2, loc* intersectLoc)
{
	if ((((LOC1.height > LOC2.y) && (LOC1.height < LOC2.height)) || ((LOC1.y < LOC2.height) && (LOC1.y > LOC2.y))) &&
		(((LOC1.width > LOC2.x) && (LOC1.width < LOC2.width)) || ((LOC1.x < LOC2.width) && (LOC1.x> LOC2.x))))
	{
		if (intersectLoc != NULL)
		{
			//UNDER CONSTRUCTION: determine location of intersection, card in file 11-20-17
		}

		return true;
	}

	return false;
}

//physics_attrib (de)constructor(s)
fatcore::physics_attrb::physics_attrb()
{
	physics_proc pyProc; 
	pyProc.registerPhysicsProfile(this, m_registryLocation);
}
fatcore::physics_attrb::~physics_attrb()
{
	physics_proc pyProc; 
	pyProc.purgePhysicsProfile(this, m_registryLocation);
}

//static member deffinition
int fatcore::physics_proc::m_instancecount = 0;
int fatcore::physics_proc::m_gravpoint = 0;
long unsigned int fatcore::physics_proc::m_frameCount = 0;
bool fatcore::physics_proc::m_cleanUpComplete = false;
std::vector<fatcore::physics_attrb*> fatcore::physics_proc::m_objectsWithPhysics;

fatcore::physics_proc::physics_proc()
{
	//incriment instance count
	m_instancecount++;
}
fatcore::physics_proc::~physics_proc()
{
	//deincriment instance count
	m_instancecount--;

	//last one performs cleanup
	if (m_instancecount < 1)
	{
		m_cleanUpComplete = true;

		for (unsigned int i = 0; i < m_objectsWithPhysics.size(); ++i)
		{
			if (m_objectsWithPhysics[i] != NULL)
			{
				m_objectsWithPhysics[i] = NULL;
			}
		}
	}
}
void fatcore::physics_proc::registerPhysicsProfile(fatcore::physics_attrb* newProfile, int& registryLocation)
{
	//search for any empty spaces in the registry and occupy those first
	for (unsigned int i = 0; i < m_objectsWithPhysics.size(); ++i)
	{
		if (!m_objectsWithPhysics[i])
		{
			m_objectsWithPhysics[i] = newProfile;
			registryLocation = i;
			return;
		}
	}

	//add on to the end if no empty slots were found
	m_objectsWithPhysics.push_back(newProfile);
	registryLocation = m_objectsWithPhysics.size() - 1;
}
void fatcore::physics_proc::purgePhysicsProfile(fatcore::physics_attrb* purgeProfile, const int REGISTRY_LOCATION)
{
	if (m_cleanUpComplete) return;

	//free the associated memory and set pointer to NULL;
	if (m_objectsWithPhysics[REGISTRY_LOCATION])
	{
		m_objectsWithPhysics[REGISTRY_LOCATION] = NULL;
	}
}
void fatcore::physics_proc::procframe()
{
	//incriment frame count
	m_frameCount++;

	//begin processing loop
	for (unsigned int i = 0; i < m_objectsWithPhysics.size(); ++i)
	{
		//do not process NULL pointers
		if (m_objectsWithPhysics[i] == NULL) continue;

		//local pointer to current physics_attrb profile
		physics_attrb* currentProfile = m_objectsWithPhysics[i];

		//improve performance by skiping non gravity or momentum effected objects
		if (!currentProfile->gravityEffect && !currentProfile->momentumEffect) continue;

		//NOTE: calculations are NOT intended as a perfect representation of physics, but as a way to make objects behave
		//      in a way that somewhat mimics the laws of the natural world. In other words, FUCK OFF!

		//process gravitational pull
		if (currentProfile->gravityEffect)
		{
			//process change in momentumY (gravitiational pull)
			if (currentProfile->weight != 0)
			{
				//reset fallstart if momentumY is 0 (not currently being effected by gravity) or 
				//incriment the fallstart count
				if (currentProfile->momentumY == 0)
				{
					currentProfile->fallstart = 0;
				}
				else
				{
					currentProfile->fallstart++;
				}

				const int TIMESTEP = currentProfile->fallstart % 30;
				const float WEIGHT_CALC = currentProfile->weight * 0.01;

				if (currentProfile->momentumY > 0) //falling
				{
					if (currentProfile->Location.height < m_gravpoint) //falling toward gravpoint
					{
						currentProfile->momentumY = 
							currentProfile->momentumY + (WEIGHT_CALC + TIMESTEP);
					}
					else //falling below the gravpoint
					{
						currentProfile->momentumY =
							currentProfile->momentumY - (WEIGHT_CALC + TIMESTEP);
					}
				}
				else if (currentProfile->momentumY < 0) //rising
				{
					if (currentProfile->Location.height > m_gravpoint) //rising toward gravpoint
					{
						currentProfile->momentumY =
							currentProfile->momentumY - (WEIGHT_CALC + TIMESTEP);
					}
					else //rising away from the gravpoint
					{
						currentProfile->momentumY =
							currentProfile->momentumY + (WEIGHT_CALC + TIMESTEP);
					}
				}
				

				//simulate terminal velocity
				if (currentProfile->momentumY > currentProfile->weight * 2)
				{
					currentProfile->momentumY = currentProfile->weight * 2;
				}
			}
		}

		//process momentum
		if (currentProfile->momentumEffect)
		{
			//reset the motionstart or incriment it
			if (currentProfile->momentumX == 0)
			{
				currentProfile->motionstart = 0;
			}
			else
			{
				currentProfile->motionstart++;
			}

			//momentum formulas
			(currentProfile->momentumX * -1) + (currentProfile->momentumX * (currentProfile->weight / currentProfile->motionstart));
			
			//calculate
			if (currentProfile->momentumY) //if object is floating
			{
				if ((currentProfile->motionstart % 40) == 0)
					currentProfile->momentumX = currentProfile->momentumX * 0.85;
			}
			else //if object is colliding with another
			{
				if ((currentProfile->motionstart % 40) == 0)
					currentProfile->momentumX = currentProfile->momentumX * 0.25;
			}
		}
		
		//keep object moving toward the gravpoint at all times
		if (currentProfile->Location.height < m_gravpoint && currentProfile->momentumY == 0)//not touching the gravpoint and set to 0
		{
			currentProfile->momentumY = 1;
		}
		else if (currentProfile->Location.height > m_gravpoint && currentProfile->momentumY == 0)//not touching the gravpoint and set to 0
		{
			currentProfile->momentumY = -1;
		}

		//apply changes to object location
		currentProfile->Location.y += currentProfile->momentumY; //gravity
		currentProfile->Location.height += currentProfile->momentumY;

		currentProfile->Location.x += currentProfile->momentumX; //momentum
		currentProfile->Location.width += currentProfile->momentumX;
	}
}
float fatcore::physics_proc::calcGive(float profile1, float profile2)
{
	//make sure the supplied values are in the correct range
	if (profile1 > 1) profile1 = 1;
	else if (profile1 < -1) profile1 = -1;

	if (profile2 > 1) profile2 = 1;
	else if (profile2 < -1) profile2 = -1;

	//make the calculation
	float give = profile1 + profile2;

	//check then return findings
	if (give > 1) give = 0.99;
	else if (give < -1) give = -0.99;

	return give * -1;
}

//placeholder
fatcore::physics_proc __PRIMARY_PHYSICS_PROC__;