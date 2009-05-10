#ifndef COMPUTER_PLAYER_H
#define COMPUTER_PLAYER_H

#include "player.h"

/** Covers specificity for a computer player
   @author Andrew Mustun
   @author Matthieu Lecesne
*/
class ComputerPlayer : public Player
{
public:
    ComputerPlayer(int id, const std::string& name, int carNumber);
    ~ComputerPlayer();
    void pilot();
    void resetForRace();
private:
    float getCheckAwayAngle();
    //! Index of next route point to drive to
    int    routePoint;
};

#endif

// EOF
