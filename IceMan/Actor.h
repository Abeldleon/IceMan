#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject { // ABC
public:
	Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* sw)
		: GraphObject(imageID, startX, startY, dir, size, depth), isActive(true), m_sw(sw){
		//isActive = true;
	}
	virtual void doSomething() = 0;
	bool getIsActive() { // to check is object is active
		return isActive;
	}
	void setInactive() { // to set an object inactive
		isActive = false;
	}
	StudentWorld* getWorld() {
		return m_sw;
	}
protected:
private:
	bool isActive; // for move() function in StudentWorld that will check whether each actor is active. Can be modified by each derived class
	StudentWorld* m_sw;

private:
};

class Ice : public Actor {
public:
	Ice(int imageID, int startX, int startY,Direction dir, double size, unsigned int depth, StudentWorld* sw)
		: Actor(imageID, startX, startY, dir, size, depth, sw) {
	}
	virtual void doSomething() {

	}
};

class Boulder : public Actor {
public: 
	enum State {
		Stable, Waiting, Falling
	};

	Boulder(int startX, int startY, StudentWorld* sw)
		:Actor(IID_BOULDER, startX, startY, down, 1.0, 1, sw)
	{
		setVisible(true);
		currentState = Stable;
		ticksElapsed = 0;
	}

	virtual void doSomething();
	void setState(State state) {
		currentState = state;
	}
	State getState() {
		return currentState;
	}
	virtual ~Boulder() {}
private:
	State currentState;
	int ticksElapsed;
};

class Acquirable : public Actor {
public:
	enum WhoCanPickUp {	icemanCan = 0, protestorCan = 1 };
	enum PermOrTemp { permanent = 0, temporary = 1};
	Acquirable(int imageID, int startX, int startY, WhoCanPickUp who, PermOrTemp pt, StudentWorld* sw) : Actor(imageID, startX, startY, right, 1.0, 0, sw) {
		m_WhoCanPickUp = who;
		m_PermOrTemp = pt;
	}
	void setWhoCanPickUp(WhoCanPickUp w) {
		m_WhoCanPickUp = w;
	}
	void setPermOrTemp(PermOrTemp p) {
		m_PermOrTemp = p;
	}



private:
	int m_WhoCanPickUp;
	int m_PermOrTemp;
};

class WaterPuddle : public Acquirable {
public:
	WaterPuddle(int startX, int startY, int ticksAvailable, StudentWorld* sw) : Acquirable(IID_WATER_POOL, startX, startY, icemanCan, temporary, sw) {
		setVisible(true);
		m_ticksAvailable = 0;
	}
	virtual void doSomething();

private:
	int m_ticksAvailable; // likely controlled by StudentWorld. if not then have to make permanent / temporary goodies a base class
	// enum for state of pickuability? - pickupabble by iceman or protestor or neither
	// other state: permanent and temporary
};

class GoldNugget : public Acquirable {
public:
	GoldNugget(int startX, int startY, WhoCanPickUp who, PermOrTemp pt, StudentWorld* sw) : Acquirable(IID_GOLD, startX, startY, who, pt, sw) {
		setVisible(true); 
	}
	// if in perm state, starts invisble and becomes visible when iceman within radius of 3
	virtual void doSomething() {

	}
private:
	
};

class OilBarrel : public Acquirable {
public:
	OilBarrel(int startX, int startY, StudentWorld* sw) : Acquirable(IID_BARREL, startX, startY, icemanCan, permanent, sw) {
		setVisible(true);
	}
	// starts invisible and becomes visible when iceman within radius of 4
	virtual void doSomething() {

	}
private:

};

class SonarKit : public Acquirable {
public:
	SonarKit(int startX, int startY, StudentWorld* sw) : Acquirable(IID_SONAR, startX, startY, icemanCan, temporary, sw) {
		setVisible(true);
	}

	void doSomething();
private:
	int m_ticksAvailable;
};

class Character : public Actor {
public:
	Character(int imageID, int startX, int startY, Direction dir, int hp, StudentWorld* sw) : Actor(imageID, startX, startY, dir, 1.0, 0, sw) {
		setVisible(true);
		m_healthPoints = hp;
	}
private:
	int m_healthPoints;
};

class Iceman : public Character {
public:
	Iceman(StudentWorld* sw) : Character(IID_PLAYER, 30, 60, right, 10, sw) {
		setVisible(true);
		waterSquirts = 5;
		sonarCharge = 1;
		goldNuggets = 0;
	}
	virtual void doSomething();

private:
	int waterSquirts;
	int sonarCharge;
	int goldNuggets;

};

class Protestor : public Character {

};

class HardcoreProtestor : public Protestor {

};

class RegularProtestor : public Protestor {

};



#endif // ACTOR_H_
