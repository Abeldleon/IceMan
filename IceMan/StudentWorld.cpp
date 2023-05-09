#include "StudentWorld.h"
#include <string>
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
			icePtr[i][j] = new Ice(IID_ICE, i, j, GraphObject::right, 0.25, 3);
			icePtr[i][j]->setVisible(true);
		}
	}
}



void StudentWorld::populateBoulders() {

	numBouldersForLevel = min(getLevel() / 2 + 2, 9);


	for (int i = 0; i < numBouldersForLevel; i++) {

		int x = rand() % VIEW_WIDTH;
		int y = rand() % 59;


		actorPtr.push_back(new Boulder(x,y));
	}
}

int StudentWorld::min(int a, int b)
{
	return (a < b) ? a : b;
}
