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
			actorPtr.push_back(new GoldNugget(icemanPtr->getX(), icemanPtr->getY(), w, pt, this));
		}

	}

}

void StudentWorld::populateOilBarrels() {
	numOilBarrelsForLevel = min(2 + getLevel(), 21);
	int x = 0;
	int y = 0;
	for (int i = 0; i < numOilBarrelsForLevel; i++) {
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

bool StudentWorld::isBlocked(int xPos, int yPos) { // can just use boulder location
	for (int k = 0; k < actorPtr.size(); k++) {
		if (actorPtr[k]->isCollidable()) {
		
			if (isOverLappingActor(xPos, yPos, actorPtr[k])) {
				return true;
			}
		}
	}
	return false;
}



bool StudentWorld::isOverLappingActor(int xPos, int yPos, Actor* otherActor)
{
	for (int i = -3; i < 3; i++) {
		for (int k = -2; k < 4; k++) {
			if (sqrt(pow(xPos - (otherActor->getX() + i), 2) + pow(yPos - (otherActor->getY() + k), 2)) == 0) {			
				cerr << "is overlapping" << endl;
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::isOverlappingIceman(int xPos, int yPos)
{
	//if (isOverLappingActor(xPos, yPos, icemanPtr)) {
	//	return true;
	//}
	//return false;
	if (objectDistance(xPos, yPos, icemanPtr) < 4) {
		return true;
	}
	return false;
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
			delete actorPtr[i];
			actorPtr[i] = nullptr;
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
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::icemanOverlapsIce(int x, int y) {
	// delete the overlapping ice accordint to the direction that the iceman is facing

	if (isThereIce(x, y) && icemanPtr->getDirection() == Iceman::left) { 
		for (int i = 0; i < 4; i++) {
			if (icePtr[x][y + i] == nullptr) // to prevent double deletion
				continue;
			delete icePtr[x][y + i];
			icePtr[x][y + i] = nullptr;
		}
		return true;

	}
	else if (isThereIce(x, y) && icemanPtr->getDirection() == Iceman::right) {
		for (int i = 0; i < 4; i++) {
			if (icePtr[x + 3][y + i] == nullptr)
				continue;
			delete icePtr[x + 3][y + i];
			icePtr[x + 3][y + i] = nullptr;
		}
		return true;

	}
	else if (isThereIce(x, y) && icemanPtr->getDirection() == Iceman::down) {
		for (int i = 0; i < 4; i++) {
			if (icePtr[x + i][y] == nullptr)
				continue;
			delete icePtr[x + i][y];
			icePtr[x + i][y] = nullptr;
		}
		return true;

	}
	else if (isThereIce(x, y) && icemanPtr->getDirection() == Iceman::up) {
		for (int i = 0; i < 4; i++) {
			if (icePtr[x + i][y + 3] == nullptr)
				continue;
			delete icePtr[x + i][y + 3];
			icePtr[x + i][y + 3] = nullptr;
		}
		return true;

	}
	return false;
}


void StudentWorld::formatAndSetDisplayText() {
	int level = getLevel();
	int lives = getLives();
	int health = icemanPtr->getHP() * 10;
	int squirts = icemanPtr->getSquirts();
	int gold = icemanPtr->getGold();
	int barrelsLeft = numOilBarrelsForLevel - icemanPtr->getBarrelsCollected();
	int sonar = icemanPtr->getSonar();
	int score = getScore();
	string s = "Lvl: " + std::to_string(level) + " Lives: " + std::to_string(lives) +
		" Hlth: " + std::to_string(health) + "% Wtr: " + std::to_string(squirts) + 
		" Gld: " + std::to_string(gold) + " Oil Left: " + std::to_string(barrelsLeft) +
		" Sonar: " + std::to_string(sonar) + " Scr: " + std::to_string(score);
	setGameStatText(s);
}

bool StudentWorld::makeActorsVisible(int x, int y, bool sonarUsed)
{
	if (sonarUsed) {
		for (Actor* a : actorPtr) {
			if (objectDistance(x, y, a) < 12.0) {
				cerr << "sonar made visible" << endl;
				a->setVisible(true);
			}
		}
		return true;
	}
	else if (!sonarUsed) { // iceman walks close by to gold or oil
		if (objectDistance(x, y, icemanPtr) <= 4.0) {
			cerr << "walking by made visible" << endl;
			return true;
		}
	}
	return false;
}


void StudentWorld::updateGoldNuggets() //GoldNugget::doSomething() calls this when it overlaps iceman
{
	icemanPtr->increaseGoldNuggets();
}

void StudentWorld::updateWaterSquirts()
{
	icemanPtr->increaseWaterSquirts();
}

void StudentWorld::updateSonarCharge()
{
	icemanPtr->increaseSonar();
}
void StudentWorld::updateOilBarrel()
{
	icemanPtr->increaseOilBarrels();
}

void StudentWorld::populateWaterSquirt() { // is called by Iceman when spacebar is pressed
	// TODO: need to do same for boulder. Will have to make boulder coordinates a data member
	if (icemanPtr->getDirection() == Iceman::right) {
		if (isThereIce(icemanPtr->getX() + 4, icemanPtr->getY())) { // if theres ice within specified distance make shooting noise but don't populate (so it won't damage protestors
			playSound(SOUND_PLAYER_SQUIRT);
			return;
		}
		actorPtr.push_back(new WaterSquirt(icemanPtr->getX() + 4, icemanPtr->getY(), icemanPtr->getDirection(), this)); // populate in front of iceman
	}
	else if (icemanPtr->getDirection() == Iceman::left) {
		if (isThereIce(icemanPtr->getX() - 4, icemanPtr->getY())) {
			playSound(SOUND_PLAYER_SQUIRT);
			return;
		}
		actorPtr.push_back(new WaterSquirt(icemanPtr->getX() - 4, icemanPtr->getY(), icemanPtr->getDirection(), this));
	}
	else if (icemanPtr->getDirection() == Iceman::up) {
		if (isThereIce(icemanPtr->getX(), icemanPtr->getY() + 4)) {
			playSound(SOUND_PLAYER_SQUIRT);
			return;
		}
		actorPtr.push_back(new WaterSquirt(icemanPtr->getX(), icemanPtr->getY() + 4, icemanPtr->getDirection(), this));
		}
	else if (icemanPtr->getDirection() == Iceman::down) {
		if (isThereIce(icemanPtr->getX(), icemanPtr->getY() - 4)) {
			playSound(SOUND_PLAYER_SQUIRT);
			return;
		}
		actorPtr.push_back(new WaterSquirt(icemanPtr->getX(), icemanPtr->getY() - 4, icemanPtr->getDirection(), this));
	}

}

void StudentWorld::populateProtestor() {
	//if (numProtestors == 0) { // populate first protestor at first tick. put in init because wouldn't be exactly right to put here
	//	actorPtr.push_back(new RegularProtestor(1, this)); 
	//	return;
	//}
	if (numProtestors < numProtestorsForLevel && (ticks - tickAtWhichLastProtestorWasAdded > numTicksToAddNewProtestor)) {
		actorPtr.push_back(new RegularProtestor(1, this));
	}
}