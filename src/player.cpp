#include "player.h"

#include "cartype.h"
#include "caimagemanipulation.h"
#include "catrophy.h"
#include "utils/trophymath.h"
#include "capositiontable.h"

/** Constructor.
    \param id     Id of this player (for network) or -1 for no id
    \param name   Name of this player
    \param carNumber The car number from 0 to CA_MAXPLAYERS
    \param moving Is this sprite controlled by the player (Keyboard),
                  the computer (Computer) or a net player (Network)
*/
Player::Player( int id, const std::string& name,
                    int carNumber,
                    ControlMode controlMode )
: m_Pcar(CA_APP->carType[carNumber])
{
    for( int i=0; i<CA_FPR; ++i )
    {
        sprite[i] = 0;
    }

    this->id = id;
    this->name = name;
    this->controlMode = controlMode;
    this->carNumber = carNumber;
    
    cMaxSpeed = m_Pcar.getMotor()->getMaxSpeed();
    cAcceleration = m_Pcar.getMotor()->getAcceleration();
    active = true;

    explFrame = 0.0;
    vx=vy= 0.0;

    ox=oy = 0.0;

    reset();
}

/** Destructor.
*/
Player::~Player()
{
    for( int i=0; i<CA_FPR; ++i )
    {
        if( sprite[i] )
        {
            delete sprite[i];
            sprite[i] = 0;
        }
    }
}

/** Resets this player.
*/
void
Player::reset() {
    money         = 1000;                 // Initial money for each player
    totalPoints   = 0;
    totalRank     = 0;

    resetForRace();
}

/** Reset things before a new race starts.
*/
void
Player::resetForRace() {
    active = true;

    setFrame( 0 );

    speed         = 0.0;
    direction     = 0.0;
    newDirection  = 0.0;
    up            = false;
    speedMode     = Constant;
    directionMode = Straight;
    blockDirection= false;
    shootMode     = false;

    life          = 100.0;
    bullets       = 500;
    fogBombs      = 2;

    racePoints  = 0;
    raceRank    = 0;
    raceTime    = 0;

    turbo       = m_Pcar.maxTurbo;    // Current turbo load (in pixel!)
    turboActive = 0;

    resetHitPoints();

    routeNumber   = 0;

    lapNumber     = 0;
    lapParts      = 1;
    lastLapPart   = 0;
    finished      = false;
    death         = false;
}

/** Sets a new color for this player.
    \param col New car color
    \param render Render new sprites?
*/
void
Player::setColor( HSVColor col, bool render ) {
    if( color!=col || sprite[0]==0 ) {
        color = col;
        if( render ) renderSprites( color );
    }
}

/** buy a new car (car number 0-CA_NUMCARS)
    \param carNumber (index of CATrophy::carType)
 */
void
Player::buyNewCar(const int carNumber, const bool render)
{
    setCarNumber(carNumber, render);
    spendMoney( CA_APP->carType[carNumber].price );
}


/** Sets a new car for this player.
    \param carNumber Car number (index of CATrophy::carType[])
    \param render Render new sprites?
*/
void
Player::setCarNumber( const int carNumber,const bool render ) {
    if( carNumber<CA_NUMCARTYPES ) m_Pcar = CarType(CA_APP->carType[carNumber]);
    else                           m_Pcar = CarType(CA_APP->carType[0]);
    if (this->carNumber != carNumber)
    {
        this->carNumber = carNumber;

        if( render ) renderSprites( color );
    }
}

/** Init a computer player. This method moves the player to the start
    point and gives the right direction.
    \param routeNumber The initial route number for this player
*/
void
Player::initPlayer( int routeNumber )
{
    //reset();
    this->routeNumber = routeNumber;
    move( CA_APP->track.rp[routeNumber][0][0], CA_APP->track.rp[routeNumber][0][1] );
    setDirection( CA_APP->track.startAngle );
}

