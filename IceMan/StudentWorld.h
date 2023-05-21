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



		// constructing oil field and inserting iceman
		populateIce();
		populateBoulders();
		populateIceman();
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
	//void populateProtestors();

	double distanceToIceman();
	double objectDistance(int xPos, int yPos , Actor *otherActor); // returns euclidean distance to avoid populating objects close to one another
	bool invalidCoord(const int& x1, const int& y1);
	bool isThereIceBelow(int xPos, int yPos);
	void deleteInactiveActors();
	bool isThereIce(int xPos, int yPos);
	bool icemanOverlaps(int x, int y);
	bool isBlocked(int xPos, int yPos);
	bool isOverLappingActor(int xPos, int yPos, Actor* otherActor);
	bool isOverlappingIceman(int xPos, int yPos);
	void formatAndSetDisplayText();


	void updateGoldNuggets();
	void updateWaterSquirts();

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

};

#endif // STUDENTWORLD_H_
