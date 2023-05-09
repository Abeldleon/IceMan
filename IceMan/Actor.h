#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp



class Actor : public GraphObject { // ABC
public:
	Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
		: GraphObject(imageID, startX, startY, dir, size, depth) {
		isActive = true;
		
	}
	virtual void doSomething() = 0;
	bool getIsActive() { // to check is object is active
		return isActive;
	}
	void setInactive() { // to set an object inactive
		isActive = false;
	}
private:
	bool isActive; // for move() function in StudentWorld that will check whether each actor is active. Can be modified by each derived class
};

class Ice : public Actor {
public:
	Ice(int imageID, int startX, int startY,Direction dir, double size, unsigned int depth)
		: Actor(imageID, startX, startY, dir, size, depth) {}
	virtual void doSomething() {

	}
};

class Boulder : public Actor {
public: 

	Boulder(int startX, int startY) 
		:Actor(IID_BOULDER, startX, startY, down, 1.0, 1)
	{
		setVisible(true);
	}

	void Boulder::doSomething()
	{
	}


	virtual ~Boulder() {}
private:

};

class WaterPuddle : public Actor {
public:
	WaterPuddle(int startX, int startY, int ticksAvailable) : Actor(IID_WATER_POOL, startX, startY, right, 1.0, 2) {
		setVisible(true);
	}
private:
	//int m_ticksAvailable; // likely controlled by StudentWorld. if not then have to make permanent / temporary goodies a base class
	// enum for state of pickuability? - pickupabble by iceman or protestor or neither
	// other state: permanent and temporary
};

class Character : public Actor {

};

class Iceman : public Character {

};

class Protestor : public Character {

};

class HardcoreProtestor : public Protestor {

};

class RegularProtestor : public Protestor {

};



#endif // ACTOR_H_
