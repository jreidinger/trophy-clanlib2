#include "player.h"
#include "capositiontable.h"
#include "catrophy.h"

CAPositionTable* CAPositionTable::theOneAndOnlyInstance = 0;


/** Returns the one and only instance of position table.
    If it doesn't exist yet, it gets created here.
*/
CAPositionTable*
CAPositionTable::getPositionTable() {
    if( theOneAndOnlyInstance==0 ) {
        theOneAndOnlyInstance = new CAPositionTable();
    }
    return theOneAndOnlyInstance;
}



/** Constructor.
*/
CAPositionTable::CAPositionTable() {
    resetRace();
}



/** Destructor.
*/
CAPositionTable::~CAPositionTable() {}



/** Resets the lap information (rank list).
*/
void
CAPositionTable::resetRace() {
    for( int i=0; i<CA_RACEMAXPLAYERS; ++i ) {
        raceRankList[i] = 0;
    }
    raceRankIndex = 0;
    raceRankIndexBottom = CA_RACEMAXPLAYERS-1;
}



/** Registers a player in the current lap rank list.
    The first player who calls this function is the lap winner.
*/
void
CAPositionTable::playerFinishedRace( Player* player )
{
    if( raceRankIndex<CA_RACEMAXPLAYERS )
    {
        raceRankList[raceRankIndex] = player;
        player->setRaceRank( raceRankIndex+1 );
        int points = ((CA_RACEMAXPLAYERS+1)-player->getRaceRank()) * (m_raceLevel +1);
        if( points>0 )
        {
            player->setRacePoints( points );
        }
        player->addRaceMoney((float)CA_PRIZE/CA_RACEMAXPLAYERS*(CA_RACEMAXPLAYERS-player->getRaceRank()+1));
        raceRankIndex++;
    }
}



/** Registers a player in the current lap rank list if he died.
    The first player who calls this function is the lap looser.
*/
void
CAPositionTable::playerDied( Player* player ) {
    if( raceRankIndexBottom>=0 ) {
        raceRankList[raceRankIndexBottom] = player;
        player->setRaceRank( raceRankIndexBottom+1 );
        raceRankIndexBottom--;
    }
}


// EOF
