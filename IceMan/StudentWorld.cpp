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
		for (int j = 0; j <= ICE_HEIGHT; j++) {
			if (i >= 30 && i <= 33 && j >= 4 && j <= 59)
				break;
			icePtr[i][j] = new Ice(IID_ICE, i, j, GraphObject::right, 0.25, 3);
			icePtr[i][j]->setVisible(true);
		}
	}
}
