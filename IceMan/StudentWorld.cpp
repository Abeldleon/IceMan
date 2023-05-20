#include "StudentWorld.h"
#include <string>
//#include <cstdlib>
//#include <cmath>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
void StudentWorld::populateIce() {
	for (int i = 0; i < VIEW_WIDTH; i++) {
		//icePtr[i][0] = new Ice(IID_ICE, )
		for (int j = 0; j < ICE_HEIGHT; j++) {
			if (i >= 30 && i <= 33 && j >= 4)
				break;
			icePtr[i][j] = new Ice(IID_ICE, i, j, GraphObject::right, 0.25, 3, this);
			icePtr[i][j]->setVisible(true);
		}
	}
}



void StudentWorld::populateBoulders() {

	numBouldersForLevel = min(getLevel() / 2 + 2, 9);


	for (int i = 0; i < numBouldersForLevel; i++) {

		//boulders might overlap :: fix with objectDistance()


		int x = 0;
		int y = 0;
		bool overlapping = false;

		do {
			x = rand() % 60;
			y = rand() % 56;

			if (i >= 1) { //if another boulder has been created
				overlapping = (objectDistance(x, y, actorPtr[0]) < 6);
			}
			
		} while (x >= 30 - 4 && x <= 33 || y < 20|| overlapping); //boulders wont apper in middle path
		//generateRandomLocation(x, y, isBoulder); // doesn't work because of invalidCoord() that it calls. I think it's better to keep them seperate

		cerr << "Boulder " << i << " x: " << x << " y: " << y << endl;


		for (int j = 0; j < 4; j++)
		{
			for(int k = 0; k < 4; k++)
			{
				delete icePtr[x+j][y + k];
				icePtr[x + j][y + k] = nullptr;
			}

		}

		actorPtr.push_back(new Boulder(x,y, this));
		//invalidCoordinates.emplace_back(x, y);  // store coorinates in invalid so that GoldNugget and OilBarrel aren't populated nearby
	}

	actorPtr.push_back(new Boulder(30, 50, this));
}



void StudentWorld::populateIceman() {
	icemanPtr = new Iceman(this);
}

void StudentWorld::populateGold(GoldNugget::WhoCanPickUp w, GoldNugget::PermOrTemp pt) {
	numGoldForLevel = max(5 - (getLevel()) / 2, 2); // should be at beginning of init. same thing, doesn't matter
	int x = 0;
	int y = 0;
	//cerr << numGoldForLevel << endl;
	for (int i = 0; i < numGoldForLevel; i++) {

		if (pt == GoldNugget::permanent) {
			// TODO: generate random location for numGoldForLevel and create new GoldNugget at that location
			generateRandomLocation(x, y, isGoldOrOilBarrel);
			actorPtr.push_back(new GoldNugget(x, y, w, pt, this));
			//cerr << "Gold " << i << " x: " << x << " y: " << y << endl;

		}
		else if (pt == GoldNugget::temporary) { // called when iceman drops gold
			// TODO: populate gold at location that iceman is at
		}

	}

}

void StudentWorld::populateOilBarrels() {
	int numOilBarrels = min(2 + getLevel(), 21);
	int x = 0;
	int y = 0;
	for (int i = 0; i < numOilBarrels; i++) {
		generateRandomLocation(x, y, isGoldOrOilBarrel);
		actorPtr.push_back(new OilBarrel(x, y, this));
	}
}


void StudentWorld::populateSonarKitAndWaterPool()
{
	probForWaterPoolOrSonar = getLevel() * 25 + 300;
	int num = rand() % probForWaterPoolOrSonar;
	int chance = rand() % 5;

	if (num == 0) {
		if (chance > 0) {
			int x = 0;
			int y = 0;
			generateRandomLocation(x, y, isWaterPuddle);
			actorPtr.push_back(new WaterPuddle(x, y, 20, this));
			invalidCoord(x, y);
		}
		else {
			actorPtr.push_back(new SonarKit(0, 60, this));
		}
	}

}


