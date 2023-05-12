#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


<<<<<<< Updated upstream
//////////LOading actors functions///////////
void StudentWorld::createIceMan(StudentWorld* world)
=======

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

void StudentWorld::populateGold() {

}

double StudentWorld::objectDistance(int xPos, int yPos, Actor* otherActor)
{
	iceman = new IceMan(30, 60, world);
	iceman->setVisible(true);
}


int StudentWorld::init()
{
	createIceMan(this);
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{

	return GWSTATUS_CONTINUE_GAME;
}
