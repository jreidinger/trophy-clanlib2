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
     m_raceRankList.clear();
     m_diedPlayers.clear();
     m_lappedPlayers.clear();
}


bool CAPositionTable::isPossibleWin() const
{
    if (m_raceRankList.size()==0)
		return true;
	else
		return false;
}

	
/** Registers a player in the current lap rank list.
    The first player who calls this function is the lap winner.
*/
void
CAPositionTable::playerFinishedRace( Player* player )
{
	m_raceRankList.push_back(player);
	player->setRaceRank( m_raceRankList.size() );
	int points = ((CA_RACEMAXPLAYERS+1)-player->getRaceRank()) * (m_raceLevel +1);
	if( points>0 )
	{
		player->setRacePoints( points );
	}
	player->addRaceMoney((float)CA_PRIZE/CA_RACEMAXPLAYERS*(CA_RACEMAXPLAYERS-player->getRaceRank()+1));
}


/** Registers a player in the current lap rank list if he finish the race lapped
    No point or money for lapped player
*/
void
CAPositionTable::playerFinishedLapped( Player* player )
{
	player->setRaceRank(CA_RACEMAXPLAYERS-m_diedPlayers.size()-m_lappedPlayers.size());
    m_lappedPlayers.push_back(player);
}


/** Registers a player in the current lap rank list if he died.
    The first player who calls this function is the lap looser.
*/
void
CAPositionTable::playerDied( Player* player )
{
    player->setRaceRank(CA_RACEMAXPLAYERS-m_diedPlayers.size());
    m_diedPlayers.push_back(player);

}




// EOF
