#include "Actor.h"
#include "StudentWorld.h"
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

void Boulder::doSomething() { // still need to implement waiting state for 30 ticks. Will figure out how to ocunt ticks as we go along. Set isActive to false
	if (!getWorld()->isThereIceBelow(getX(), getY())) { //it can't see the function isThereIceBelow
		moveTo(getX(), getY() - 1); //even if you include "StudentWorld.h"
	}
}