/** Renders the sprites for this player. The car image gets
    rotated and the color adjusted to 'color'.
*/
void
Player::renderSprites( HSVColor col ) 
{
    /*
       CL_Surface* tmpSf = CAImageManipulation::changeHSV( m_Pcar.surface,
       color.h, color.s, color.v );
       for( int i=0; i<CA_FPR; ++i ) {
       if( sprite[i] ) delete sprite[i];
       sprite[i] = CAImageManipulation::rotate( tmpSf, (float)i/CA_FPR*360.0, true );
       }
       delete tmpSf;
       */

    // Due to a ClanLib bug introduced with ClanLib 0.5 I have to use
    // green as the transparency channel - which results in this
    // massive overhead of calculations / waste of memory - sorry.
    CL_Surface* tmpSf;
    tmpSf = CAImageManipulation::changeHSV( m_Pcar.surface, color.h, color.s, color.v );
    for( int i=0; i<CA_FPR; ++i ) 
    {
        if( sprite[i] ) delete sprite[i];
        sprite[i] = new CL_Surface( *tmpSf );
        sprite[i]->rotate( (float)i/CA_FPR*360.0 );
    }
    delete tmpSf;
}

/** Changes the direction and the frame of this sprite.
*/
void
Player::setDirection( float dir ) 
{
    newDirection=dir;
    if( newDirection >= 360.0 ) newDirection-=360.0;
    if( newDirection <    0.0 ) newDirection+=360.0;

    int newFrame = (int)(newDirection/360.0*CA_FPR);
    setFrame( newFrame );
}

/** Changes the speed of this sprite.
*/
void
Player::setSpeed( float sp ) 
{
    speed = sp;

    if( speed > cMaxSpeed         ) speed = cMaxSpeed;
    if( speed < m_Pcar.minSpeed ) speed = m_Pcar.minSpeed;

}

/** Changes the turbo load of this sprite.
*/
void
Player::setTurbo( float tb ) {
    turbo = tb;

    if( turbo > m_Pcar.maxTurbo ) turbo = m_Pcar.maxTurbo;
    if( turbo < 0                 ) turbo = 0;
}

bool Player::spendMoney(const int value)
{
    bool wasAbleToBuy = false;
    if (money >= value)
    {
        money-= value;
        wasAbleToBuy = true;
    }
    return wasAbleToBuy;
}


/** Called on every "step" the game makes.
    The Sprite moves according to speed and direction.
*/
void
Player::advance() 
{
    // Play engine sound (Volume of engine sound depends on speed):
    //
    if( id==0 && CA_APP->sound ) 
    {
        CA_RES->effectEngine->play();
        if( CA_APP->sound ) CA_RES->effectEngine->setVolume( speed / cMaxSpeed );
    }

    // Slide towards direction:
    //
    if( !TrophyMath::compFloat(direction, newDirection) ) {
        float dirStep = (m_Pcar.steeringPower / CA_APP->framesPerSec) * m_Pcar.getTires()->getSlidingFactor();

        if( (newDirection>direction && newDirection-direction<=180.0) ||
                (newDirection<direction && direction-newDirection> 180.0) ) {
            if( newDirection-direction < dirStep ) {
                direction = newDirection;
            } else {
                direction += dirStep;
            }
        } else {
            if( direction-newDirection < dirStep ) {
                direction = newDirection;
            } else {
                direction -= dirStep;
            }
        }

        // Play slide sound:
        //
        if( id==0 && fabs(direction-newDirection)>=15.0 && fabs(direction-newDirection)<=16.0 ) {
            if( CA_APP->sound ) CA_RES->effectSlide->play();
        }

        if( fabs(direction-newDirection)>=15.0 ) {
            CA_APP->makeDustClowd( (edge[2][0]+edge[3][0])/2,
                                   (edge[2][1]+edge[3][1])/2,
                                   up );
        }
    }

    // Die now (car burns):
    //
    if( death )  {

        // One fire frame stays for 1/8s:
        //
        //CA_RES->advanceAnimation( &explFrame, CA_RES->misc_carfire->get_frame_surface(CA_RES->misc_carfire->get_current_frame()), 8.0 );
        if( id==0 ) CA_RES->effectFire->play( 5 );
    }


    if( controlMode!=Network && active )  {

        // Check collisions with other players / goodies
        //
        checkCollisions();

        // Check underlying function map:
        //
        checkFunctionMap();

        // Regulate Speed:
        //
        if( speedMode==Accelerate ) setSpeed( speed + (cAcceleration / CA_APP->framesPerSec) );
        else if( speedMode==Decelerate ) setSpeed( speed - (m_Pcar.deceleration / CA_APP->framesPerSec) );
        else                             {
            setSpeed( speed * 0.96 );
            if( fabs(speed)<10.0 ) speed=0.0;
        }

        // Control direction:
        //
        // TODO : It seems that the turning problem of cars is here ... maybe when steeringPower < 1.5 ?
        // TODO: examines what happens when speed > getMaxSpeed (ie during turbo)
        float steeringPower = (m_Pcar.steeringPower / CA_APP->framesPerSec) * (speed / m_Pcar.getMotor()->getMaxSpeed());
        //if( steeringPower<1.5 && steeringPower>=0.0 ) steeringPower = 1.5;
        if( (steeringPower<200 / CA_APP->framesPerSec) && steeringPower>=0.0 ) steeringPower = 200 / CA_APP->framesPerSec;

        if( directionMode==Right ) setDirection( newDirection+steeringPower );
        if( directionMode==Left  ) setDirection( newDirection-steeringPower );


        // Calculate speed vector:
        //
        vx = cos( direction/ARAD ) * (speed / CA_APP->framesPerSec);
        vy = sin( direction/ARAD ) * (speed / CA_APP->framesPerSec);

        float nx = x + vx;
        float ny = y + vy;

        // Moving out of map?
        //
        if( nx < 0 || nx >= CA_APP->track.visualMap->get_width() ) vx *= -1;
        if( ny < 0 || ny >= CA_APP->track.visualMap->get_height() ) vy *= -1;

        // Unload turbo:
        //
        if( turboActive && turbo>0.0 ) {
            setTurbo( turbo - TrophyMath::getDistance( x,y, nx,ny ) );
            if( turbo<=0.0 ) deactivateTurbo();
        }

        // Auto Reload turbo
        //
        else {
            setTurbo( turbo + TrophyMath::getDistance( x,y, nx,ny ) * (m_Pcar.maxTurbo/5000.0) );
        }

        // Move now:
        //
        move( nx, ny );

        // Recalculate the borders / edges:
        //
        calcEdges();
    }
}

