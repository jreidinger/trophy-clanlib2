#ifndef CAPOSITIONTABLE_H
#define CAPOSITIONTABLE_H

#include "caplayer.h"

#define CA_POSITIONTABLE CAPositionTable::getPositionTable()

/** Controls the position table data.
    @author Andrew Mustun
*/
class CAPositionTable {
public:
    static CAPositionTable* getPositionTable();
    ~CAPositionTable();

    void resetRace();
    void playerFinishedRace( CAPlayer* player );
    void playerDied( CAPlayer* player );

private:
    CAPositionTable();

private:
    //! The single instance of this class.

    static CAPositionTable* theOneAndOnlyInstance;

    /** List of players which have
        successfully finished the current race.
        raceRankList[0] is the first.
    */
    CAPlayer* raceRankList[CA_MAXPLAYERS];
    //! Index of next rank.
    int raceRankIndex;
    //! Index of next rank from bottom
    int raceRankIndexBottom;

};

#endif
