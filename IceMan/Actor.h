#ifndef ACTOR_H_
#define ACTOR_H_


#include "GameConstants.h"
#include "GraphObject.h"
//#include "StudentWorld.h"

class StudentWorld; ///foward declaration
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


class Actor : public GraphObject
{
private:
	bool alive;
	StudentWorld* world;
public:
	//page 24
	Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* world_passed)
		:GraphObject(imageID, startX, startY, dir, size, depth)
	{
		world = world_passed;
		alive = true;
	}


	virtual void doSomething() = 0;

	//Accessors
	StudentWorld* getWorld() {
		return world;
	}

	bool isAlive() {
		return alive;
	}

	// Mutators
	void setDead() {
		alive = false;
	}




	virtual ~Actor() {}
};


class IceMan : public Actor
{
public:
	IceMan(int startX, int startY, StudentWorld* world_passed)
		:Actor(IID_PLAYER, startX, startY, right, 1.0, 0, world_passed) {

	}

	void doSomething();

	~IceMan(){}
};


#endif  //ACTOR_H_
