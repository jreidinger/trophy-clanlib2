#ifndef CANETSERVER_H
#define CANETSERVER_H

#include <ClanLib/core.h>

/** Wraps the Server things. This class is used by the players
    who create new multiplayer games.
    @author Andrew Mustun
*/
class CANetServer {
public:
    CANetServer();
    ~CANetServer();

    bool createGame();
    void keepUp();

protected:
    //! Pointer to net game handled by this server or NULL.
    //CL_NetGame* netGame;
}
;

#endif

// EOF
