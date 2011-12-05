#include "computerplayer.h"
#include "caresources.h"
#include "catrophy.h"
#include "track.h"

ComputerPlayer::ComputerPlayer(int id, const std::string& name, int carNumber):
Player(id, name, carNumber),
m_routePoint(0),
m_routeNumber(0),
nxTemp(0.0),
nyTemp(0.0)
{}

ComputerPlayer::~ComputerPlayer()
{}

void ComputerPlayer::display(  const int offsetX, const int offsetY )
{
    Player::display(offsetX, offsetY);
    if( CA_APP->trackInfo )
        CL_Draw::line( *CA_APP->graphicContext,getX()+offsetX, getY()+offsetY, nxTemp + offsetX, nyTemp + offsetY, CL_Colorf::white);
}

/** On Race Over, computers players are checking if they could buy a new car
*/
void ComputerPlayer::OnRaceOver()
{
    for( int ca=CA_NUMCARTYPES-1; ca>=0; --ca )
    {
        if( getMoney() >= CA_APP->carType[ca].price && getCarNumber()<ca )
        {
            buyNewCar( ca );
        }
    }
}


/** Calculates route of computer players.
*/
void ComputerPlayer::pilot()
{
    float nx, ny;
    getCurrentTrack()->getNextRoutePoint(m_routeNumber, m_routePoint, nx, ny);

    // For --trackinfo option
    //
    nxTemp = nx;
    nyTemp = ny;

    // Angle to next route point:
    //
    float b = TrophyMath::getAngle( getX(), getY(), nx,ny );
    float diff = TrophyMath::getAngleDiff( getDirection(), b );
    float dist = TrophyMath::getDistance( getX(),getY(), nx,ny );

    const bool retard = false; // TODO: useless code (retard is always false)

    // Steer:
    //
    // TODO : autoPilot should also take care of functionMap pixel that are under 
    // edges of the car (or better, next to the edges)
    if (diff<20.0 || diff>340.0) {
        directionMode = Straight;
    } else if (diff<180.0) {
        // TODO : was commented
        //if( diff>40.0 ) retard=true;
        directionMode = Right;
    } else if (diff>180.0) {
        // TODO : was commented
        //if( diff<320.0 ) retard=true;
        directionMode = Left;
    }

    // Switch to next route point:
    //
    // TODO : clean these constants all over the code (original = 120)!
    // This constant is the distance where the computer take the next route Point
    if (dist < 120.0) {
        ++m_routePoint;
    }
    


    // Speed:
    //
    if( !isDeath() && !hasFinished() && !isLapped() && !retard )
		speedMode=Accelerate;
    else // TODO: useless code (retard is always false)
		speedMode=Constant;


    if( !isDeath() && !hasFinished() && !isLapped() ) 
    {
        // Decide wheter to activate turbo or not:
        //
        // TODO : strange ... 
        // CATrophy::Easy -> turboLaunchDistance = 600 or deactivateTurbo() ?
        if( CA_APP->difficulty!=CATrophy::Easy )
        {
            int turboLaunchDistance = 600; // TODO : the turbo is use even before turbo launh distance
            switch( CA_APP->difficulty )
            {
            case CATrophy::Easy:
                turboLaunchDistance = 600; // TODO : useless : unreachable code
                break;
            case CATrophy::Medium:
                turboLaunchDistance = 200;
                break;
            case CATrophy::Hard:
                turboLaunchDistance = 50;
                break;
            }
            int xt = (int)(getX() + cos( getNewDirection()/ARAD ) * turboLaunchDistance);
            int yt = (int)(getY() + sin( getNewDirection()/ARAD ) * turboLaunchDistance);
            int xt2 = (int)((getX() + xt)/2);
            int yt2 = (int)((getY() + yt)/2);
            if( getTurbo()>0.0 &&
                    CA_APP->getSpeedLimit(xt,yt)==15 &&
                    CA_APP->getSpeedLimit(xt2,yt2)==15 ) {
                activateTurbo();
            } else {
                deactivateTurbo();
            }
        } else {
            deactivateTurbo();
        }

        // Decide wheter to shoot or not:
        //
        if( CA_APP->allowShooting && getBullets()!=0 ) 
        {
            int distance;
            int angle;
            int angleDiff;
            shootMode = false;
            for( int pl=0; pl<CA_RACEMAXPLAYERS; ++pl )
            {
                if( pl!= getId() )
                {
                    distance = (int)TrophyMath::getDistance( getX(), getY(), CA_APP->m_RacePlayer[pl]->getX(),CA_APP->m_RacePlayer[pl]->getY() );
                    angle = (int)TrophyMath::getAngle( getX(), getY(), CA_APP->m_RacePlayer[pl]->getX(),CA_APP->m_RacePlayer[pl]->getY() );
                    angleDiff = (int)TrophyMath::getAngleDiff( angle, getNewDirection() );
                    if( distance<CA_SHOOTINGRANGE && (angleDiff < 10.0 || angleDiff > 350.0) )
                    {
                        shoot();
                        break;
                    }
                }
            }
        }

        // Decide wheter to drop a fog bomb or not:
        //
        if( CA_APP->allowShooting && getFogBombs()!=0 )
        {
            int distance;
            int angle;
            int angleDiff;
            for( int pl=0; pl<CA_RACEMAXPLAYERS; ++pl )
            {
                if( pl!=getId() )
                {
                    distance = (int)TrophyMath::getDistance( getX(), getY(), CA_APP->m_RacePlayer[pl]->getX(), CA_APP->m_RacePlayer[pl]->getY() );
                    angle = (int)TrophyMath::getAngle( getX(), getY(), CA_APP->m_RacePlayer[pl]->getX(), CA_APP->m_RacePlayer[pl]->getY() );
                    angleDiff = (int)TrophyMath::getAngleDiff( angle, getNewDirection() );
                    if( distance > 200 && distance < 202 && angleDiff < 185.0 && angleDiff > 175.0 )
                    {
                        CA_APP->dropFogBomb( int(getX()),int(getY()), isUp() );
                        useFogBomb();
                        break;
                    }
                }
            }
        }
    }
}

/** Reset things before a new race starts.
*/
void
ComputerPlayer::resetForRace(const unsigned int routeNumber, Track* currentTrack)
{
    Player::resetForRace(routeNumber, currentTrack);
    m_routeNumber = routeNumber;
    m_routePoint = 1;
}


float ComputerPlayer::getCheckAwayAngle()
{
    // Check car away (computer players check away stronger):
    return 7.5f;
}
// EOF
