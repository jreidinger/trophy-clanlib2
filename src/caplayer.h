#ifndef CAPLAYER_H
#define CAPLAYER_H

#include <string>
#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "cacolor.h"
#include "camath.h"

struct CACarType;

/** Covers all data and methods for a single player.
    @author Andrew Mustun
*/
class CAPlayer {
public:
    enum ControlMode { Keyboard, Computer, Network };
    enum Direction   { Left, Right, Straight };
    enum Speed       { Accelerate, Decelerate, Constant };

    CAPlayer( int id, std::string name,
              int carNumber,
              ControlMode controlMode=Keyboard );
    ~CAPlayer();

    void   reset();
    void   resetForRace();

    void   initPlayer( int routeNumber );
    void   renderSprites( CAColor col );

    //! Returns ID of this player.
    int    getId() const {
        return id;
    }
    //! Sets ID of this player.
    void   setId( int i ) {
        id=i;
    }

    //! Returns name of this player.
    std::string getName() const {
        return name;
    }
    //! Sets name of this player.
    void   setName( std::string n ) {
        name=n;
    }

    //! Returns the color of this player.
    CAColor  getColor() const {
        return color;
    }
    void     setColor( CAColor c, bool render=true );

    void     setCarNumber( int carNumber, bool render=true );
    //! Returns current car number (0-CA_NUMCARS).
    int      getCarNumber() {
        return carNumber;
    }

    /** Sets the newCar flag. Used for showing who has bought
        a new car in the position table.
    */
    void     setNewCar( bool nc ) {
        newCar=nc;
    }
    //! Returns the newCar flag.
    bool     getNewCar() {
        return newCar;
    }

    //! Returns pointer to car type.
    CACarType* getCarType() {
        return carType;
    }

    //! Returns control mode of this player (Keyboard, Computer, Network)
    ControlMode getControlMode() {
        return controlMode;
    }
    //! Sets a new control mode for this player.
    void setControlMode( ControlMode cm ) {
        controlMode = cm;
    }

    //! Returns the direction of this player in degrees (0=right, 90=bottom (!)).
    float  getDirection() const {
        return direction;
    }
    void   setDirection(float dir);

    //! Returns the current speed of this player.
    float  getSpeed() const {
        return speed;
    }
    void   setSpeed( float sp );

    //! Returns turbo load left.
    float  getTurbo() const {
        return turbo;
    }
    void   setTurbo( float tb );

    //! Returns true if this player is currently on a bridge, false otherwise.
    bool   isUp() {
        return up;
    }

    //! Returns the current frame number.
    int    getFrame() const {
        return frame;
    }
    //! Sets a new frame number.
    void   setFrame( int f ) {
        frame=f;
    }

    //! Returns true if this player is active (true) or not (false).
    bool   isActive() {
        return active;
    }
    //! Activates or deactivates this player.
    void   activate( bool yes=true ) {
        active=yes;
    }

    //! Returns true if this player has finished the race successfully.
    bool   hasFinished() {
        return finished;
    }

    //! Returns true if this player died.
    bool   isDeath() {
        return death;
    }

    //! Returns the life this player has. (0-100)
    float  getLife() const {
        return life;
    }
    //! Sets a new life amount.
    void   setLife( float l ) {
        life = (l>100 ? 100 : l);
    }

    //! Returns number of bullets left.
    int    getBullets() const {
        return bullets;
    }
    //! Sets number of bullets left.
    void   setBullets(int b) {
        bullets = (b>100 ? 100 : b);
    }

    //! Returns number of fog bombs left (0-3).
    int    getFogBombs() const {
        return fogBombs;
    }
    //! Sets number of fog bombs left (0-3).
    void   setFogBombs(int f) {
        fogBombs = (f>3 ? 3 : f);
    }

    //! Returns current rank of this player in the current/last race.
    int    getRaceRank() const {
        return raceRank;
    }
    //! Sets rank for this player.
    void   setRaceRank(int r) {
        raceRank = r;
    }

