#ifndef CACONFIGUREKEY_H
#define CACONFIGUREKEY_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "cadialog.h"
#include <map>
#include <string>

class HumanPlayer;

namespace ConfigureKey
{
    enum DefineKey
    {
        ACCELERATE = 0,
        BRAKE,
        LEFT,
        RIGHT,
        SHOOT,
        BOMB,
        BOOST,
        HORN
    };
}

/** Credits screen.
    @author Matthieu Lecesne
*/
class CAConfigureKey : public CADialog  {
public:
    CAConfigureKey(HumanPlayer* human);
    ~CAConfigureKey();

    virtual void buildScreen();
    virtual void on_key_released( const CL_InputEvent &key, CL_InputState &state );

    /*std::map<ConfigureKey::DefineKey, int> getKeyMap()
    { return m_keyMap; }*/

private:
    std::map<ConfigureKey::DefineKey, std::string> m_keyString;
    std::map<ConfigureKey::DefineKey, int> m_keyMap;
    HumanPlayer* m_human;
    ConfigureKey::DefineKey m_keyMode;
};

#endif
