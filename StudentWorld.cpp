#include "StudentWorld.h"
#include <string>
#include <algorithm>
#include <cmath>
using namespace std; 

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            HEADER TEXT FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

string format(int score, int level, int livesLeft, int healthLeft, int squirts, int gold, int sonar, int oil)
{
    string text = "Scr: ";
    text += (to_string(score) + " Lvl: " + to_string(level) +  " Lives: " + to_string(livesLeft) + " Hlth: " + to_string(healthLeft) + "% Wtr: " + to_string(squirts) + " Gld: " + to_string(gold) + " Sonar: " + to_string(sonar) + " Oil Left: " + to_string(oil));
    
    return text;
}

void StudentWorld::setDisplayText()
{
    int score = getScore();
    int level = getLevel();
    int livesLeft = getLives();
    int healthLeft = (m_frackman->hp() * 10);
    int squirts = m_frackman->numSquirts();
    int gold = m_frackman->numGold();
    int sonar = m_frackman->numSonar();
    int oil = getNumOil();
    
    string s = format(score, level, livesLeft, healthLeft, squirts, gold, sonar, oil);
    
    setGameStatText(s);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            DIRT HELPER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool StudentWorld::isDirt(int x, int y) const
{
    if (m_dirt[x][y] != nullptr)
        return true;
    else
        return false;
}

void StudentWorld::cleanDirtInTheWay(int x, int y)
{
    
    for (int i = x; i < x + 4; i++)
    {
        for (int j = y; j < y + 4; j++)
        {
            if (i >= 0 && i < 64 && j < 60 && j >=0)
            {
                if (isDirt(i, j))
                {
                    // These conditions always true at beginning of level.
                    // This function is called to get rid of existing dirt when boulders / gold / others are added
                    // So it would play the sound extraneously at the beginning
                    // Checks so the sound only plays during movement :)
                    if (m_frackman->getX() != 30 && m_frackman->getY() != 60)
                        playSound(SOUND_DIG);
                    delete m_dirt[i][j];
                    m_dirt[i][j] = nullptr;
                }
            }
        }
    }
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            PARTS OF INIT() IN S-WORLD
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Used in random generation of items so as to avoid polluting the mine shaft
int randX()
{
    int xRand = rand() % 60;
    
    if(xRand > 26 && xRand < 34)
        xRand = randX();
    
    return xRand;
}

bool StudentWorld::objectTryingToBeClose(int x, int y)
{
    if (m_actor.empty())
        return false;
    
    int newX = x;
    int newY = y;
    
    for (int i = 0; i < m_actor.size(); i++)
    {
        int oldX = m_actor[i]->getX();
        int oldY = m_actor[i]->getY();
        
        int totalX = abs(newX - oldX);
        int totalY = abs(newY - oldY);
        
        int dist = sqrt(totalX*totalX + totalY*totalY);
        
        if (dist < 6)
            return true;
    }
    
    return false;
}

bool StudentWorld:: willMoveIntoBoulder(int x, int y) const
{
    for (int i = 0; i < m_actor.size(); i++)
    {
        if (m_actor[i]->getName() == 'b')
        {
            if (isRadLessThanNum(x, y, m_actor[i]->getX(), m_actor[i]->getY(), 3))
                return true;
        }
    }
    
    return false;
}

void StudentWorld::generateBoulders()
{
    int current_level_number = getLevel();
    int B = min(current_level_number / 2 + 2, 6);
    
    for (int i = 0; i < B; i++)
    {
        int col = randX();
        int row = rand() % 36 + 20;
        
        while (objectTryingToBeClose(col, row))            // Regenerate col/row values if rad not > 6
        {
            col = randX();
            row = rand() % 36 + 20;
        }
        
        cleanDirtInTheWay(col, row);
        m_boulder = new Boulder(col, row, this);
        m_actor.push_back(m_boulder);
        
    }
    
//    m_boulder = new Boulder(40, 60, this);
//    m_actor.push_back(m_boulder);
}

void StudentWorld::generateDirt()
{
    // Draw left half of dirt
    for (int col = 0; col < 30; col++)
    {
        for (int row = 0; row < 60; row++)
        {
            m_dirt[col][row] = new Dirt(col, row, this);
        }
    }
    
    // Draw right half of dirt
    for (int col = 34; col < 64; col++)
    {
        for (int row = 0; row < 60; row++)
        {
            m_dirt[col][row] = new Dirt(col, row, this);
        }
    }
    
    // Bottom of mineshaft
    for (int col = 30; col < 34; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            m_dirt[col][row] = new Dirt(col, row, this);
        }
    }
    
}

void StudentWorld:: generateOilandGold()
{
    int current_level_number = getLevel();
    int L = min(2 + current_level_number, 20);
    
    int num = 0;
    for (int i = 0; i < L; i++)
    {
        num = num + 1;
        setNumOil(num);
        
        int col = randX();
        int row = rand() % 56;
        
        while (objectTryingToBeClose(col, row))            // Regenerate col/row values if rad not > 6
        {
            col = randX();
            row = rand() % 56;
        }
        
        m_barrel = new Barrel(col, row, this);
        m_actor.push_back(m_barrel);
    }
    
    // G Gold Nuggets in each level, where:
    int G = max(5-current_level_number / 2, 2);
    for (int i = 0; i < G; i++)
    {
        int col = randX();
        int row = rand() % 56;
        
        while (objectTryingToBeClose(col, row))            // Regenerate col/row values if rad not > 6
        {
            col = randX();
            row = rand() % 56;
        }
        
        m_gold = new GoldNugget(col, row, this, false, true);
        m_actor.push_back(m_gold); 
    }

}

void StudentWorld::generateProtesters()
{
    int current_level_number = getLevel();
    int T = max(25, 200 - current_level_number);
    int P = fmin(15, 2 + current_level_number * 1.5);
    int probabilityOfHardcore = min(90, current_level_number * 10 + 30);
    
    if (curNumTicks > T && curNumProtests < P)
    {
        // If Probability = 20/100, generate number between 1 and 10. If # < 2 then create new HardCore. Else RegProtest
        int randNum = rand() % 10 + 1;
        if (randNum < (probabilityOfHardcore / 10))
            m_protest = new HardcoreProtester(this);
        else
            m_protest = new RegularProtester(this);
        
        m_actor.push_back(m_protest);
        curNumTicks = 0;
        curNumProtests++;
    }
}

void StudentWorld::generateSonarAndWater()
{
    int current_level_number = getLevel();
    int G = current_level_number * 25 + 300;
    
    int randNum = rand() % 10 + 1;

    int col = randX();
    int row = rand() % 56;
    
    if (randNum < G / 10)
    {
        m_sonar = new SonarKit(0, 60, this);
        m_actor.push_back(m_sonar);
    }
    else if (randNum > G/10 && !isDirt(col, row))
    {
        m_water = new Water(col, row, this);
        m_actor.push_back(m_water);
    }

}

int StudentWorld::init()
{
    m_frackman = new FrackMan(this);        // create FrackMan object and insert at right location
    
    curNumTicks = 0;
    
    m_protest = new RegularProtester(this);
    m_actor.push_back(m_protest);
    
    curNumProtests = 1;
    
    generateDirt();
    generateBoulders();
    generateOilandGold();
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // Update game status line
    setDisplayText();
    
    curNumTicks++;
    m_frackman->doSomething();
    
    Actor* a = m_actor[0];
    
    generateProtesters();
    //generateSonarAndWater();
    
    for (int i = 0; i < m_actor.size(); i++)
    {
        if (!a->isAlive())
        {
            delete a;
            m_actor.erase(m_actor.begin() + i);
            i--;
            
        }
        else
        {
            a->doSomething();
        }
        
        a = m_actor[i+1];
    }
    
    if (!m_frackman->isAlive())
    {
        decLives();
        playSound(SOUND_PLAYER_GIVE_UP);
        return GWSTATUS_PLAYER_DIED;
        
    }

    if (numOil == 0)
    {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    return GWSTATUS_CONTINUE_GAME;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            IS __________ ? FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Used to check if objects have # or more pixels between them
// If they have less, they can't be placed in that row/col so we would regenerate random values
bool StudentWorld::isRadLessThanNum(int movingCol, int movingRow, int curCol, int curRow, int num) const
{
    if (m_actor.size() == 0)
        return false;
    else
    {
        int x = abs(movingCol - (curCol));         // Moving/New Object - Center of Actor
        int y = abs(movingRow - (curRow));         // Moving/New Object - Center of Actor
        int calc = sqrt(x*x + y*y);
        
        if (calc <= num)
            return true;
    }
    
    return false;
}

bool StudentWorld::isBoulder(int x, int y) const
{
    
    for (int i = 0; i < m_actor.size(); i++)
    {
        if (m_actor[i]->getID() == IID_BOULDER)
        {
            if (m_actor[i]->getX() == x && m_actor[i]->getY() == y)
                return true;
        }
    }

    return false; 
}

bool StudentWorld::touchedByFrack(int objX, int objY, int rad)
{
    int frackX = m_frackman->getX();
    int frackY = m_frackman->getY();
    
    if (isRadLessThanNum(frackX, frackY, objX, objY, rad))
        return true;
    else
        return false;
    
}

/////////////////////////////////////////// CAN ACTOR MOVE //////////////////////////////

bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const
{
    bool canMove = true;

    for (int i = x; i < x + 4; i++)
    {
        for (int j = y; j < y + 4; j++)
        {
            if (i >= 0 && i < 64 && j < 64 && j >=0)
            {
                if (isDirt(i, j))
                {
                    if (!a->canDigThroughDirt())
                        canMove = false;
                }
            }
            else
                canMove = false;
        }
    }

    // Check for objects in the way
    if (willMoveIntoBoulder(x, y))
        canMove = false;
    
    return canMove;
}


void StudentWorld::addActor(int x, int y, bool isVisible, char imgName, GraphObject::Direction dir)
{
    
    bool permanent;
    if (isVisible == true)
        permanent = false;
    else
        permanent = true;
    
    switch (imgName)
    {
        case 'g':
            m_gold = new GoldNugget(x, y, this, isVisible, permanent);
            m_actor.push_back(m_gold);
            break;
        case 's':
            m_squirt = new Squirt(x, y, dir, this);
            m_actor.push_back(m_squirt); 
            break;
        default:
            break;
            
    }
}

void StudentWorld::annoyActor(Actor* a, int hp)
{
    a->annoy(hp); 
}

bool StudentWorld::hitProtester(Squirt* s)
{
    // Have a function in StudentWorld that takes in a Squirt and checks if the radius between it and all the protesters is less than whatever it has to be less than
    
    bool canBeHit = false;
    for (int i = 0; i < m_actor.size(); i++)
    {
        if (m_actor[i]->getName() == 'r' || m_actor[i]->getName() == 'h')
        {
            if (isRadLessThanNum(m_actor[i]->getX(), m_actor[i]->getY(), s->getX(), s->getY(), 3))
            {
                m_actor[i]->annoy(2);
                canBeHit = true;
            }
        }
    }
    
    return canBeHit;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            CLEAN UP / S - WORLD DESTRUCTOR
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StudentWorld::cleanUp()
{
    // Delete left half of dirt
    
    for (int col = 0; col < 30; col++)
    {
        for (int row = 0; row < 60; row++)
        {
            delete m_dirt[col][row];
        }
    }
    
    // Delete right half of dirt
    for (int col = 34; col < 64; col++)
    {
        for (int row = 0; row < 60; row++)
        {
            delete m_dirt[col][row];
        }
    }
    
    for (int col = 30; col < 34; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            delete m_dirt[col][row];
        }
    }
    
    delete m_frackman;
    
    // Delete contents in vector
    if (!m_actor.empty())
    {
        Actor *a = m_actor[0];
        
        for(int k = 0; k < m_actor.size(); k++)
        {
            delete a;
            m_actor.erase(m_actor.begin() + k);
            k--;
            
            a = m_actor[k+1];
        }
    }
    
}


//Destructor 
StudentWorld::~StudentWorld()
{
    cleanUp();
}

