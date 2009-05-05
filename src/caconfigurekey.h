#ifndef CACONFIGUREKEY_H
#define CACONFIGUREKEY_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "cadialog.h"
#include <map>
#include <string>

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
    CAConfigureKey();
    ~CAConfigureKey();

    virtual void buildScreen();
    virtual void on_key_released( const CL_InputEvent &key );

    std::map<ConfigureKey::DefineKey, int> getKeyMap()
    { return m_keyMap; }

private:
    std::map<ConfigureKey::DefineKey, std::string> m_keyString;
    std::map<ConfigureKey::DefineKey, int> m_keyMap;
    ConfigureKey::DefineKey m_keyMode;
   
    
};

#endif
