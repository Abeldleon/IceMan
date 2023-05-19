#include "Actor.h"
#include "StudentWorld.h"
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

void Boulder::doSomething() { // still need to implement waiting state for 30 ticks. Will figure out how to ocunt ticks as we go along. Set isActive to false
	if (!getWorld()->isThereIceBelow(getX(), getY()) && getState() == Stable) {
		setState(Waiting);
	}
	// if the boulder is Waiting && 30 ticks have elpased transition to Falling
	if (getState() == Waiting) {
		ticksElapsed++;
		if (ticksElapsed >= 30) {
			setState(Falling);
			ticksElapsed = 0;
		}
		//Play SOUND_FALLING_ROCK sound
	}
	if (getState() == Falling) {
		moveTo(getX(), getY() - 1);
		if (getWorld()->isThereIceBelow(getX(), getY())) {
			setInactive();
		}
	}
}

void WaterPuddle::doSomething()
{
	m_ticksAvailable++;
	int T = getWorld()->max(100, 300 - 10 * getWorld()->getLevel());
	
	if (m_ticksAvailable == T) {
		setInactive();
	}

}

void SonarKit::doSomething()
{
	m_ticksAvailable++;
	int T = getWorld()->max(100, 300 - 10 * getWorld()->getLevel());

	if (m_ticksAvailable == T) {
		setInactive();
	}
}


void Iceman::doSomething() {
	if (!getIsActive()) // if not alive, return immediately
		return;
	// need to remove ice when iceman overlaps
	if (getWorld()->icemanOverlaps(getX(), getY())) {
		getWorld()->playSound(SOUND_DIG);
	}
	//std::cerr << getWorld()->icemanOverlaps(getX(), getY()) << std::endl;
	int key = 0;
	if (getWorld()->getKey(key) == true) {
		switch (key) {
		case KEY_PRESS_LEFT:
			if (getX() == 0)
				break;
			setDirection(left);
			moveTo(getX() - 1, getY());
			break;
		case KEY_PRESS_RIGHT:
			if (getX() == VIEW_WIDTH - 4)
				break;
			setDirection(right);
			moveTo(getX() + 1, getY());
			break;
		case KEY_PRESS_DOWN:
			if (getY() == 0)
				break;
			setDirection(down);
			moveTo(getX(), getY() - 1);
			break;
		case KEY_PRESS_UP:
			if (getY() == VIEW_HEIGHT - 4)
				break;
			setDirection(up);
			moveTo(getX(), getY() + 1);
			break;
		}
	}
}