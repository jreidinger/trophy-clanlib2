#ifndef HUMAN_PLAYER_H
#define HUMAN_PLAYER_H

#include "caconfigurekey.h"
#include "player.h"
#include <map>

/** Covers specificity for a human player
   @author Andrew Mustun
   @author Matthieu Lecesne
*/
class HumanPlayer : public Player
{
public:
    HumanPlayer( int id, const std::string& name, int carNumber );
    ~HumanPlayer();

    void pilot();

    void setKeyMap(const std::map<ConfigureKey::DefineKey, int>& keyMap)
    {
       m_keyMap = keyMap;
    }

private:
    float getCheckAwayAngle();
    //! key control
    std::map<ConfigureKey::DefineKey, int> m_keyMap;
};

#endif

// EOF
