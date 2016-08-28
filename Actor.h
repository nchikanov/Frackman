#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <list>
#include <queue>
//#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;


class Actor : public GraphObject
{
public:
    // Double check this to make sure...
    Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* sW, int hp, char name)
    :GraphObject(imageID, startX, startY, dir, size, depth)
    {
        setVisible(true);
        sWorld = sW;
        m_hitPts = hp;
        m_name = name;
    }
 
    virtual ~Actor() { setVisible(false); }       // Reevaluate this later, do we need something here?
    
    virtual void doSomething() = 0;
    char getName() {return m_name;}
    
    bool validStraight2Frack();
    
    bool isAlive() const
    {
        if (hp() <= 0)
            return false;
        else
            return true;
    }
    
    // Adjust number of hit points of an actor.
    void setHitPts(int hp) { m_hitPts = hp; }
    
    // Return number of hit pts an actor has. If hp = 0, actor is dead.
    int hp() const { return m_hitPts; }
    
    // Annoy this actor (used by "agents")
    virtual bool annoy(unsigned int amount) {return false;}
    
    // Can this actor pick items up? (used by "agents")
    virtual bool canPickThingsUp() const {return false;}
    
    // Can this actor dig through dirt?
    virtual bool canDigThroughDirt() const {return false;}

   // Return a pointer to student world
    StudentWorld* getWorld() const {return sWorld;}
    
    virtual bool canActorPassThroughMe() const {return true;}
    
    // Is the Actor a facing toward the FrackMan?
    bool facingTowardFrackMan(Actor* a) const;
    
    // If the Actor a has a clear line of sight to the FrackMan, return
    // the direction to the FrackMan, otherwise GraphObject::none.
    GraphObject::Direction lineOfSightToFrackMan(Actor* a) const;
    
    
private:
    int m_hitPts;                   // If hit pts reach 0, not alive
    char m_name;
    
    StudentWorld* sWorld;

};

class Dirt : public Actor
{
public:
    Dirt(int x, int y, StudentWorld* sW) : Actor(IID_DIRT, x, y, right, .25, 3, sW, 10, 'd') {}
    virtual ~Dirt() {};
    virtual void doSomething() {};

};

class Squirt : public Actor
{
public:
    Squirt(int x, int y, Direction dir, StudentWorld* sW) : Actor(IID_WATER_SPURT, x, y, dir, 1.0, 1, sW, 10, 's')
    {
        m_distance = 4;
    }
    
    virtual void doSomething();
    
private:
    int m_distance;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   MOVING ACTORS (FRACK/PROTEST)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Agent : public Actor
{
public:
    Agent(StudentWorld* sW, int startX, int startY, Direction startDir,
          int imageID, unsigned int hitPoints, char name)
    : Actor (imageID, startX, startY, startDir, 1.0, 0, sW, hitPoints, name)
    {
        setHitPts(hitPoints);
    }
    
    // Pick up a gold nugget.
    virtual void addGold() = 0; 
    
    // Annoy this actor (part of Actor base class)
    virtual bool annoy(unsigned int amount);
    
    // This is part of Actor base class
    virtual bool canPickThingsUp() const {return true;}
};

class FrackMan : public Agent
{
public:
    FrackMan(StudentWorld* sW) : Agent(sW, 30, 60, right, IID_PLAYER, 10, 'f')
    {
        m_numSquirts = 5;
        m_numSonar = 1;
        m_numGoldNugs = 0;
    }
    virtual ~FrackMan(){};
    
    virtual void doSomething();
    
    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool canDigThroughDirt() const {return true;}
    
    // Pick up a sonar kit.
    void addSonar() {m_numSonar = m_numSonar + 1;}
    
    // Pick up water.
    void addWater() {m_numSquirts = m_numSquirts + 1;}
    
    // Use squirt
    void useSquirt() { m_numSquirts = m_numSquirts - 1; }
    
    // Pick up gold.
    void updateGold() {m_numGoldNugs = m_numGoldNugs + 1;}
    
    // Return num of Squirts
    unsigned int numSquirts() const {return m_numSquirts;}
    
    // Return num of Sonars
    unsigned int numSonar() const {return m_numSonar;}
    
    // Return num of Gold Nugs collected
    unsigned int numGold() const {return m_numGoldNugs;}

    
private:
    // Health: Loses 20% each time he's yelled at / Health (loses 2 HP) 
    int m_numSquirts;
    int m_numSonar;
    int m_numGoldNugs;
    
};

class Protester : public Agent
{
public:
    Protester(StudentWorld* sW, int startX, int startY, int ID, unsigned int hitPts, char name) : Agent(sW, startX, startY, left, ID, hitPts, name)
    {
        isLeaveOilFieldState = false;
        isRestState = true;
        
        m_actionCounter = 0;
        m_time_no_shout = 0;
        m_perp_move = 0; 
        hasShouted = false;
        madePerpMove = false;
        
        // Initialize numSqsToMoveInCurrDir
        decideNumMoves();
        setRestTicks();
    }
    
    virtual ~Protester() {};
    
    // Used mostly to determine the next random direction it can move in 
    bool CanProtestMove(int curx, int cury, Direction dir);
    
    // Check if can move perpendicularly
    bool checkPerp();
    
    // Decide the number of squares to move before changing direction
    void decideNumMoves();
    
    void move();
    
