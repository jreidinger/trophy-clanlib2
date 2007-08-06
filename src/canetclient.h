#ifndef CANETCLIENT_H
#define CANETCLIENT_H

#include <ClanLib/core.h>
#include <ClanLib/network.h>
#include "caplayer.h"

/** Net client for multiplayer games.
    @author Andrew Mustun
*/
class CANetClient {
public:
    CANetClient( CAPlayer* player );
    ~CANetClient();

    bool joinGame();
    void keepUp();

protected:
    //! Pointer to joined net game or NULL.
    //CL_NetGame* netGame;
    //! Pointer to playing player.
    //CAPlayer* player;
}
;

#endif

// EOF