double StudentWorld::objectDistance(int xPos, int yPos, Actor* otherActor)
{
	return sqrt(pow(xPos - otherActor->getX(), 2) + pow(yPos - otherActor->getY(), 2));
}
bool StudentWorld::invalidCoord(const int& x1,const int& y1) {
	double dist = 0;


	for (unsigned int i = 0; i < actorPtr.size(); i++) { // iterate thru vector containing pair of invalid coordinates
		//set dist equal to euclidean distance from generated coordinates to the i'th pair of invalid coordinates
		dist = sqrt(pow(actorPtr[i]->getX() - x1, 2) + pow(actorPtr[i]->getY() - y1, 2));
		if (dist < 6) { // if dist is less than 6, return true to regenerate coordinates
			return true;
		}
	}
	// if it hasn't returned a value at this point, there are no coordinates in invalidCoordinates
	// OR all checked coordinates are valid (dist > 6 for all stored coordinates)
	// so store these coordinates inside of invalidCoordinates and return false to proceed
	//invalidCoordinates.emplace_back(x1, y1);
	return false;
}

bool StudentWorld::isThereIceBelow(int xPos, int yPos)
{
	for (int i = 0; i < 4; i++)
	{
		if (icePtr[xPos + i][yPos - 1] != nullptr) // should be yPos - 1?
		{
			return true;
		}
	}
	return false;
}

int StudentWorld::min(int a, int b)
{
	return (a < b) ? a : b;
}

int StudentWorld::max(int a, int b)
{
	return (a > b) ? a : b;
}

void StudentWorld::generateRandomLocation(int& x, int& y, ActorType at) {
	if (at == isBoulder) { // is unused because of runtime issues. Will remove this and enum type if can't fix
		do {
			x = rand() % 60; // generate random coordinates
			y = rand() % 56;
			//cerr << "y gen: " << y << endl;
		} while (invalidCoord(x, y) || y < 20); /* keep generating if coordinates are invalid
												   (within euclidean distance of 6 within another
											    	object already populated on the map) OR if y < 20  */
				
	}
	else if (at == isGoldOrOilBarrel) { 
		do {
			x = rand() % 60; // generate random coordinates
			y = rand() % 56;
		} while ((x > 26 && x < 34) || invalidCoord(x, y)); /* keep generating if coordinates are invalid
									    (within euclidean distance of 6 within another
										object already populated on the map             */
	}
	else if (at == isWaterPuddle) {
		do {
			x = rand() % 60; // generate random coordinates
			y = rand() % 56;
		} while (isThereIce(x, y) || invalidCoord(x,y));
	}
}

void StudentWorld::deleteInactiveActors() {
	// Removes objects that are inactive from the actorPtr vector
	for (int i = 0; i < actorPtr.size(); i++) {
		if (!actorPtr[i]->getIsActive()) {
			actorPtr[i]->setVisible(false);
			actorPtr.erase(actorPtr.begin() + i);
		}
	}
}

bool StudentWorld::isThereIce(int xPos, int yPos)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++) {
			if (icePtr[xPos+i][yPos+j]) {
				cerr << "There is ice!!" << endl;
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::icemanOverlaps(int x, int y) {
	// delete the overlapping ice accordint to the direction that the iceman is facing

	if (isThereIce(x, y) && icemanPtr->getDirection() == Iceman::left) { 
		for (int i = 0; i < 4; i++) {
			if (icePtr[x][y + i] == nullptr) // to prevent double deletion
				continue;
			cerr << "deleting left coordinates: " << x << " " << y + i << endl;
			delete icePtr[x][y + i];
			icePtr[x][y + i] = nullptr;
		}
		return true;

	}
	else if (isThereIce(x, y) && icemanPtr->getDirection() == Iceman::right) {
		for (int i = 0; i < 4; i++) {
			if (icePtr[x + 3][y + i] == nullptr)
				continue;
			cerr << "deleting right coordinates: " << x + 4 << " " << y + i << endl;
			delete icePtr[x + 3][y + i];
			icePtr[x + 3][y + i] = nullptr;
		}
		return true;

	}
	else if (isThereIce(x, y) && icemanPtr->getDirection() == Iceman::down) {
		for (int i = 0; i < 4; i++) {
			if (icePtr[x + i][y] == nullptr)
				continue;
			cerr << "deleting down coordinates: " << x + i << " " << y << endl;
			delete icePtr[x + i][y];
			icePtr[x + i][y] = nullptr;
		}
		return true;

	}
	else if (isThereIce(x, y) && icemanPtr->getDirection() == Iceman::up) {
		for (int i = 0; i < 4; i++) {
			if (icePtr[x + i][y + 3] == nullptr)
				continue;
			cerr << "deleting right coordinates: " << x + i << " " << y + 3 << endl;
			delete icePtr[x + i][y + 3];
			icePtr[x + i][y + 3] = nullptr;
		}
		return true;

	}
	return false;
}
