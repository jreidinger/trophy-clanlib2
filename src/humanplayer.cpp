#include "humanplayer.h"
#include "caresources.h"
#include "catrophy.h"

/** Constructor
*/
HumanPlayer::HumanPlayer( int id, const std::string& name, int carNumber )
: Player(id, name, carNumber)
{
    // default key
    using namespace ConfigureKey;
    m_keyMap[ACCELERATE]= CL_KEY_UP;
    m_keyMap[BRAKE] = CL_KEY_DOWN;
    m_keyMap[LEFT] = CL_KEY_LEFT;
    m_keyMap[RIGHT] = CL_KEY_RIGHT;
    m_keyMap[SHOOT] = CL_KEY_X;
    m_keyMap[BOMB] = CL_KEY_C;
    m_keyMap[BOOST] = CL_KEY_A;
    m_keyMap[HORN] = CL_KEY_SPACE;
}

HumanPlayer::~HumanPlayer()
{}

/** Controls the player by keyboard
*/
void HumanPlayer::pilot()
{
    // We're death and can't drive anymore:
    //
    using namespace ConfigureKey;
    if( isDeath() || hasFinished()  || isLapped()) {
        speedMode = Constant;
    }

    // We're alive:
    //
    else {
        // Accelerate (E/UP):
        //
        if (CA_APP->keyboard.get_keycode(m_keyMap[ACCELERATE]))
        {
            speedMode = Accelerate;
        }

        // Decelerate (D/DOWN):
        //
        else if (CA_APP->keyboard.get_keycode(m_keyMap[BRAKE]))
        {
            speedMode = Decelerate;
        }

        // Don't change speed:
        //
        else {
            speedMode = Constant;
        }
    }

    if( !isDeath() ) {
        // Steer left:
        //
        if (CA_APP->keyboard.get_keycode(m_keyMap[LEFT]))
        {
            directionMode = Left;
        }

        // Steer right:
        //
        else if (CA_APP->keyboard.get_keycode(m_keyMap[RIGHT]))
        {
            directionMode = Right;
        }

        // Don't change direction:
        //
        else {
            directionMode = Straight;
        }
    }

    // Horn:
    //
    if (CA_APP->keyboard.get_keycode(m_keyMap[HORN]))
    {
        CA_RES->effectHorn->play();
    }

    // Shoot:
    //
    if (CA_APP->keyboard.get_keycode(m_keyMap[SHOOT]))
    {
        if( !hasFinished() && !isDeath() && !isLapped() && CA_APP->allowShooting && getBullets()>0 ) {
            CA_RES->effectShoot->play( 2 );
            shoot();
        }
    } else {
        shootMode = false;
    }

    // Drop fog bomb:
    //
    static bool blockKeyF = false;
    if (CA_APP->keyboard.get_keycode(m_keyMap[BOMB]))
    {
        if( !hasFinished() && !isDeath() && !isLapped() && CA_APP->allowShooting && !blockKeyF && getFogBombs()!=0 ) {
            CA_APP->dropFogBomb( int(getX()), int(getY()), isUp() );
            useFogBomb();
            blockKeyF = true;
        }
    } else {
        blockKeyF = false;
    }

    // Turbo:
    //
    if (CA_APP->keyboard.get_keycode(m_keyMap[BOOST]))
    {
        activateTurbo();
    } else {
        deactivateTurbo();
    } 
}

float HumanPlayer::getCheckAwayAngle()
{
    // Check car away (computer players check away stronger):
    return 2.5f;
}

// EOF
