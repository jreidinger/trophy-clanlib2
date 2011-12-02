#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "utils/hsvcolor.h"
#include "utils/trophymath.h"
#include "cartype.h"

class Track;

/** Covers all data and methods for a single player.
    @author Andrew Mustun
    @author Matthieu Lecesne
*/
// TODO: id is uncesssary (in local game), it is only use to know who is the player
class Player 
{
public:
    enum Direction   { Left, Right, Straight };
    enum Speed       { Accelerate, Decelerate, Constant };

    Player( int id, const std::string& name,
            int carNumber);
    virtual ~Player();

    void   reset();
    virtual void   resetForRace(const unsigned int routeNumber, const Track* currentTrack);

    //! Returns ID of this player.
    int    getId() const { return id; }
    //! Sets ID of this player.
    void   setId( int i ) { id=i; }

    //! Returns name of this player.
    std::string getName() const { return name; }
    //! Sets name of this player.
    void   setName( const std::string& n ) { name=n; }

    //! Returns the color of this player.
    HSVColor getColor() const { return color; }
    void     setColor( HSVColor c, bool render=true );

    
    void     buyNewCar(const int carNumber, const bool render = true);
    
    void     setCarNumber(const int carNumber, const bool render = true);
    //! Returns current car number (0-CA_NUMCARS).
    int      getCarNumber() const { return carNumber; }

    //! Returns pointers to player car
    CarType* getCar() { return &m_Pcar; }

    //! Returns the direction of this player in degrees (0=right, 90=bottom (!)).
    float  getDirection() const { return direction; }
    void   setDirection(float dir);


    //! Returns the current speed of this player.
    float  getSpeed() const { return speed; }
    void   setSpeed( float sp );

    //! Returns turbo load left.
    float  getTurbo() const { return turbo; }
    void   setTurbo( const float tb );

    //! Returns true if this player is currently on a bridge, false otherwise.
    bool   isUp() const { return up; }

    //! Returns the current frame number.
    int    getFrame() const { return frame; }
    //! Sets a new frame number.
    void   setFrame( int f ) { frame=f; }

    //! Returns true if this player is active (true) or not (false).
    bool   isActive() const { return active; }
    //! Activates or deactivates this player.
    void   activate( bool yes=true ) { active=yes; }

    //! Returns true if this player has finished the race successfully.
    bool   hasFinished() const { return finished; }

    //! Returns true if this player died.
    bool   isDeath() const { return death; }
	
    //! Returns true if this player is lapped
	bool   isLapped() const { return lapped; }

    //! Returns the life this player has. (0-100)
    float  getLife() const { return life; }
    //! Sets a new life amount.
    void   setLife( float l ) { life = (l>100 ? 100 : l); }

    //! Returns number of bullets left.
    int    getBullets() const { return bullets; }
    //! Sets number of bullets left.
    void   setBullets(int b) { bullets = (b>500 ? 500 : b); }

    //! Returns number of fog bombs left (0-3).
    int    getFogBombs() const { return fogBombs; }
    //! Sets number of fog bombs left (0-3).
    void   setFogBombs(int f) { fogBombs = (f>3 ? 3 : f); }
    //! use a fog bombs
    void   useFogBomb() { fogBombs--;}

    //! Returns current rank of this player in the current/last race.
    int    getRaceRank() const { return raceRank; }
    //! Sets rank for this player.
    void   setRaceRank(int r) { raceRank = r; }

    //! Returns time used for the last race in millisecnds.
    int    getRaceTime() const { return raceTime; }
    //! Sets time used for the last race in millisecnds.
    void   setRaceTime( int t ) { raceTime = t; }

    //! Returns points for the last race.
    int    getRacePoints() const { return racePoints; }
    //! Sets points for the last race.
    void   setRacePoints( int p ) { racePoints = p; }

    //! Returns total points over all.
    int    getTotalPoints() const { return totalPoints; }
    //! Sets total points over all.
    void   setTotalPoints( int p ) { totalPoints = p; }

    //! Returns the players money.
    int    getMoney() const { return money; }
    //! Give Money to player
    void  addMoney(const int value) { money+= value;}
    //! set Money to player
    void setMoney(const int value) { money = value;}
     //! Take Money from player return true if the player had enough money
    bool spendMoney(const int value);

    //! Returns the money that the player has won in the race
    int    getRaceMoney() const { return m_raceMoney; }
    //! Give Race Money to player
    void  addRaceMoney(const int value) { m_raceMoney+= value;}
 
