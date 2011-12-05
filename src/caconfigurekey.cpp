#include "caconfigurekey.h"
#include "caresources.h"
#include "caimagemanipulation.h"
#include "catrophy.h"
#include "humanplayer.h"
#include "caconfigurekey.h"

/** Constructor.
*/
CAConfigureKey::CAConfigureKey(HumanPlayer* human)
    : CADialog(),
    m_human(human),
    m_keyMode(ConfigureKey::ACCELERATE)
{
    resize( 600, 300 ); // a white rectangle
    title = "Configure Keyboard";
    using namespace ConfigureKey;
    m_keyString[ACCELERATE]= "ACCELERATE";
    m_keyString[BRAKE] = "BRAKE";
    m_keyString[LEFT] = "STEER LEFT";
    m_keyString[RIGHT] = "STEER RIGHT";
    m_keyString[SHOOT] = "SHOOT";
    m_keyString[BOMB] = "DROP BOMB";
    m_keyString[BOOST] = "BOOST";
    m_keyString[HORN] = "HORN";
}

CAConfigureKey::~CAConfigureKey()
{ }

void CAConfigureKey::buildScreen()
{
    CADialog::buildScreen();
    const std::string text = std::string ("Press Key For ") + m_keyString[m_keyMode];
    CA_RES->font_normal_14_white.draw_text ( *CA_APP->graphicContext, (left+32+right-32)/2, (top+32+bottom-32)/2, text );
}

/** Called on key released.
*/
void CAConfigureKey::on_key_released( const CL_InputEvent &key, const CL_InputState &state ) 
{
    switch( key.id ) {

        // Cancel (ESC):
    case CL_KEY_ESCAPE:
        cancel = true;
        done = true;
        break;

        // Activate:
    case CL_KEY_ENTER:
        done = true;
        break;

       // define the key
    default:
        m_keyMap[m_keyMode]=key.id; 
        if (m_keyMode != ConfigureKey::HORN)
        {
           m_keyMode = ConfigureKey::DefineKey(int(m_keyMode)+1);
           buildScreen();
        }
        else
        {
           done = true;
           m_keyMode = ConfigureKey::ACCELERATE;
           m_human->setKeyMap(m_keyMap);
        }
        
        break;
    }
}
