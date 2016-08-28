#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <vector>
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    // Constructor
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
	}
    
    // Create Game Stat Text
    void setDisplayText();

    // Required to Initialize
    virtual int init();
    virtual void cleanUp();
    virtual int move();
    
    // Helper Functions for Dirt
    bool isDirt(int x, int y) const;
    void cleanDirtInTheWay(int x, int y);
    
    // Checks if any object has been touched by FrackMan
    bool touchedByFrack(int x, int y, int rad);  // Uses Rad function below
    
    // Boulder Helper Functions //
    // Used in FrackMan's doSomething() to check if he's about to move into a Boulder
    bool willMoveIntoBoulder(int x, int y) const;
    // Is a boulder in that (x,y)?
    bool isBoulder(int x, int y) const; 
    
    // Returns pointer to FrackMan
    FrackMan* returnFrack() {return m_frackman;}
    
    // Keeps track of Number of Oil Barrels active
    void setNumOil(int num) {numOil = num;}
    int getNumOil() {return numOil;}
  
    // Can an actor move here?
    bool canActorMoveTo(Actor* a, int x, int y) const;
    
    // Destructor
    ~StudentWorld();
    
    void addActor(int x, int y, bool isVisible, char imgName, GraphObject::Direction dir);
    void annoyActor(Actor*, int hp);
    
    // Checks the Radius between FrackMan (moving object) and Object
    bool isRadLessThanNum(int movingCol, int movingRow, int curCol, int curRow, int num) const;
    
    bool hitProtester(Squirt* s);

private:
    Dirt* m_dirt[64][64];           // Array of pointer to Dirt
    FrackMan* m_frackman;
    
    Boulder* m_boulder; 
    Barrel* m_barrel;
    Protester* m_protest;
    GoldNugget* m_gold;
    Squirt* m_squirt;
    SonarKit* m_sonar;
    Water* m_water; 
    
    void generateDirt();
    void generateBoulders();
    void generateOilandGold();
    void generateProtesters();
    void generateSonarAndWater();
    
    // Makes sure all objects generate a radius of 6 away from each other
    bool objectTryingToBeClose(int x, int y);
    
    int numOil;
    
    int curNumTicks;
    int curNumProtests;

    std::vector<Actor*> m_actor; 
    
};

#endif // STUDENTWORLD_H_



