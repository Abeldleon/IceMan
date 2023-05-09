#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>

#include "Actor.h"
//#include "GraphObject.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

//class Actor;
//class Iceman;
//class Ice;

const int ICE_HEIGHT = 60;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
		icemanPtr = nullptr;
		
	}

	virtual int init()
	{
		// initialize data structures to keep track of game's virtual world
		// construct a new oil field with ice, barrels, boulder, gold, etc
		// allocate and insert an iceman objet into the game world
		populateIce();
		populateBoulders();

		//// TODO
		//populateIceman();
		//populateGold();
		//populateWaterPool();
		//populateWaterSquirt(); // this is probably in move since it only is populated once iceman shoots
		//populateOilBarrels();
		//populateSonarKit(); // i think this also occurs later in the game
		//populateProtestors();

		

		return GWSTATUS_CONTINUE_GAME;
	}
	void populateIce();
	void populateBoulders();


	// TODO 
	//void populateIceman();
	//void populateGold();
	//void populateWaterPool();
	//void populateWaterSquirt(); 
	//void populateOilBarrels();
	//void populateSonarKit(); 
	//void populateProtestors();

	// double objectDistance() // returns euclidean distance to avoid populating objects close to one another

	virtual int move()
	{
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		//decLives();
		//return GWSTATUS_PLAYER_DIED;
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual void cleanUp()
	{
	}

	int min(int a, int b);
	int max(int a, int b);
private:
	std::vector<Actor*> actorPtr;
	Iceman* icemanPtr;
	Ice* icePtr[VIEW_WIDTH][ICE_HEIGHT];

	int numBouldersForLevel;
};

#endif // STUDENTWORLD_H_
