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

		//boulders might overlap :: fix with objectDistance()


		int x;
		int y;
		bool overlapping = false;

		do {
			x = rand() % 60;
			y = rand() % 56;

			if (i >= 1) { //if another boulder has been created
				overlapping = (objectDistance(x, y, actorPtr[0]) < 6);
			}
			
		} while (x >= 30 - 4 && x <= 33 || overlapping); //boulders wont apper in middle path

		cerr << "Boulder " << i << " x: " << x << " y: " << y << endl;


		for (int j = 0; j < 4; j++)
		{
			for(int k = 0; k < 4; k++)
			{
				delete icePtr[x+j][y + k];
			}

		}

		actorPtr.push_back(new Boulder(x,y));
	}

	actorPtr.push_back(new Boulder(30, 50));
}



void StudentWorld::populateIceman() {
	icemanPtr = new Iceman();
}

void StudentWorld::populateGold(GoldNugget::WhoCanPickUp w, GoldNugget::PermOrTemp pt) {
	numGoldForLevel = max(5 - (getLevel()) / 2, 2);
	//cerr << numGoldForLevel << endl;
	int x = 0;
	int y = 0;

}

double StudentWorld::objectDistance(int xPos, int yPos, Actor* otherActor)
{
	return sqrt(pow(xPos - otherActor->getX(), 2) + pow(yPos - otherActor->getY(), 2));
}


int StudentWorld::min(int a, int b)
{
	return (a < b) ? a : b;
}

int StudentWorld::max(int a, int b)
{
	return (a > b) ? a : b;
}
