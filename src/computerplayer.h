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
    void resetForRace(int routeNumber, Track* currentTrack);
    void display( int offsetX, int offsetY );
    void OnRaceOver();
private:
    float getCheckAwayAngle();
    //! Index of next route point to drive to
    int    m_routePoint;
    //! routeNumber to know which pre-determined path to follow
    int    m_routeNumber;

    //! x coordinate of next route points (only for --trackinfo option)
    float nxTemp;
    //! y coordinate of next route points (only for --trackinfo option)
    float nyTemp;
};

#endif

// EOF
