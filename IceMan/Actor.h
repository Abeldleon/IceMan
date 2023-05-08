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
		: Actor(imageID, startX, startY, dir, size, depth) {
		dir = right;
		
	}
	virtual void doSomething() {

	}
};

class Boulder : public Actor {

};

class WaterPuddle : public Actor {

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