    virtual void pilot() = 0;

    void   advance();
    void   checkCollisions();
    void   checkFunctionMap();
    //! Returns true if all edges of the car are on the road.
    bool   checkEdgeState() { return (edgeState[0] && edgeState[1] && edgeState[2] && edgeState[3]); }

    void   calcEdges();

    void   shoot();
    void   hit( const float amount );
    void   kill();

    virtual void  display( const int offsetX, const int offsetY );
    void   move( float x, float y );
    //! Returns the players position in xp and yp.
    void   getPosition( float &xp, float &yp ) const { xp=x; yp=y; }
    //! Returns the x position
    float  getX() const { return x; }
    //! Returns the y position
    float  getY() const { return y; }

    void   activateTurbo();
    void   deactivateTurbo();

    float  getPosition();
    //! Returns current lap number.
    int    getLapNumber() const { return lapNumber; }

    void   addHitPoint( int x, int y );
    void   resetHitPoints();
    //! Notification of Race Over
    virtual void OnRaceOver() {}

protected:
    //! Returns the new direction of this player in degrees (0=right, 90=bottom (!)).
    float  getNewDirection() const { return newDirection; }

    const Track* getCurrentTrack() {return m_currentTrack;}

private:
    virtual float getCheckAwayAngle() = 0;
    void   renderSprites( HSVColor col );
    //! Player id
    int          id;
    //! Player nick name
    std::string   name;
    //! Active
    bool        active;
    //! Are we up on a bridge ?
    bool        up;
    //! Current frame (sprite number)
    int         frame;
    //! Players color (HSV model)
    HSVColor     color;

    //! Current car type
    CarType     m_Pcar;
    //! Car number (index of CA_Trophy::carType[])
    int         carNumber;
    //! Rendered sprites for this player (rotated and color-cycled)
    CL_Image sprite[CA_FPR];

    //! current speed in pixel per second
    float  speed;
    //! Current max speed. This might differ from maxSpeed (ie during turbo).
    float  cMaxSpeed;
    //! Current acceleration speed. This might differ for accelaration (ie during turbo).
    float cAcceleration;
    //! Direction we want to have (but maybe we still slide towards it)
    float  newDirection;
    /** Current direction (angle in deg). We drive in that direction
        although the sprite might not point towards that direction.
        */
    float  direction;
    //! Block direction? (if we're in collision state)
    bool   blockDirection;

    //! The edge points of the sprite (4 edges, x/y)
    int edge[4][2];
    //! Edge state (true: on the street/ false: in a building / border)
    bool edgeState[4];

    //! Current explosion frame
    float explFrame;
    //! Current x position
    float x;
    //! Current y position
    float y;
    //! Last x position
    float ox;
    //! Last y position
    float oy;

    //! Current velocity in x
    float vx;
    //! Current velocity in y
    float vy;

    //! Life   (0-100)
    float  life;
    //! Turbo  (counts down to 0, 0 = no more turbo left)
    float  turbo;
    //! Is the turbo active? (left shift pressed)
    bool   turboActive;
    //! Money  (0-?)
    int    money;
    //! Race money
    int  m_raceMoney;
    //! Munition left (100-0)
    int    bullets;
    //! Fogbombs (0-3)
    int    fogBombs;

    //! Number of fullfilled laps
    int    lapNumber;
    //! Flags for fullfilled lap parts (max. 0xFFFF)
    unsigned int lapParts;
    //! Last lap part before the current
    int    lastLapPart;
    //! All laps finished
    bool   finished;
    //! Died before finishing the race
    bool   death;
	//! lapped before finishing the race
	bool   lapped;
    //! Total race time of last race in milliseconds
    int    raceTime;
    //! Current rank of this player in current race
    int    raceRank;
    //! Points in current race
    int    racePoints;
    //! Total points of all races
    int    totalPoints;

    //! Array for covering the hit points for one screen
    int    hitPoint[CA_MAXHITPOINTS][2];
    //! current index for the hitPoint array.
    int    hitPointCounter;
    // ! The track where the player is assigned
    const Track* m_currentTrack;


protected:
    //! speed mode. Accelerate, Decelerate, Constant
    Speed      speedMode;
    //! direction mode. Left, Right, Straight
    Direction  directionMode;
    //! Shoot mode. on/off
    bool       shootMode;
};

#endif

// EOF
