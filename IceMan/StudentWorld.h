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
	enum ActorType { isBoulder, isGoldOrOilBarrel, isWaterPuddle}; // for populating in init
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
		numBouldersForLevel = 0; // changed in populate functions
		numGoldForLevel = 0;
		numOilBarrelsForLevel = 0;
		probForWaterPoolOrSonar = 0;
		numProtestorsForLevel = 0;
		probForHardcore = 0;
		ticks = 0;
		tickAtWhichLastProtestorWasAdded = 0;
		numTicksToAddNewProtestor = 0;
		numProtestors = 0;

		icemanPtr = nullptr;
		for (int i = 0; i < VIEW_WIDTH; i++) { // initialize all iceptrs to null
			for (int j = 0; j < VIEW_HEIGHT; j++) {
				icePtr[i][j] = nullptr;
			}
		}
	}


	virtual int init()
	{
		// initialize data structures to keep track of game's virtual world
		numProtestorsForLevel = min(15, 2 + getLevel() * 1.5); // put here so it's not calculated every time populate is called in move
		numTicksToAddNewProtestor = max(25, 200 - getLevel());
		protestorDelayTicks = max(0, 3 - getLevel() / 4);
		stunnedProtestorTicks = max(50, 100 - getLevel() * 10);
		actorPtr.push_back(new RegularProtestor(1, 200, protestorDelayTicks, stunnedProtestorTicks, this)); // populate first protestor at first tick


		// constructing oil field and inserting iceman
		populateIce();
		populateBoulders();
		populateIceman();
		populateProtestor();
		//// TODO
		populateGold(GoldNugget::icemanCan, GoldNugget::permanent);
		populateOilBarrels();

		//populateWaterSquirt(); // this is probably in move since it only is populated once iceman shoots
		//populateSonarKit(); // i think this also occurs later in the game
		//populateProtestors();
		//delete icePtr[29][59];
		//delete icePtr[29][60];
		//delete icePtr[29][61];
		//delete icePtr[29][62];
		//delete icePtr[29][58];
		//delete icePtr[30][0];
		//delete icePtr[29][60];

		return GWSTATUS_CONTINUE_GAME;
	}

	void generateRandomLocation(int &x, int &y, ActorType at); // generate random coordinates that aren't within a euclidean 
												 // distance of 6 of already generated coordinates

	void populateIce();
	void populateBoulders();
	void populateIceman();
	void populateGold(GoldNugget::WhoCanPickUp w, GoldNugget::PermOrTemp pt);
	void populateOilBarrels();


	// TODO 
	void populateSonarKitAndWaterPool(); 
	void populateWaterSquirt();
	void populateProtestor();

	double distanceToIceman();
	double objectDistance(int xPos, int yPos , Actor *otherActor); // returns euclidean distance to avoid populating objects close to one another
	bool invalidCoord(const int& x1, const int& y1);
	bool isThereIceInThisDirection(int xPos, int yPos, GraphObject::Direction direction);
	void deleteInactiveActors();
	bool isThereIce(int xPos, int yPos);
	bool icemanOverlapsIce(int x, int y);
	bool isBlocked(int xPos, int yPos);
	bool isOverLappingActor(int xPos, int yPos, Actor* otherActor);
	bool isOverlappingIceman(int xPos, int yPos);
	void formatAndSetDisplayText();
	bool makeActorsVisible(int x, int y, bool sonarUsed);
	void killIceman();
	Actor * getOverlappingProtestor(int xPos, int yPos);
	bool isOverlappingSquirt(int xPos, int yPos);
	void updateGoldNuggets();
	void updateWaterSquirts();
	void updateSonarCharge();
	void updateOilBarrel();
	bool overlapsAnyActor(int xPos, int yPos); //for squirt to check if it overlaps any other actor
	GraphObject::Direction lineOfSightToIceman(int protestorX, int protestorY) ;

	virtual int move()
	{
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		//decLives();
		//return GWSTATUS_PLAYER_DIED;
		//TODO: 
		//updateStatusText
		//callDoSomethingForEveryActor
		if (icemanPtr->getIsActive()) {
			ticks++;
			if (icemanPtr->getBarrelsCollected() == numOilBarrelsForLevel) {
				return GWSTATUS_FINISHED_LEVEL;
			}
			formatAndSetDisplayText();
			populateSonarKitAndWaterPool();

			icemanPtr->doSomething();
			for (Actor* a : actorPtr) {
				if (!a->getIsActive() || a == nullptr)
					continue;
				a->doSomething();
			}
			deleteInactiveActors();


			return GWSTATUS_CONTINUE_GAME;
		}
		else {
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
	}

	virtual void cleanUp()
	{
		delete icemanPtr;
		//for (Actor* a : actorPtr) { //CRASHES
		//	if (!a->getIsActive() || a == nullptr)
		//		continue;
		//	//std::cerr << a->getID() << std::endl;
		//	//delete a;

		//}
		for (int k = 0; k < actorPtr.size();) {
			if (actorPtr[k] == nullptr) {
				continue;
			}
			delete actorPtr[k];
			actorPtr[k] = nullptr;
			actorPtr.erase(actorPtr.begin() + k);
		}
		//delete actorPtr[0];
		//delete actorPtr[1];
		//delete actorPtr[2];
		//delete actorPtr[3];
		//delete actorPtr[4];
		//delete actorPtr[5];

		for (int i = 0; i < VIEW_WIDTH; i++) {
			for (int j = 0; j < ICE_HEIGHT; j++) {
				if (icePtr[i][j] == nullptr)
					continue;
				delete icePtr[i][j];
			}
		}


	}

	int min(int a, int b);
	int max(int a, int b);

	~StudentWorld() {
		delete icemanPtr;
		//for (Actor* a : actorPtr) { // CRASHES
		//	if (!a->getIsActive() || a == nullptr)
		//		continue;
		//	std::cerr << a->getID() << std::endl;
		//	delete a;
		//}
		for (int k = 0; k < actorPtr.size();) {
			if (actorPtr[k] == nullptr) {
				continue;
			}
			delete actorPtr[k];
			actorPtr[k] = nullptr;
			actorPtr.erase(actorPtr.begin() + k);
		}

		for (int i = 0; i < VIEW_WIDTH; i++) {
			for (int j = 0; j < ICE_HEIGHT; j++) {
				if (icePtr[i][j] == nullptr)
					continue;
				delete icePtr[i][j];
			}
		}

	}
private:
	std::vector<Actor*> actorPtr;
	Iceman* icemanPtr;
	Ice* icePtr[VIEW_WIDTH][VIEW_HEIGHT];

	std::vector<std::pair<int, int>> invalidCoordinates;

	int numBouldersForLevel;
	int numGoldForLevel;
	int probForWaterPoolOrSonar;
	int numOilBarrelsForLevel;
	int boulderX;
	int boulderY;

	int ticks;
	int protestorDelayTicks;
	int tickAtWhichLastProtestorWasAdded;
	int numTicksToAddNewProtestor;
	int numProtestorsForLevel;
	int probForHardcore;
	int numProtestors;
	int stunnedProtestorTicks;
};

#endif // STUDENTWORLD_H_