    // When in LeaveOilField State - find a way out of the game efficiently
    void figureOutExitRoute();
    
    virtual void doSomething();
    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool huntsFrackMan() const;
    
    // Set/return number of ticks until next move
    void setRestTicks();
    
    // Random direction generator
    Direction pickRandDir(int num);
    void moveInChosenDir();
    
    // Deals with Action Counter (records every tick)
    int actionCount() const { return m_actionCounter; }
    void resetActionCount() { m_actionCounter = 0;}
    void setActionCount(int num) { m_actionCounter = num; }
    
    // Deals with Time between shouts
    void incShoutTime() {m_time_no_shout++;}
    int timeNoShout() const {return m_time_no_shout;}
    void resetTimeNoShout() { m_time_no_shout = 0;}
    
    void setShoutState(bool state) { hasShouted = state;}
    bool hasShout() const { return hasShouted; }

    // State getters
    bool isResting() const { return isRestState; }
    bool isLeaving() const { return isLeaveOilFieldState; }
    
    // Perpendicular Movement
    void resetPerp() { m_perp_move = 0; }
    void incPerp() { m_perp_move++; }
    int perpWait() const { return m_perp_move; }
    
    void setPerp(bool state) { madePerpMove = state; }
    bool didIMovePerp() const { return madePerpMove; }
    
    // Set state to having given up protest
    void setMustLeaveOilField()
    {
        if (!isAlive())
            isLeaveOilFieldState = true;
    }
    
    
private:
    bool isLeaveOilFieldState;
    bool isRestState;               // In "rest state" when action counter != numRestTicks
    
    // int m_waiting_ticks;
    int m_resting_ticks;
    
    // Keeps track of how long it hasn't shouted for
    int m_time_no_shout;
    bool hasShouted;
    
    // Perpendicular movement
    int m_perp_move;
    bool madePerpMove;
    
    // How many squares to move to before switiching direction under simple conditions
    int numSqsToMoveInCurrDir;
    
    // Keeps track of number of ticks up until it reaches m_resting_ticks
    int m_actionCounter;
};

class RegularProtester : public Protester
{
public:
    RegularProtester(StudentWorld* sW) : Protester(sW, 60, 60, IID_PROTESTER, 5, 'r')
    {
    }
    virtual ~RegularProtester() {};
    
    virtual void doSomething();
    virtual void addGold();
    
};

class HardcoreProtester : public Protester
{
public:
    HardcoreProtester(StudentWorld* sW) : Protester(sW, 60, 60, IID_HARD_CORE_PROTESTER, 20, 'h')
    {
    }
    virtual ~HardcoreProtester() {};
    
    virtual void doSomething();
    virtual void addGold();
    
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                   BOULDER
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Boulder : public Actor
{
public:
    Boulder(int x, int y, StudentWorld* sW) : Actor(IID_BOULDER, x, y, down, 1.0, 1, sW, 10, 'b')
    {
        isStable = true;
        isWaiting = false;
        isFalling = false;
        m_tick_lifetime = 0;
    }
    virtual ~Boulder(){};
    
    void setState(bool stable, bool waiting, bool falling)
    {
        isStable = stable;
        isWaiting = waiting;
        isFalling = falling;
    }
    
    virtual bool canActorPassThroughMe() {return false;}
    virtual void doSomething();
    
private:
    bool isStable;
    bool isWaiting;
    bool isFalling;
    
    bool have30TicksElapsed();
    bool isValidToFall() const;
    void fallDown();
    void checkDirt4SquaresBelow();
    
    int m_tick_lifetime;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                GOODIES
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Goodie : public Actor
{
public:
    Goodie(int goodieID, int x, int y, Direction dir, bool visible, StudentWorld* sW, char name) : Actor(goodieID, x, y, dir, 1.0, 2, sW, 10, name)
    {
        setVisible(visible);
        isPermanent = true;
    }

    virtual ~Goodie() {};
    
    virtual void updateFrackScore(int num);    // Used by gold, water pool, sonar, oil
    bool canBePickedUpByFrack() {return true;}               // Used by all
    
    void setPermanent(bool permanent) {isPermanent = permanent;}
    bool isPerm() {return isPermanent;}
    
private:
    bool isPermanent;
    
};

class Water : public Goodie
{
public:
    Water(int x, int y, StudentWorld* sW) : Goodie(IID_WATER_POOL, x, y, right, true, sW, 'w') {}
    virtual ~Water() {};
    
    virtual void doSomething();
};

class SonarKit : public Goodie
{
public:
    SonarKit(int x, int y, StudentWorld* sW) : Goodie(IID_SONAR, x, y, right, true, sW, 'k') {}
    virtual ~SonarKit() {}
    
    virtual void doSomething();
    
};

class GoldNugget : public Goodie
{
public:
    GoldNugget(int x, int y, StudentWorld* sW, bool visible, bool permanent) : Goodie(IID_GOLD, x, y, right, visible, sW, 'g')
    {
        setPermanent(permanent);
    }
    
    virtual ~GoldNugget() {};
    
    virtual void doSomething(); 
    
private:
    
};


class Barrel: public Goodie
{
public:
    Barrel(int x, int y, StudentWorld *sW) : Goodie(IID_BARREL, x, y, right, false, sW, 'o') {};
    virtual ~Barrel() {};
                                     
    virtual void doSomething();
    
};




#endif // ACTOR_H_

