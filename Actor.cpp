#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            BARREL CLASS FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Goodie::updateFrackScore(int num)
{
    getWorld()->increaseScore(num);
}

void Barrel::doSomething()
{
    if (!isAlive())
        return;
    
    //Change back to !isVisible() and Barrel constructor to false after testing
    if (!isVisible() && getWorld()->touchedByFrack(getX(), getY(), 4))
    {
        setVisible(true);
        return;
    }
    
    if (getWorld()->touchedByFrack(getX(), getY(), 3))
    {
        setHitPts(0);
        getWorld()->playSound(SOUND_FOUND_OIL);
        updateFrackScore(1000); 
        getWorld()->setNumOil((getWorld()->getNumOil()) - 1);
    }
        
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            GOLD NUGGET CLASS FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GoldNugget::doSomething()
{
    if (!isAlive())
        return;
    
    if (!isVisible() && getWorld()->touchedByFrack(getX(), getY(), 4))
    {
        setVisible(true);
        return;
    }
    
    if (canBePickedUpByFrack() && getWorld()->touchedByFrack(getX(), getY(), 3))
    {
        setHitPts(0);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        updateFrackScore(10);
        getWorld()->returnFrack()->updateGold();
    }
    
    // TO DO:
    // Close to protesters **FIGURE THIS OUT**
     else if (!canBePickedUpByFrack())
    {
        setHitPts(0);
        getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
        
        // TO DO:
        //  The Gold Nugget must tell the Protester object that it just received a new Nugget so it can react appropriately (e.g., be bribed).
        
        updateFrackScore(25);
        
        // TO DO:
        //  Note: A Gold Nugget can only bribe a single Protester (either Regular or Hardcore) before disappearing from the game. If multiple Protesters are within the activating radius of the Nugget, then only one of the Protesters must be bribed.
    }
    
    if (!isPerm())
    {
        // TODO:
        // if (tick_lifetime_elapsed)
        setHitPts(0);
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            SONAR KIT IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SonarKit:: doSomething()
{
    if (!isAlive())
        return;
    
    // Otherwise, if the Sonar Kit is within a radius of 3.0 (<= 3.00 units away) from the FrackMan, then the Sonar Kit will activate, and:
    if (getWorld()->touchedByFrack(getX(), getY(), 3))
    {
        setHitPts(0);
        getWorld()->playSound(SOUND_SONAR);
        getWorld()->returnFrack()->addSonar();
        updateFrackScore(75);
    }
    
    // TO DO:
    //  Since the Sonar Kit is always in a temporary state, it will check to see if its tick lifetime has elapsed, and if so it must set its state to dead (so that it will be removed by your StudentWorld class from the game at the end of the current tick).
    
        
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            WATER POOL IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Water:: doSomething()
{
    if (!isAlive())
        return;
    
    // Otherwise, if the Water pool is within a radius of 3.0 (<= 3.00 units away) from the FrackMan, then the Water pool will activate, and:
    if (getWorld()->touchedByFrack(getX(), getY(), 3))
    {
        setHitPts(0);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->returnFrack()->addWater();
        updateFrackScore(100);
    }
    
    // 3. Since the Pool of Water is always in a temporary state, it will check to see if its tick lifetime has elapsed, and if so it must set its state to dead (so that it will be removed by your StudentWorld class from the game at the end of the current tick).
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            SQUIRT SQUIRT SQUIRT
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Squirt:: doSomething()
{
    // TO DO:
    //  If a Squirt is within a radius of 3.0 of one or more Protesters (up to and including a distance of 3.0 squares away), it will cause 2 points of annoyance to these Protester(s), and then immediately set its state to dead, so it can be removed from the oil field at the end of the tick.
    
    if (getWorld()->hitProtester(this))
    {
        setHitPts(0);
    }
    
    //  If a Squirt has traveled through its full travel distance, then it immediately sets its state to dead, so it can be removed from the oil field at the end of the current tick.
    if (m_distance == 0)
        setHitPts(0);
    else
    {
//        Otherwise, the Squirt must check to see if it can move one square in its currently-
//        facing direction. If this target location is occupied by either Dirt or a Boulder, then the Squirt immediately sets its state to dead so it can be removed from the oil field at the end of the current tick.
        
        Direction dir = getDirection();
        switch(dir)
        {
            case up:
                if (!getWorld()->canActorMoveTo(this, getX(), getY()+1))
                    setHitPts(0);
                else
                {
                    moveTo(getX(), getY()+1);
                    m_distance--;
                    return;
                }
                
                break;
                
            case down:
                if (!getWorld()->canActorMoveTo(this, getX(), getY()-1))
                    setHitPts(0);
                else
                {
                    moveTo(getX(), getY()-1);
                    m_distance--;
                    return;
                }
                
                break;
                
            case left:
                if (!getWorld()->canActorMoveTo(this, getX()-1, getY()))
                    setHitPts(0);
                else
                {
                    moveTo(getX()-1, getY());
                    m_distance--;
                    return;
                }
                
                break;
                
            case right:
                if (!getWorld()->canActorMoveTo(this, getX()+1, getY()))
                    setHitPts(0);
                else
                {
                    moveTo(getX()+1, getY());
                    m_distance--;
                    return;
                }
                
                break;
                
            default:
                break;
        }

    }
    
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            BOULDER CLASS FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Boulder::doSomething()
{
    if (!isAlive())                  // If it's dead...
        return;
    
    else
    {
        if (isStable == true)
            checkDirt4SquaresBelow();
        
        if (isWaiting == true && have30TicksElapsed())
        {
            // play the sound SOUND_FALLING_ROCK.
            getWorld()->playSound(SOUND_FALLING_ROCK);
        }
        if (isFalling == true)
            fallDown();
    }
    
}

void Boulder::checkDirt4SquaresBelow() 
{
        // check to see if there is any Dirt in the 4 squares immediately below it
        // For example, if the Boulder is at location x=20,y=30, it would check squares (20,29), (21,29), (22,29) and (23,29) for Dirt.
    
    bool temp = false;
    
    for (int x = getX(); x < getX() + 4; x++)
    {
        // If there is any Dirt below the Boulder, it does nothing.
        // However, if none of the 4 squares beneath the Boulder have any Dirt, then the Boulder must transition into a waiting state: for the next 30 ticks.
        
        if (getWorld()->isDirt(x, getY() - 1))
        {
            temp = true;
            break;
        }
    }
    
    if (temp != true)
    {
        std::cout<<"Transition into waiting state"<<std::endl;
        setState(false, true, false);
    }
    
}

bool Boulder:: have30TicksElapsed()                               // MAKE THIS FUNCTION PRIVATE
{
    
    m_tick_lifetime++;
    
    if (m_tick_lifetime == 30 && isValidToFall())
    {
        m_tick_lifetime = 0; 
        
        // Transition to falling
        std::cout<<"Transition into falling state"<<std::endl;
        setState(false, false, true);
        
        return true;
    }

    return false;
}

bool Boulder::isValidToFall() const
{
    bool tempIsDirt = true;
    bool tempIsBoulder = true;
    
    for (int x = 0; x < 4; x++)
    {
        if (!getWorld()->isDirt(getX() + x, getY() - 1))
        {
            tempIsDirt = false;
        }
        else
        {
            tempIsDirt = true;
            std::cout<<"There is dirt here!!"<<std::endl;
            break;
        }
        
        if (!getWorld()->isBoulder(getX() + x, getY() -1))
            tempIsBoulder = false;
        else
        {
            std::cout<<"There is a boulder here!!"<<std::endl;
            tempIsBoulder = true;
            break;
        }
    }
    
    if (getY() > -1 && tempIsBoulder == false && tempIsDirt == false)
        return true;
    else
        return false;
}

void Boulder::fallDown()
{
    /*
     A. It must continue to move downward one square during each tick until it either (a) hits the bottom of the oil field (i.e., it tries to move to y=-1), (b) runs into the top of another Boulder, or (c) it runs into Dirt (i.e., by moving down a square, the Boulder would overlap over one or more Dirt objects). When any of the above conditions are met the Boulder must set its state to dead so it can be removed from the game at the end of the current tick.
     */
    
    if (isValidToFall())
    {
        moveTo(getX(), getY()-1);
        std::cout<<"Moved already."<<std::endl;
    }
    else
    {
        std::cout<<"Setting Health to 0"<<std::endl;
        setHitPts(0);
    }
    
    /*
     B. If the Boulder comes within a radius of 3 (i.e. less than or equal to 3 squares, so 2.99 would count) of any Protester(s) or the FrackMan while falling, it must cause 100 points of annoyance to those actors (effectively completely annoying them in one fell swoop). The Boulder will continue falling down as described above, even if it annoys one or more Protesters on its way down (i.e., it won’t stop falling if it runs into a Protester). However, if the Boulder annoys the FrackMan, the player will instantly lose a life.
     */

    if (getWorld()->touchedByFrack(getX(), getY(), 3))
        getWorld()->returnFrack()->setHitPts(0);
        
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  AGENT CLASS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Annoy this actor (part of Actor base class)
bool Agent::annoy(unsigned int amount)
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  PROTESTER CLASS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Actor:: facingTowardFrackMan(Actor* a) const
{
    Direction facingDirection = lineOfSightToFrackMan(a);
    if (getDirection() == facingDirection)
        return true;
    else
        return false;
}

// If the Actor a has a clear line of sight to the FrackMan, return
// the direction to the FrackMan, otherwise GraphObject::none.
Actor::Direction Actor::lineOfSightToFrackMan(Actor* a) const
{

    int frackX = getWorld()->returnFrack()->getX();
    int frackY = getWorld()->returnFrack()->getY();
    
    // Same square
    if (a->getY() == frackY && a->getX() == frackX)
    {
        return getDirection();
    }
    
    // Vertical
    if (a->getX() == frackX)
    {
        if (frackY > a->getY())
            return GraphObject::up;
        else
            return GraphObject::down;
    }
    
    // Horizontal
    if (a->getY() == frackY)
    {
        if (frackX < a->getX())
            return GraphObject::left;
        else
            return GraphObject::right;
    }

    
    return GraphObject::none;
}

//      c. Could actually move the entire way to the FrackMan with no Dirt or
//        Boulders blocking its path (assuming it kept walking straight over the
//        next N turns),

bool Actor::validStraight2Frack() 
{
    
    int x = getWorld()->returnFrack()->getX();
    int y = getWorld()->returnFrack()->getY();
    
    bool valid = true;
    Direction dir = lineOfSightToFrackMan(this);
    
    // Horizontal
    switch(dir)
    {
        case up:
            for (int i = getY(); i < y; i++)
            {
                if (!getWorld()->canActorMoveTo(this, x, i))
                {
                    valid = false;
                    return valid;
                }
            
            }
            break;
            
        case down:
            for (int i = getY(); i > y; i--)
            {
                if (!getWorld()->canActorMoveTo(this, x, i))
                {
                    valid = false;
                    return valid;
                }

            }
            break;
            
        case left:
            for (int i = getX(); i > x; i--)
            {
                if (!getWorld()->canActorMoveTo(this, i, y))
                {
                    valid = false;
                    return valid;
                }
            }
            break;
            
        case right:
            for (int i = getX(); i < x; i++)
            {
                if (!getWorld()->canActorMoveTo(this, i, y))
                {
                    valid = false;
                    return valid;
                }
            }
            break;
            
        default:
            valid = false;
            break;
    }
    
    return valid;
}


void Protester:: doSomething()
{
    // # 1
    if (!isAlive())
        return;
    
    // #3 - FIGURE THIS OUT TOWARDS THE END
    if (isLeaving())
    {
        // If the Regular Protester is at its exit point (at location x=60, y=60) then it will immediately set its status to dead so that it will be removed from the game at the end of the current tick (by your StudentWorld class).
        
        if (getX() == 60 && getY() == 60)
            setHitPts(0);
//        else
//            figureOutExitRoute();
        
        // After moving one step toward their exit point, the Regular Protester will return immediately. It will not perform any of the following steps.
    }
    
    // #4
    //  Otherwise, the Regular Protester will check to see if they are within a distance of 4 units of the FrackMan, AND they are currently facing in the FrackMan’s direction (#5, is in direct line of sight). If BOTH are true and the Regular Protester hasn’t shouted within its last non-resting 15 ticks, then the Regular Protester will:
    if (isResting())
    {
        if (hasShout() == true)
        {
            incShoutTime();
            
            if (timeNoShout() >= 15*m_resting_ticks)
            {
                setShoutState(false);
                resetTimeNoShout();
            }
        }
        
        if (getWorld()->touchedByFrack(getX(), getY(), 4) && facingTowardFrackMan(this))
        {
            if (hasShout() == false)
            {
                getWorld()->playSound(SOUND_PROTESTER_YELL);
                getWorld()->returnFrack()->annoy(2);
                setShoutState(true);
            }
            
            // Have him pause for a little before moving again ("stunned")
            //            int current_level_number = getWorld()->getLevel();
            //            int N = fmax(50, 100 - current_level_number * 10);
            setActionCount(-20);
            
            return;
        }
        
        //#5 ** THIS PART IS DIFFERENT FOR HARDCORE **
        //    Otherwise, if the Protester
        //        a. Is in a straight horizontal or vertical line of sight to the FrackMan (even if                                                                      the Regular Protester isn’t currently facing the Frackman), and
        //
        //        b. Is more than 4 units away from the FrackMan – that is, the radius from the
        //        Regular Protester and the FrackMan is greater than 4.0 units away, and
        //
        //        c. Could actually move the entire way to the FrackMan with no Dirt or
        //        Boulders blocking its path (assuming it kept walking straight over the
        //                                     next N turns),
        else if (lineOfSightToFrackMan(this) != GraphObject::none && !getWorld()->touchedByFrack(getX(), getY(), 4) && validStraight2Frack())
        {
            // setDirection(lineOfSightToFrackMan(this));
            
            bool reset = false;
            
            // This means we've waited out resting ticks and can move / do other things :)
            if (actionCount() == m_resting_ticks)
            {
                reset = true;
                
                // Change its direction to face in the direction of the FrackMan, AND then take one step toward him.
                setDirection(lineOfSightToFrackMan(this));
                
                moveInChosenDir();
            }
            // # 2: In Rest State, do nothing besides increment action counter
            if (reset == true)
                resetActionCount();
            else
                m_actionCounter++;
            
            return;
        }
        
        //#6
        // Double check the move() function we have right now - because it's specifically tailored to #6s instructions. Might have to edit it or add conditionals for the other parts of the problem.
        
        // If can't directly see the FrackMan
        else if (lineOfSightToFrackMan(this) == GraphObject::none || !validStraight2Frack())
        {
            bool reset = false;
            
            // This means we've waited out resting ticks and can move / do other things :)
            if (actionCount() == m_resting_ticks)
            {
                reset = true;
                move();
            }
            // # 2: In Rest State, do nothing besides increment action counter
            if (reset == true)
                resetActionCount();
            else
                m_actionCounter++;
        }
        
        // #7
        
        if (didIMovePerp() == true)
        {
            incPerp();
            
            if (perpWait() >= 200*m_resting_ticks)
            {
                setPerp(false);
                resetPerp();
            }
        }
        
        // Is sitting at an intersection && hasn’t made a perpendicular turn in the last 200 non-resting ticks
        else if (checkPerp() && !didIMovePerp())
        {
            // Determine a viable perpendicular direction
            int upDown = 1;
            int leftRight = 2;
            Direction chosen = none;
            
            if (getDirection() == left || getDirection() == right)
                chosen = pickRandDir(upDown);
            
            if (getDirection() == up || getDirection() == down)
                chosen = pickRandDir(leftRight);
            
            // Set its direction to the selected perpendicular direction.
            setDirection(chosen);
            
            // Pick a new value for numSquaresToMoveInCurrentDirection
            decideNumMoves();
            
            // Remember that it made a perpendicular turn so that it doesn’t make another perpendicular turn for at least 200 more non-resting ticks.
            setPerp(true);
        }
    }
}

/*
 Otherwise, the Regular Protester must move one square closer to its exit point (at x=60, y=60). It must determine what direction to move based on a queue-based maze-searching algorithm like the one we learned in class. The Regular Protester may not walk through Dirt or Boulders to reach its exit point (meaning that none of the 16 squares that comprise the 4x4 Regular Protester icon may overlap with a Dirt object and that the Regular Protester must not come within (within means <=) a distance of 3.0 units of any Boulders). Hint: It’s inefficient for every single Regular Protester to try to figure out its own optimal path to the exit, so see if you can figure out a way to compute a single data structure that enables all Protesters to quickly determine their optimal path to their common exit point of 60,60, no matter where they are located in the oil field.
 */

void Protester:: figureOutExitRoute()
{
    
}

Actor::Direction convertNumToDir(int num)
{
    switch(num)
    {
        // Up
        case 0:
            return Actor::up;
            break;
        case 1:
            return Actor::down;
            break;
        case 2:
            return Actor::left;
            break;
        case 3:
            return Actor::right;
            break;
    }
    
    return Actor::up;
}

Actor::Direction Protester::pickRandDir(int num)
{
    int dir = 0;
    
    // If user passes in 1, will check UP/DOWN
    // If user passes in 2, will check % 4 + 2 which is LEFT/RIGHT
    // If user passes in 3, will check ALL directions
    
    if (num == 1)
        dir = rand() % 2;
    if (num == 2)
        dir = rand() % 4 + 2;
    if (num == 3)
        dir = rand() % 4;
    
    Actor::Direction temp = convertNumToDir(dir);
    
    while(!CanProtestMove(getX(), getY(), temp))
    {
        if (num == 1)
            dir = rand() % 2;
        if (num == 2)
            dir = rand() % 4 + 2;
        if (num == 3)
            dir = rand() % 4;
        
        temp = convertNumToDir(dir);
    }
    
    return temp;
}

bool Protester::checkPerp()
{
    Direction dir = getDirection();
    
    switch(dir)
    {
        case up:
        case down:
            if (getWorld()->canActorMoveTo(this, getX()-1, getY()) || getWorld()->canActorMoveTo(this, getX()+1, getY()))
                return true;
            break;
            
        case left:
        case right:
            if (getWorld()->canActorMoveTo(this, getX(), getY()+1) || getWorld()->canActorMoveTo(this, getX(), getY()-1))
                return true;
            break;
        default:
            return false;
    }
    return false;
}

bool Protester::CanProtestMove(int curx, int cury, Direction dir) 
{
    switch (dir)
    {
        case Actor::up:
            if (cury < 60 && getWorld()->canActorMoveTo(this, curx, cury+1))
                return true;
            
            break;
            
        case Actor::down:
            if (cury > 0 && getWorld()->canActorMoveTo(this, curx, cury-1))
                return true;
            
            break;
            
        case Actor::left:
            if ((curx > 0 && getWorld()->canActorMoveTo(this, curx-1, cury)))
                return true;
            
            break;
            
        case Actor::right:
            if ((curx < 60 && getWorld()->canActorMoveTo(this, curx+1, cury)))
                return true;
            
            break;
            
        default:
            break;
    }
    
    return false;
}

// This is only called when numOfSquaresToMove is not 0...so it's not ready to change directions yet
void Protester:: moveInChosenDir()
{
    Actor::Direction dir = getDirection();
    
    switch (dir)
    {
        case up:
            
            if (getY() < 60 && getWorld()->canActorMoveTo(this, getX(), getY() + 1))
                moveTo(getX(), getY()+1);

            else
                numSqsToMoveInCurrDir = 0;
            
            if (lineOfSightToFrackMan(this) != GraphObject::none)
                numSqsToMoveInCurrDir = 0;

            
            break;
            
        case down:
            
            if (getY() > 0 && getWorld()->canActorMoveTo(this, getX(), getY() - 1))
                moveTo(getX(), getY()-1);
            
            else
                numSqsToMoveInCurrDir = 0;
            
            if (lineOfSightToFrackMan(this) != GraphObject::none)
                numSqsToMoveInCurrDir = 0;

            break;
            
        case left:
            
            if (getX() > 0 && getWorld()->canActorMoveTo(this, getX() - 1, getY()))
                moveTo(getX()-1, getY());
        
            else
                numSqsToMoveInCurrDir = 0;
            
            if (lineOfSightToFrackMan(this) != GraphObject::none)
                numSqsToMoveInCurrDir = 0;

            break;
            
        case right:
            if (getX() < 60 && getWorld()->canActorMoveTo(this, getX() + 1, getY()))
                moveTo(getX()+1, getY());

            else
                numSqsToMoveInCurrDir = 0;
            
            if (lineOfSightToFrackMan(this) != GraphObject::none)
                numSqsToMoveInCurrDir = 0;

            break;
            
        default:
            moveTo(getX(), getY());
            break;
            
    }
}

void Protester:: move()
{
    // #6
    // Otherwise, the Regular Protester can’t directly see the FrackMan. As such, it will decrement its numSquaresToMoveInCurrentDirection variable by one. If the Regular Protester has finished walking numSquaresToMoveInCurrentDirection steps in its currently-selected direction (i.e., numSquaresToMoveInCurrentDirection <= 0), then:
    
    numSqsToMoveInCurrDir--;
    if (numSqsToMoveInCurrDir <= 0)
    {
        // The Regular Protester will pick a random new direction to move (up, down, left or right) & if it's a valid position, it'll set it's direction to the valid direction.
        
        Direction temp = pickRandDir(3);
        setDirection(temp);
        
        // Decide how many squares to move in newly chosen direction
        decideNumMoves();
    }
    else
    {
        moveInChosenDir();
    }
    
}

bool Protester:: annoy(unsigned int amount)
{
    setHitPts(hp() - amount);
    
    // Can't be further annoyed
    if (isLeaving())
        return false;
    
    //setHitPts(hp() - amount);
    
    //int current_level_number = getWorld()->getLevel();
    //int N = fmax(50, 100 - current_level_number * 10);
    
    
    // If not fully annoyed / not set to LeaveTheOilField
    if (hp() > 0)
    {
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        
        //m_resting_ticks = N;
    }
    else
    {
        isLeaveOilFieldState = true;
        isRestState = false;
        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
        m_resting_ticks = 0;
        
        if (getWorld()->isBoulder(getX(), getY()))
        {
            getWorld()->increaseScore(500);
        }
        else        // Annoyed by Squirt (only other way it can die)
        {
            getWorld()->increaseScore(100);
        }
            
    }
    
    return true;
}

void Protester:: addGold()
{
    
}

bool Protester:: huntsFrackMan() const
{
    return true;
}

void Protester:: setRestTicks()
{
    int current_level_number = getWorld()->getLevel();
    int ticksToWaitBetweenMoves = fmax(0, 3 - current_level_number/4);
    
    m_resting_ticks = ticksToWaitBetweenMoves;
}

/*  Each Regular Protester must decide how many squares,
 numSquaresToMoveInCurrentDirection, it will decide to move left before possibly switching its direction. This value must be: 8 <= numSquaresToMoveInCurrentDirection <= 60 */

void Protester:: decideNumMoves()
{
    int numMoves = rand() % 53 + 8; // 8 <= numMoves <= 60
        
    numSqsToMoveInCurrDir = numMoves;
    std::cerr << numMoves << std::endl;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          DERIVED PROTESTER CLASSES
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RegularProtester:: doSomething()
{
    // Right now, Protester's doSomething method was crafted solely for Regular Protesters. See what I can keep in the base class implementation as opposed to the others once you start thinking about Hardcore.
    // Can take chunks and put them into separate functions potentially then call it in our doSomething here.
    
    Protester::doSomething();
}

void RegularProtester::addGold()
{
    
}

void HardcoreProtester::doSomething()
{
    Protester::doSomething();
}

void HardcoreProtester:: addGold()
{
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            FRACKMAN IMPLEMENTATIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool FrackMan:: annoy(unsigned int amount)
{
    setHitPts(hp()-2);
    
    if (!isAlive())
        return true;
    
    return false;
}

void FrackMan:: addGold()
{
    
}


void FrackMan::doSomething()
{
    if (!isAlive())
        return;
    
    int value;
    if (getWorld()->getKey(value) == true)
    {
        switch(value)
        {
            case KEY_PRESS_LEFT:
                if (getX() > 0 && getDirection() == left && !getWorld()->willMoveIntoBoulder(getX() - 1, getY()))
                    moveTo(getX()-1, getY());
                
                else if (getX() == 0 && getDirection() != left)
                {
                    setDirection(left);
                    moveTo(getX(), getY());
                }
                
                else
                {
                    setDirection(left);
                    moveTo(getX(), getY());
                }
                
                // remove dirt
                getWorld()->cleanDirtInTheWay(getX(), getY());

                break;
                
            case KEY_PRESS_RIGHT:
                
                if (getX() < 60 && getDirection() == right && !getWorld()->willMoveIntoBoulder(getX() + 1, getY()))
                    moveTo(getX()+1, getY());
                
                else if (getX() == 60 && getDirection() != right)
                {
                    setDirection(right);
                    moveTo(getX(), getY());
                }
                
                else
                {
                    setDirection(right);
                    moveTo(getX(), getY());
                }
                
                // remove dirt
                getWorld()->cleanDirtInTheWay(getX(), getY());
 
                break;
                
            case KEY_PRESS_DOWN:
                if (getY() > 0 && getDirection() == down && !getWorld()->willMoveIntoBoulder(getX(), getY() - 1))
                    moveTo(getX(), getY() - 1);
                
                else if (getY() == 0 && getDirection() != down)
                {
                    setDirection(down);
                    moveTo(getX(), getY());
                }
                
                else
                {
                    setDirection(down);
                    moveTo(getX(), getY());
                }
                
                // remove dirt
                getWorld()->cleanDirtInTheWay(getX(), getY());
               
                break;
                
            case KEY_PRESS_UP:
                if (getY() < 60 && getDirection() == up && !getWorld()->willMoveIntoBoulder(getX(), getY() + 1))
                    moveTo(getX(), getY() + 1);
                
                else if (getY() == 60 && getDirection() != up)
                {
                    setDirection(up);
                    moveTo(getX(), getY());
                }
                
                else
                {
                    setDirection(up);
                    moveTo(getX(), getY());
                }
                
                // remove dirt
                getWorld()->cleanDirtInTheWay(getX(), getY());
                
                break;
                
            case KEY_PRESS_ESCAPE:
                //  The code in your StudentWorld class should detect that the FrackMan has died and address this appropriately (e.g., replay the level from scratch, or end the game if the player has run out of lives).
                setHitPts(0);
                break;
                
            case KEY_PRESS_SPACE:
                
                Direction dir = getDirection();
                if(numSquirts() >= 1)
                {
                    switch(dir)
                    {
                        case up:
                            getWorld()->addActor(getX(), getY()+4, true, 's', dir);
                            getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                            useSquirt();
                            break;
                            
                        case down:
                            getWorld()->addActor(getX(), getY()-4, true, 's', dir);
                            getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                            useSquirt();
                            break;
                            
                        case left:
                            getWorld()->addActor(getX()-4, getY(), true, 's', dir);
                            getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                            useSquirt();
                            break;
                            
                        case right:
                            getWorld()->addActor(getX()+4, getY(), true, 's', dir);
                            getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                            useSquirt();
                            break;
                            
                        default:
                            break;
                    }
                }
                else
                    break;
                
                break;
            
        }
        
    }
    
    
}

