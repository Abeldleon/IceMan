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
			}

		}

		actorPtr.push_back(new Boulder(x,y, this));
		invalidCoordinates.emplace_back(x, y);  // store coorinates in invalid so that GoldNugget and OilBarrel aren't populated nearby
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

double StudentWorld::objectDistance(int xPos, int yPos, Actor* otherActor)
{
	return sqrt(pow(xPos - otherActor->getX(), 2) + pow(yPos - otherActor->getY(), 2));
}
bool StudentWorld::invalidCoord(const int& x1,const int& y1) {
	//cerr << "size: " << invalidCoordinates.size() << endl;
	double dist = 0;
	for (unsigned int i = 0; i < invalidCoordinates.size(); i++) { // iterate thru vector containing pair of invalid coordinates
		//set dist equal to euclidean distance from generated coordinates to the i'th pair of invalid coordinates
		dist = sqrt(pow(invalidCoordinates[i].first - x1, 2) + pow(invalidCoordinates[i].second - y1, 2)); 
		//cerr << "dist: " << dist << endl;
		cerr << "invalids: (" << invalidCoordinates[i].first << ", " << invalidCoordinates[i].second
			<< ")		curr coord: (" << x1 << ", " << y1 << ")			dist: " << dist << endl;
		if (dist < 6) { // if dist is less than 6, return true to regenerate coordinates
			//cerr << "dist redo: " << dist << endl;
			//cerr << "x1: " << x1 << " " << "y1: " << y1 << endl;
			return true;
		}
		//else { // if dist is greater than 6, store these coordinates as invalid and return false to proceed
		//	invalidCoordinates.emplace_back(x1, y1);
		//	cerr << "emplacing:  x1: " << x1 << " " << "y1: " << y1 << endl;
		//	//cerr << "dist proceed: " << dist << endl;
		//	return false;
		//}

	}
	// if it hasn't returned a value at this point, there are no coordinates in invalidCoordinates
	// OR all checked coordinates are valid (dist > 6 for all stored coordinates)
	// so store these coordinates inside of invalidCoordinates and return false to proceed

	//cerr << "x1: " << x1 << " " << "y1: " << y1 << endl;
	cerr << "valid distance: " << dist << endl;
	invalidCoordinates.emplace_back(x1, y1);
	return false;

}

bool StudentWorld::isThereIceBelow(int xPos, int yPos)
{
	for (int i = 0; i < 4; i++)
	{
		if (icePtr[xPos + i][yPos] != nullptr)
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
		} while (invalidCoord(x, y)); /* keep generating if coordinates are invalid
									    (within euclidean distance of 6 within another
										object already populated on the map             */

	}
}