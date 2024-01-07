/***************************************************************************************************
FatBoy Engine -- physics

Purpose: this will define the majority of the objects and functions that encapsulate the physics
module of the Fatboy Engine
***************************************************************************************************/

#include "core.h"

namespace fatcore
{
#ifndef __PHYSICS_HEADER_1__
#define __PHYSICS_HEADER_1__

	class physics_proc;

	bool locIntersect(const loc LOC1, const loc LOC2, loc* intersectLoc = NULL); //used to determine if 2 location objects are overlapping
	
	struct physics_attrb
	{
		physics_attrb();
		~physics_attrb();

		bool gravityEffect; //will gravity effect this object?
		bool momentumEffect; //false, object will stop immediatly when not recieving input; true, it will continue and gradually slow
		int weight; //how heavy an object is, i.e. how quickly it is pulled by gravity
		float give; //how much give an object has, neg numbers cushon objects, pos numbers bounce objects, 0 is neutral, -1 to 1
		float friction; //how much friction the object has, range -1.0 to 1.0, negitive values cause more slide, positive cause less.
		int momentumX; //(momentum) how fast is the object moving on the X axis
		int momentumY; //(gravitional pull) how fast is the object moving on the Y axis
		long unsigned int fallstart; //timestamp the object began falling toward gravitational point
		long unsigned int motionstart; //timestamp the object began moving on the X axis
		loc Location; //location of the object being effected by physics
		std::vector<physics_attrb*> colliding_objects; //vector of all the objects colliding with current profile

	private:
		int m_registryLocation;
	};

	class physics_proc
	{
	public:
		physics_proc();
		~physics_proc();
		inline void setgravpoint(const int gravitational_axis) { m_gravpoint = gravitational_axis; }
		inline bool cleanUpComplete() { return m_cleanUpComplete; }
		inline long unsigned int getFramecount() { return m_frameCount; }
		float calcGive(float profile1, float profile2);
		void registerPhysicsProfile(physics_attrb* newProfile, int& registryLocation);
		void purgePhysicsProfile(physics_attrb* purgeProfile, const int REGISTRY_LOCATION);
		virtual void procframe();

	private:
		static int m_instancecount;
		static int m_gravpoint;
		static long unsigned int m_frameCount;
		static bool m_cleanUpComplete;
		static std::vector<physics_attrb*> m_objectsWithPhysics;
	};

#endif
}