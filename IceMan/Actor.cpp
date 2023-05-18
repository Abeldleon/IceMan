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
