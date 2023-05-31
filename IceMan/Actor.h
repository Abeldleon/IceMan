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
		ticksElapsed = 0;
		ticksAvailable = 0;
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

	virtual bool isCollidable() {
		return false;
	}
	virtual ~Actor() {
	
	}
protected:
	int ticksElapsed;
	int ticksAvailable;
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
	virtual ~Ice() {
		
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

	bool isCollidable() override {
		return true;
	}

	virtual ~Boulder() {}
private:
	State currentState;
	//int ticksElapsed;
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
	WhoCanPickUp getWhoCanPickUp() {
		return m_WhoCanPickUp;
	}
	virtual ~Acquirable() {

	}


private:
	WhoCanPickUp m_WhoCanPickUp;
	PermOrTemp m_PermOrTemp;
};

class WaterPuddle : public Acquirable {
public:
	WaterPuddle(int startX, int startY, int ticksAvailable, StudentWorld* sw) : Acquirable(IID_WATER_POOL, startX, startY, icemanCan, temporary, sw) {
		setVisible(true);
	}
	void doSomething() override;
	virtual ~WaterPuddle() {

	}

private:
	//int m_ticksAvailable; // likely controlled by StudentWorld. if not then have to make permanent / temporary goodies a base class
	// enum for state of pickuability? - pickupabble by iceman or protestor or neither
	// other state: permanent and temporary
};

class GoldNugget : public Acquirable {
public:
	GoldNugget(int startX, int startY, WhoCanPickUp who, PermOrTemp pt, StudentWorld* sw) : Acquirable(IID_GOLD, startX, startY, who, pt, sw) {
		if (who == icemanCan)
			setVisible(false);
		else if (who == protestorCan)
			setVisible(true);
	}
	// if in perm state, starts invisble and becomes visible when iceman within radius of 3
	void doSomething() override;
	virtual ~GoldNugget() {

	}
private:
	
};

class OilBarrel : public Acquirable {
public:
	OilBarrel(int startX, int startY, StudentWorld* sw) : Acquirable(IID_BARREL, startX, startY, icemanCan, permanent, sw) {
		setVisible(false);
	}
	// starts invisible and becomes visible when iceman within radius of 4
	virtual void doSomething();
	virtual ~OilBarrel() {

	}
private:

};

class SonarKit : public Acquirable {
public:
	SonarKit(int startX, int startY, StudentWorld* sw) : Acquirable(IID_SONAR, startX, startY, icemanCan, temporary, sw) {
		setVisible(true);

	}

	void doSomething();
	virtual ~SonarKit() {

	}
private:
	/*int m_ticksAvailable*/;
};

class Character : public Actor {
public:
	Character(int imageID, int startX, int startY, Direction dir, int hp, StudentWorld* sw) : Actor(imageID, startX, startY, dir, 1.0, 0, sw) {
		setVisible(true);
		m_healthPoints = hp;
	}
	int getHP() {
		return m_healthPoints;
	}
	virtual ~Character() {

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
		numBarrelsCollected = 0;
	}
	virtual void doSomething();
	bool invalidIcemanCoordinate(const int& x, const int& y);

	void increaseSonar() {
		sonarCharge++;
	}

	void increaseGoldNuggets() {
		goldNuggets++;
	}

	void increaseWaterSquirts() {
		waterSquirts++;
	}
	void increaseOilBarrels() {
		numBarrelsCollected++;
	}

	void userSquirt() {
		if (waterSquirts > 0) {
			waterSquirts--;
		}
	}

	void useSonar();

	void dropGold() {
		if (goldNuggets > 0) {
			goldNuggets--;
		}
	}

	int getSquirts() {
		return waterSquirts;
	}
	int getSonar() {
		return sonarCharge;
	}
	int getGold() {
		return goldNuggets;
	}
	int getBarrelsCollected() {
		return numBarrelsCollected;
	}
	virtual ~Iceman() {

	}

private:
	int waterSquirts;
	int sonarCharge;
	int goldNuggets;
	int numBarrelsCollected;
};

class Protestor : public Character {
public:
	enum StayOrLeave{stay, leave};
	Protestor(int id, int hp, int numSquaresToMove, int pTicks, int restTicks, StudentWorld* sw) : Character(id, 60, 60, left, hp, sw) {
		m_numSquaresToMove = numSquaresToMove;
		perpendicularTicks = pTicks;
		m_StayLeave = stay;
		protestorDelayTicks = restTicks;
		setVisible(true);
	}

	int getNumSquaresToMove() {
		return m_numSquaresToMove;
	}

	virtual void decreaseNumSquaresToMove() {
		if (m_numSquaresToMove > 0) {
			m_numSquaresToMove--;
		}
	}

	void setNumSquaresToMove(int m) {
		m_numSquaresToMove = m;
	}

	//perpencidualr ticks
	virtual void decreasePerpendicularTicks() {
		if (perpendicularTicks > 0) {
			perpendicularTicks--;
		}
	}

	void setPerpendicularTicks(int m) {
		perpendicularTicks= m;
	}

	int getPerpendicularTicks() {
		return perpendicularTicks;
	}


	virtual void decreaseProtestorDelayTicks() {
		if (protestorDelayTicks > 0) {
			protestorDelayTicks--;
		}
	}

	void setProtestorDelayTicks(int m) {
		protestorDelayTicks = m;
	}

	int getProtestorDelayTicks() {
		return protestorDelayTicks;
	}
	//resting ticks

	virtual ~Protestor() {

	}
private:
	StayOrLeave m_StayLeave;
	int m_numSquaresToMove;
	int perpendicularTicks;
	int protestorDelayTicks;
};

class HardcoreProtestor : public Protestor {

};

class RegularProtestor : public Protestor {
public:
	RegularProtestor(int numSquaresToMove, int numPerpendicularTicks, int restTicks, StudentWorld* sw) : Protestor(IID_PROTESTER, 5, numSquaresToMove, numPerpendicularTicks, restTicks, sw) {

	}


	void doSomething();
	virtual ~RegularProtestor() {

	}
};

class WaterSquirt : public Actor {
public:
	WaterSquirt(int x, int y, Direction dir, StudentWorld* sw) : Actor(IID_WATER_SPURT, x, y, dir, 1, 1.0, sw) {
		setVisible(true);
		distanceTravelled = 0;
	}
	int getDistanceTravelled() {
		return distanceTravelled;
	}
	void incrementDistanceTravelled() {
		distanceTravelled++;
	}
	virtual void doSomething();
private:
	int distanceTravelled;
};

#endif // ACTOR_H_