/** Checks collisions of this sprite with others.
*/
void
Player::checkCollisions()
{
    // Other players:
    //
    for( int c=0; c<CA_RACEMAXPLAYERS; ++c )
    {
        Player* pl = CA_APP->m_RacePlayer[c];
        if( pl!=this )
        {
            if( x+32 > pl->getX()-32 && x-32 < pl->getX()+32 &&
                    y+32 > pl->getY()-32 && y-32 < pl->getY()+32 &&
                    up == pl->isUp() )
            {

                bool inters = false;
                int ix, iy;

                for( int i=0; i<4 && !inters; ++i )
                {
                    int ni = i+1;
                    if( ni==4 ) ni=0;

                    for( int j=0; j<4 && !inters; ++j )
                    {
                        int nj = j+1;
                        if( nj==4 ) nj=0;

                        if( TrophyMath::getIntersection( edge[i][0], edge[i][1], edge[ni][0], edge[ni][1],
                                                     pl->edge[j][0], pl->edge[j][1], pl->edge[nj][0], pl->edge[nj][1],
                                                     &ix, &iy) )
                        {
                            inters = true;
                        }
                    }
                }

                if( inters )
                {
                    // Play crash sound:
                    //
                    if( id==0 || pl->getId()==0 )
                    {
                        if( CA_APP->sound ) CA_RES->effectCrash->play();
                    }

                    float ang = TrophyMath::getAngle (x, y, pl->getX(), pl->getY());
                    if( pl->checkEdgeState() )
                    {      // Don't check players into borders
                        pl->move( pl->getX()+cos(ang/ARAD)*4,
                                  pl->getY()+sin(ang/ARAD)*4 );
                        pl->setSpeed( pl->getSpeed()*0.98 );
                        pl->hit( 0.1 );
                    }

                    if( checkEdgeState() )
                    {
                        move( x-cos(ang/ARAD)*4,
                              y-sin(ang/ARAD)*4 );
                        setSpeed( getSpeed()*0.98 );
                        hit( 0.1 );
                    }
                }
            }
        }
    }


    // Goodies:
    //
    for( int gt=0; gt<CA_NUMGOODYTYPES; gt++ )
    {
        for( int gi=0; gi<CA_NUMGOODIES; gi++ )
        {
            CAGoody* go = CA_APP->goody[gt][gi];

            if( go->isActive() ) {
                if( go->isUp()==isUp() &&
                        TrophyMath::getDistance( x,y, go->getX(),go->getY() ) < 28.0 )
                {

                    if( go->getType()->name == "Turbo" )
                    {
                        setTurbo( turbo+1000 );
                        if( id==0 ) CA_RES->effectGoodyTurbo->play();
                    }
                    else if( go->getType()->name == "Life" )
                    {
                        if( !death && !finished ) setLife( life+25.0 );
                        if( id==0 ) CA_RES->effectGoodyLife->play();
                    }
                    else if (go->getType ()->name == "Bullets")
                    {
                        setBullets( bullets+100 );
                        if( id==0 ) CA_RES->effectGoodyBullets->play();
                    }
                    else if (go->getType ()->name == "Fogbomb") {
                        setFogBombs( fogBombs+1 );
                        if( id==0 ) CA_RES->effectGoodyFogbomb->play();
                    }
                    else if( go->getType()->name == "Money" )
                    {
                        money+=100;
                        if( id==0 ) CA_RES->effectGoodyMoney->play();
                    }
                    go->reset();
                }
            }
        }
    }
}

