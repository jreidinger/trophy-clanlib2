#ifndef CAPOSITIONTABLE_H
#define CAPOSITIONTABLE_H

#include "player.h"


enum RaceLevel
{
	Easy =0,
	Medium = 1,
	Hard = 2
};

/** Controls the position table data.
    @author Andrew Mustun
    @author Matthieu Lecesne
*/
class CAPositionTable {
public:
    static CAPositionTable* getPositionTable();
    ~CAPositionTable();

    void resetRace();
    void playerFinishedRace( Player* player );
    void playerDied( Player* player );
    void setRaceLevel(const RaceLevel rl) {m_raceLevel = rl;}

private:
    CAPositionTable();

private:
    //! The single instance of this class.

    static CAPositionTable* theOneAndOnlyInstance;

    /** List of players which have
        successfully finished the current race.
        raceRankList[0] is the first.
    */
    Player* raceRankList[CA_RACEMAXPLAYERS];
    //! Index of next rank.
    int raceRankIndex;
    //! Index of next rank from bottom
    int raceRankIndexBottom;
    // Difficulty of actual race
    RaceLevel m_raceLevel;

};

#endif
