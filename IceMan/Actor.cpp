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
		std::cerr << "????" << std::endl;
		getWorld()->playSound(SOUND_DIG);
	}
	//std::cerr << getWorld()->icemanOverlaps(getX(), getY()) << std::endl;
	int key = 0;
	if (getWorld()->getKey(key) == true) {
		switch (key) {
		case KEY_PRESS_LEFT:
			if (getX() == 0)
				break;
			else if (getDirection() == left) {
				moveTo(getX() - 1, getY());
				break;
			}
			else {
				setDirection(left);
				break;
			}
		case KEY_PRESS_RIGHT:
			if (getX() == VIEW_WIDTH - 4)
				break;
			else if (getDirection() == right) {
				moveTo(getX() + 1, getY());
				break;
			}
			else {
				setDirection(right);
				break;
			}
		case KEY_PRESS_DOWN:
			if (getY() == 0)
				break;
			else if (getDirection() == down) {
				moveTo(getX(), getY() - 1);
				break;
			}
			else {
				setDirection(down);
				break;
			}
		case KEY_PRESS_UP:
			if (getY() == VIEW_HEIGHT - 4)
				break;
			else if (getDirection() == up) {
				moveTo(getX(), getY() + 1);
				break;
			}
			else {
				setDirection(up);
				break;
			}
		}
	}

	// TODO: 
	// don't allow iceman to move through boulders
	// if esc key is pressed abort current level (set iceman to inactive and dec lives or end game if out of lives
	// if spacebar is pressed fire squirt and reduce water count by one
	// if z is pressed use sonar charge (illuminate acquirables within radius of 12) if available and dec numSonar
	// if tab is pressed and iceman has gold, drop a gold nugget with a temporary and visible state
	// if iceman not currently facing direction of key stroke, just turn in that direction, but don't move
}
bool Iceman::invalidIcemanCoordinate(const int& x, const int& y) {

	return false;
}