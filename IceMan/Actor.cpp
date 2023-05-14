#include "Actor.h"
#include "StudentWorld.h"
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

void Boulder::doSomething() {
		if (getWorld()->isThereIceBelow(3, 4)) { //it can't see the function isThereIceBelow
		moveTo(getX(), getY() - 1); //even if you include "StudentWorld.h"
	}
}