/** Checks underlying function map.
*/
void
Player::checkFunctionMap() 
{
    // lapParts is a bit pattern of fulfilled lap parts (15 parts total)
    //   0 0 0 0  0 0 0 0   0 0 0 0  0 0 1 1 = Lap part 1 and 2 fulfilled
    //   0 1 1 1  1 1 1 1   1 1 1 1  1 1 1 1 = All lap parts fulfilled

    int i;
    int px, py;
    unsigned int r,g,b,a;
    unsigned int speedNibbleEdge=15;

    // Color below car edges:
    //
    for( i=0; i<4; ++i ) 
    {
        px = edge[i][0];
        py = edge[i][1];

        // Car edge is out of map:
        //
        if( !CA_APP->checkCoordinate( px,py ) ) continue;

        // Get the function pixel under this edge:
        //
        CL_Color tmp = CA_APP->track.functionMap->get_pixel( edge[i][0],edge[i][1] );
        g = tmp.get_green();

        // Calculate speed reduction:
        //
        if( ((g&0xF0)>>4) < speedNibbleEdge ) speedNibbleEdge = ((g&0xF0)>>4);

        // Collision:
        //
        if( ((g&0xF0)>>4)==0 || (up && (g&0x04)!=0) || (!up && (g&0x08)!=0) ) 
        {
            edgeState[i] = false;   // Edge is in a building

            if( (speed>0 && i<2) || (speed<0 && i>1) ) 
            {
                // Check car away (computer players check away stronger):
                //
                /*float cangle=2.5;
                if( controlMode==Computer ) cangle=7.5;*/
                float cangle = getCheckAwayAngle();
                switch(i) 
                {
                    default:
                    case 0:
                        if( speed>0 ) setDirection( TrophyMath::corrAngle(newDirection-cangle) );
                        break;
                    case 1:
                        if( speed>0 ) setDirection( TrophyMath::corrAngle(newDirection+cangle) );
                        break;
                    case 2:
                        if( speed<0 ) setDirection( TrophyMath::corrAngle(newDirection-cangle) );
                        break;
                    case 3:
                        if( speed<0 ) setDirection( TrophyMath::corrAngle(newDirection+cangle) );
                        break;
                }
                direction = newDirection;

                // Pull car back:
                //
                x = ox - cos( newDirection/ARAD ) * (speed / CA_APP->framesPerSec / 2);
                y = oy - sin( newDirection/ARAD ) * (speed / CA_APP->framesPerSec / 2);
                speed *= 0.9;
            }
        } else {
            edgeState[i] = true;     // Edge is on the street / gras / ...
        }
    }

    // Color below car center:
    //
    px = (int)x;
    py = (int)y;

    if( !CA_APP->checkCoordinate( px,py ) ) return;

    CL_Color tmp = CA_APP->track.functionMap->get_pixel( px,py );
    r = tmp.get_red();
    g = tmp.get_green();
    b = tmp.get_blue();
    a = tmp.get_alpha();

    // TODO : It seems to be the contrary (15=fast, ... 0 = slow) ?
    unsigned int speedNibble = (g&0xF0)>>4;   // Speed regulation (0=fast ... 15=slow)
    //bool oilPuddle = (r&0x80);     // Oil puddle
    int lapPart = b&0x1F;            // We are passing lap mark x (01-3F)

    if( speedNibbleEdge < speedNibble ) speedNibble = speedNibbleEdge;

    // Speed limit:
    //
    if( speedNibble!=0 ) {
        if( speed > (cMaxSpeed/15*speedNibble) ) speed = (cMaxSpeed/15*speedNibble);
    }

    // Oil puddle
    //
    //if (oilPuddle) {
    //setDirection (direction+(5.0*speed));
    //setSpeed (speed*0.5);
    //}

    // Lift (up or down before bridges and subways):
    //
    if( (g&0x02)!=0 ) up = true;
    else if( (g&0x01)!=0 ) up = false;

    // Lap part:
    //
    if( lapPart!=0 && !finished && !death ) 
    {
        // We pass another lap part:
        //
        lapParts = (lapParts | (1 << (lapPart - 1)));

        /*if(id==0 && CA_APP->debug) {
          for( i=31; i>=0; --i ) {
            std::cout << (((lapParts & (1<<i)) == 0) ? '0' : '1');
          }
          std::cout << std::endl;
    }*/

        // We finished a lap:
        //
        if( lapPart==1 && (getPosition()-lapNumber)>0.93 ) {
            if( lastLapPart==31 || lastLapPart==30 ) {
                lapNumber++;

                // We finished the race:
                //
                if( lapNumber==CA_NUMLAPS ) {
                    finished = true;
                    raceTime = CA_APP->getTime();
                    CAPositionTable::getPositionTable()->playerFinishedRace( this );
                }
            }
            lapParts=1;
        }

        lastLapPart = lapPart;
    }
}

