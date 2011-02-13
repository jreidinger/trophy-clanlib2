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
	void playerFinishedLapped(Player* player);
    void setRaceLevel(const RaceLevel rl) {m_raceLevel = rl;}
	bool isPossibleWin() const;

private:
    CAPositionTable();

private:
    //! The single instance of this class.

    static CAPositionTable* theOneAndOnlyInstance;

    /** List of players which have
        successfully finished the current race.
        raceRankList[0] is the first.
    */
    std::vector<Player*> m_raceRankList;
    std::vector<Player*> m_diedPlayers;
    std::vector<Player*> m_lappedPlayers;
    // Difficulty of actual race
    RaceLevel m_raceLevel;

};

#endif
