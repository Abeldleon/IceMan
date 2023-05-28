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
	ticksElapsed++;
	ticksAvailable = getWorld()->max(100, 300 - 10 * getWorld()->getLevel()); // num ticks available
	
	if (ticksElapsed == ticksAvailable) {
		setInactive();
	}

	if (getWorld()->isOverlappingIceman(getX(), getY())) {
		getWorld()->updateWaterSquirts();
		getWorld()->increaseScore(100);
		setInactive();
	}

}

void SonarKit::doSomething()
{
	//m_ticksAvailable++;
	ticksElapsed++;
	ticksAvailable = getWorld()->max(100, 300 - 10 * getWorld()->getLevel()); // numticks avialable

	if (ticksElapsed == ticksAvailable) {
		setInactive();
	}

	if (getWorld()->isOverlappingIceman(getX(), getY())) {
		getWorld()->updateSonarCharge();
		getWorld()->increaseScore(75);
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
			else if (getDirection() == left) {

				if (getWorld()->isBlocked(getX()-2, getY())) {
					//moveTo(getX(), getY());
					break;
				}
				else {

					moveTo(getX() - 1, getY());
					break;

				}

			}
			else {
				setDirection(left);
				break;
			}
		case KEY_PRESS_RIGHT:
			if (getX() == VIEW_WIDTH - 4)
				break;
			else if (getDirection() == right) {

				if (getWorld()->isBlocked(getX() + 1, getY())) {
					//moveTo(getX(), getY());
					break;
				}
				else {

					moveTo(getX() + 1, getY());
					break;
				
				}
			}
			else {
				setDirection(right);
				break;
			}
		case KEY_PRESS_DOWN:
			if (getY() == 0)
				break;
			else if (getDirection() == down) {
				if (getWorld()->isBlocked(getX(), getY() - 1)) {
					//moveTo(getX(), getY());
					break;
				}
				else {
					moveTo(getX(), getY() - 1);
					break;
				}
			}
			else {
				setDirection(down);
				break;
			}
		case KEY_PRESS_UP:
			if (getY() == VIEW_HEIGHT - 4)
				break;
			else if (getDirection() == up) {

				if (getWorld()->isBlocked(getX(), getY() + 2)) {
					//moveTo(getX(), getY());
					break;
				}
				else {
					moveTo(getX(), getY() + 1);
					break;
				}
			}
			else {
				setDirection(up);
				break;
			}
		case KEY_PRESS_ESCAPE: // calls cleanup() in studentworld. doesn't currently delete actorPtrs
			setInactive();
		
		case KEY_PRESS_SPACE:
			getWorld()->populateWaterSquirt();
			getWorld()->playSound(SOUND_PLAYER_SQUIRT);
			userSquirt();
			break;
		case 'z':
			useSonar();
			break;
		case KEY_PRESS_TAB:
			if (goldNuggets > 0) {
				dropGold(); // decrements goldNuggets
				getWorld()->populateGold(GoldNugget::protestorCan, GoldNugget::temporary); // populates gold at iceman's location.
			}

		}
	}

	// TODO: 
	// don't allow iceman to move through boulders
	// if esc key is pressed abort current level (set iceman to inactive and dec lives or end game if out of lives
	// if spacebar is pressed fire squirt and reduce water count by one
	// if z is pressed use sonar charge (illuminate acquirables within radius of 12) if available and dec numSonar
	// if tab is pressed and iceman has gold, drop a gold nugget with a temporary and visible state
}
bool Iceman::invalidIcemanCoordinate(const int& x, const int& y) {

	return false;
}

void GoldNugget::doSomething()
{
	if (getWhoCanPickUp() == icemanCan && getWorld()->isOverlappingIceman(getX(), getY())) {
		getWorld()->updateGoldNuggets();
		getWorld()->increaseScore(10);
		setInactive();
	}
	else if (getWhoCanPickUp() == protestorCan) {
		ticksElapsed++;
		// TODO: if overlaps protestor, setInactive and make regular protestor leave the oil field, hardcore protestor pause
		if (ticksElapsed == 100) {
			setInactive();
		}
	}
}

void WaterSquirt::doSomething() {
	// if within radius of 3 to one or more protestor cause two points of annoyance to protestor and set state to dead

	if (getDistanceTravelled() == 4) {
		setInactive();
		return;
	}
	//if (getWorld()->overlapsProtestor()) { // make these functions in studentworld when create proetestors
	//	getWorld()->damageProtestor();
	//	setInactive();
	//	return;
	//}

	// checking to see if ice or boulder infront of squirt done by studentworld (seems like sample is implemented this way also)

	// move squirt one in direction that it's facing
	if (getDirection() == right) {
		moveTo(getX() + 1, getY());
		incrementDistanceTravelled();
	}
	else if (getDirection() == left) {
		moveTo(getX() - 1, getY());
		incrementDistanceTravelled();
	}
	else if (getDirection() == down) {
		moveTo(getX(), getY() - 1);
		incrementDistanceTravelled();
	}
	else if (getDirection() == up) {
		moveTo(getX(), getY() + 1);
		incrementDistanceTravelled();
	}
}

void OilBarrel::doSomething() {
	if (getWorld()->isOverlappingIceman(getX(), getY())) {
		getWorld()->updateOilBarrel(); // increments num oil barrels iceman has
		getWorld()->increaseScore(1000);
		getWorld()->playSound(SOUND_FOUND_OIL);
		setInactive();
	}
}