/** Calculate edges of the sprite
*/
void
Player::calcEdges() {
    float dx, dy;

    dx = cos( (newDirection + m_Pcar.angle) / ARAD ) * m_Pcar.radius;
    dy = sin( (newDirection + m_Pcar.angle) / ARAD ) * m_Pcar.radius;

    edge[0][0] = (int)(x + dx);
    edge[0][1] = (int)(y + dy);

    edge[2][0] = (int)(x - dx);
    edge[2][1] = (int)(y - dy);

    dx = cos( (newDirection - m_Pcar.angle) / ARAD ) * m_Pcar.radius;
    dy = sin( (newDirection - m_Pcar.angle) / ARAD ) * m_Pcar.radius;

    edge[1][0] = (int)(x + dx);
    edge[1][1] = (int)(y + dy);

    edge[3][0] = (int)(x - dx);
    edge[3][1] = (int)(y - dy);
}

/** Shooting
*/
void
Player::shoot()
{
    // Count down bullets:
    //
    if( bullets>0 )
    {
        bullets--;

        shootMode = true;
        int pl, en, en2;                     // Counters
        int rayX1, rayY1, rayX2, rayY2;      // Coordinates of shooting ray
        bool inters;                         // Intersection found?
        int ix, iy;                          // Intersection
        int hitX=0, hitY=0;                  // Position we've actually hit
        int hitPl=-1;                        // Player we've actually hit
        int dist;                            // Current distance to hit point
        int minDist=10000;                   // shortest distance to the hit point

        // Calculate shoot line:
        //
        rayX1 = (int)x;
        rayY1 = (int)y;
        rayX2 = (int)(x + cos( newDirection/ARAD ) * CA_SHOOTINGRANGE);
        rayY2 = (int)(y + sin( newDirection/ARAD ) * CA_SHOOTINGRANGE);

        // Look for a hit player:
        //
        for( pl=0; pl<CA_RACEMAXPLAYERS; ++pl )
        {
            if( CA_APP->m_RacePlayer[pl]!=this && isUp()==CA_APP->m_RacePlayer[pl]->isUp() )
            {
                for( en=0; en<4; ++en )
                {
                    en2 = ((en==3) ? 0 : (en+1));
                    inters = TrophyMath::getIntersection( rayX1, rayY1, rayX2, rayY2,
                                                      CA_APP->m_RacePlayer[pl]->edge[en][0],
                                                      CA_APP->m_RacePlayer[pl]->edge[en][1],
                                                      CA_APP->m_RacePlayer[pl]->edge[en2][0],
                                                      CA_APP->m_RacePlayer[pl]->edge[en2][1],
                                                      &ix, &iy );
                    if( inters )
                    {
                        dist = (int)TrophyMath::getDistance( rayX1, rayY1, ix,iy );
                        if( dist<minDist )
                        {
                            minDist = dist;
                            hitPl = pl;
                            hitX = ix + (int)(cos( newDirection/ARAD ) * 5);
                            hitY = iy + (int)(sin( newDirection/ARAD ) * 5);
                        }
                    }
                }
            }
        }

        if( hitPl>=0 && hitPl<CA_RACEMAXPLAYERS )
        {
            CA_APP->m_RacePlayer[hitPl]->addHitPoint( hitX, hitY );
            CA_APP->m_RacePlayer[hitPl]->hit( 0.16 );
        }
    }
}

