#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


//////////LOading actors functions///////////
void StudentWorld::createIceMan(StudentWorld* world)
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
