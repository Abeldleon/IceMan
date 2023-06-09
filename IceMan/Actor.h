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

	virtual bool isProtestor() { // gets overridden by protestor
		return false;
	}
	virtual bool isRegularProtestor() {
		return false;
	}
	virtual bool isHardcoreProtestor() {
		return false;
	}
	virtual void setLeaveState() {}

	virtual void setAnnoyed(bool a){}

	virtual void setStunnedRestingTicks ( int n ) {}

	virtual bool isBribed() {
		return false;
	}
	virtual void setBribed(bool a) {}

	virtual bool isSquirt() {
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
	void setVisibleIfGoodieClose();
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
	void decHP(int h) {
		m_healthPoints = m_healthPoints - h;
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

	void setAnnoyed(bool a) {
		annoyed = a;
		decHP(2);
	}
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
	bool annoyed;
};

class Protestor : public Character {
public:
	enum StayOrLeave{stay, leave};
	Protestor(int id, int hp, int numSquaresToMove, int pTicks, int restTicks, int stunnedTicks, StudentWorld* sw) : Character(id, 60, 60, left, hp, sw) {
		m_numSquaresToMove = numSquaresToMove;
		perpendicularTicks = pTicks;
		m_StayLeave = stay;
		protestorDelayTicks = restTicks;
		stunnedRestingTicks = stunnedTicks;
		stunnedTicksCounter = 0;
		shoutingDelayTicks = 15;
		setVisible(true);
		annoyed = false;
		bribed = false;
	}

	void doCommonProtestorStuff();

	bool isProtestor() {
		return true;
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

	void setLeaveState() {
		m_StayLeave = leave;
	}
	//resting ticks

	void setStunnedTicksCounter(int n) {
		stunnedTicksCounter = n;
	}

	void increaseStunnedTicksCounter() {
		stunnedTicksCounter++;
	}

	void setStunnedRestingTicks( int a) override{
		stunnedRestingTicks = a;
	}

	int getStunnedTicksCounter() {
		return stunnedTicksCounter;
	}

	int getStunnedRestingTicks() {
		return stunnedRestingTicks;
	}

	StayOrLeave getState() {
		return m_StayLeave;
	}
	
	void setBribed( bool a ) {
		bribed = a;
	}

	bool isBribed() {
		return bribed;
	}

	void setAnnoyed(bool a) {
		if (a) {
			decHP(1);
		}
		setStunnedTicksCounter(0);
		annoyed = a;
	}

	bool isAnnoyed() {
		return annoyed;
	}

	virtual ~Protestor() {

	}

	int getShoutingDelayTicks() {
		return shoutingDelayTicks;
	}
	void setShoutingDelayTicks(int n) {
		shoutingDelayTicks = n;
	}

	void increasingShoutingDelayTicks() {
		shoutingDelayTicks++;
	}

	void moveInThisDirection(GraphObject::Direction dir);

private:
	StayOrLeave m_StayLeave;
	int m_numSquaresToMove;
	int perpendicularTicks;
	int protestorDelayTicks;
	int stunnedRestingTicks;
	int stunnedTicksCounter;
	bool annoyed;
	bool bribed;
	int shoutingDelayTicks;
};

class HardcoreProtestor : public Protestor {
public:
	HardcoreProtestor(int numSquaresToMove, int numPerpendicularTicks, int restTicks, int stunnedTicks, StudentWorld* sw) : Protestor(IID_HARD_CORE_PROTESTER, 5, numSquaresToMove, numPerpendicularTicks, restTicks, stunnedTicks, sw) {

	}
	void doSomething();

	bool isHardcoreProtestor() {
		return true;
	}
	virtual ~HardcoreProtestor() {

	}
};

class RegularProtestor : public Protestor {
public:
	RegularProtestor(int numSquaresToMove, int numPerpendicularTicks, int restTicks, int stunnedTicks, StudentWorld* sw) : Protestor(IID_PROTESTER, 5, numSquaresToMove, numPerpendicularTicks, restTicks, stunnedTicks, sw) {

	}


	void doSomething();
	bool isRegularProtestor() {
		return true;
	}
	virtual ~RegularProtestor() {

	}
};

class WaterSquirt : public Actor {
public:
	WaterSquirt(int x, int y, Direction dir, StudentWorld* sw) : Actor(IID_WATER_SPURT, x, y, dir, 1.0, 1, sw) {
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

	bool isSquirt() {
		return true;
	}
private:
	int distanceTravelled;
};

#endif // ACTOR_H_