/** Player was hit.
    \param amount How hard was the hit?
*/
void
Player::hit( float amount ) {
    if( !death && !finished ) {
        life -= amount/100.0*(100.0-m_Pcar.getArmor()->getArmor());
        if( life<0.1 ) {
            kill();
        }
    }
}

/** Player was killed.
*/
void
Player::kill() {
    if( !death ) {
        life=0.0;
        death=true;
        CAPositionTable::getPositionTable()->playerDied( this );
    }
}

/** Displays the player on it's current position.
*/
void
Player::display( const int offsetX, const int offsetY )
{
    sprite[frame]->draw ( (int)(x+offsetX - sprite[ frame ]->get_width()/2),
                          (int)(y+offsetY - sprite[ frame ]->get_height()/2) );

    // Display hit points:
    //
    for( int c=0; c<hitPointCounter; ++c ) 
    {
        CA_RES->misc_hitpoint->set_frame(TrophyMath::getRandomNumber( 0,4 ));
        CA_RES->misc_hitpoint->draw ( hitPoint[c][0]+offsetX - CA_RES->misc_hitpoint->get_width()/2,
                                      hitPoint[c][1]+offsetY - CA_RES->misc_hitpoint->get_height()/2);
    }
    resetHitPoints();

    // Display gun fire:
    //
    if( !death && !finished && shootMode && bullets>0 ) 
    {
        int gunX, gunY;
        gunX = (int)(x + cos( newDirection/ARAD ) * 18) - CA_RES->misc_gunfire->get_width()/2;
        gunY = (int)(y + sin( newDirection/ARAD ) * 18) - CA_RES->misc_gunfire->get_height()/2;
        CA_RES->misc_gunfire->set_frame((int)((float)newDirection/5.0));
        CA_RES->misc_gunfire->draw ( gunX+offsetX, gunY+offsetY );
    }

    // Display car fire if we're death or if we are almost dead:
    // TODO: differentate death and almost dead
    if( death || life <10) 
    {
        CA_RES->misc_carfire->set_frame((int)floor(explFrame));
        CA_RES->misc_carfire->draw ( (int)(x+offsetX - CA_RES->misc_carfire->get_width()/2),
                                          (int)(y+offsetY - CA_RES->misc_carfire->get_height()/2));
    }
}

/** Reimplemented for moving the shadow with the sprite.
*/
void
Player::move( float x, float y ) {
    ox = this->x;
    oy = this->y;
    this->x = x;
    this->y = y;
}

/** Activates turbo mode.
*/
void
Player::activateTurbo() {
    if( turbo>0 ) {
        cMaxSpeed = m_Pcar.getMotor()->getMaxSpeed() * CA_TURBOFACTOR;
        cAcceleration = m_Pcar.getMotor()->getAcceleration() * CA_TURBOFACTOR;
        turboActive = true;
    }
}

/** Deactivates turbo mode.
*/
void
Player::deactivateTurbo() {
    cMaxSpeed = m_Pcar.getMotor()->getMaxSpeed();
    cAcceleration = m_Pcar.getMotor()->getAcceleration();
    turboActive =false;
}

/** Returns the approximate position in laps as a float.
    e.g. 3.5 means the player has finished 3.5 laps
*/
float
Player::getPosition() {
    float ret = lapNumber;
    int fraction=0;
    int leftOut=0;

    for( int f=0; f<32; ++f ) {
        if( (lapParts&(1<<f))!=0 ) {
            fraction = f;
            leftOut=0;
        } else {
            ++leftOut;
            if(leftOut>1) break;  // Leave out max 1 part
        }
    }

    ret += fraction/32.0;

    return ret;
}

/** Resets all hit points. This happens once for every screen.
*/
void
Player::resetHitPoints() {
    hitPointCounter = 0;
}

/** Adds a hit point for the next screen.
*/
void
Player::addHitPoint( int x, int y ) {
    if( hitPointCounter<CA_MAXHITPOINTS ) {
        hitPoint[hitPointCounter][0] = x;
        hitPoint[hitPointCounter][1] = y;
        hitPointCounter++;
    }
}

// EOF