    //! Returns time used for the last race in millisecnds.
    int    getRaceTime() {
        return raceTime;
    }
    //! Sets time used for the last race in millisecnds.
    void   setRaceTime( int t ) {
        raceTime = t;
    }

    //! Returns points for the last race.
    int    getRacePoints() {
        return racePoints;
    }
    //! Sets points for the last race.
    void   setRacePoints( int p ) {
        racePoints = p;
    }

    //! Returns current rank of this player over all.
    int    getTotalRank() const {
        return totalRank;
    }
    //! Sets current rank of this player over all.
    void   setTotalRank(int r) {
        totalRank = r;
    }

    //! Returns total points over all.
    int    getTotalPoints() {
        return totalPoints;
    }
    //! Sets total points over all.
    void   setTotalPoints( int p ) {
        totalPoints = p;
    }

    //! Returns the players money.
    int    getMoney() const {
        return money;
    }
    //! Sets the players money.
    void   setMoney( int m ) {
        money = m;
    }

    void   keyControl();
    void   autoPilot();

    void   advance();
    void   checkCollisions();
    void   checkFunctionMap();
    //! Returns true if all edges of the car are on the road.
    bool   checkEdgeState() {
        return (edgeState[0] && edgeState[1] && edgeState[2] && edgeState[3]);
    }

    void   calcEdges();

    void   shoot();
    void   hit( float amount );
    void   kill();

    void   display( int offsetX, int offsetY );
    void   move( float x, float y );
    //! Returns the players position in xp and yp.
    void   getPosition( float &xp, float &yp ) {
        xp=x;
        yp=y;
    }
    //! Returns the x position
    float  getX() {
        return x;
    }
    //! Returns the y position
    float  getY() {
        return y;
    }

    void   activateTurbo();
    void   deactivateTurbo();

    float  getPosition();
    //! Returns current lap number.
    int    getLapNumber() {
        return lapNumber;
    }
    //! Returns current route number. Routes are programmed ways for computer players.
    int    getRouteNumber() {
        return routeNumber;
    }

    void   addHitPoint( int x, int y );
    void   resetHitPoints();

private:
    //! Player id

    int         id;
    //! Player nick name
    std::string   name;
    //! Active
    bool        active;
    //! Are we up on a bridge ?
    bool        up;
    //! Current frame (sprite number)
    int         frame;
    //! Players color (HSV model)
    CAColor     color;

    //! Current car type
    CACarType*  carType;
    //! Car number (index of CA_Trophy::carType[])
    int         carNumber;
    //! Flag which tells us if that player has bought a new car.
    bool        newCar;
    //! Rendered sprites for this player (rotated and color-cycled)
    CL_Surface* sprite[CA_FPR];

    /** Is this sprite moved by keyboard or controlled
        over network or by the computer
    */
    ControlMode controlMode;

    //! current speed in pixel per second
    float  speed;
    //! Current max speed. This might differ from maxSpeed.
    float  cMaxSpeed;
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

    //! Index of next route point to drive to
    int    routePoint;
    //! Route number
    int    routeNumber;

    //! Life   (0-100)
    float  life;
    //! Armor (0-100)
    int    armor;
    //! Turbo  (counts down to 0, 0 = no more turbo left)
    float  turbo;
    //! Is the turbo active? (left shift pressed)
    bool   turboActive;
    //! Money  (0-?)
    int    money;
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
    //! Total race time of last race in milliseconds
    int    raceTime;
    //! Current rank of this player in current race
    int    raceRank;
    //! Points in current race
    int    racePoints;
    //! Current rank in total ranking
    int    totalRank;
    //! Total points of all races
    int    totalPoints;

    //! Array for covering the hit points for one screen
    int    hitPoint[CA_MAXHITPOINTS][2];
    //! current index for the hitPoint array.
    int    hitPointCounter;

public:
    //! speed mode. Accelerate, Decelerate, Constant

    Speed      speedMode;
    //! direction mode. Left, Right, Straight
    Direction  directionMode;
    //! Shoot mode. on/off
    bool       shootMode;
};

#endif

// EOF
