#ifndef CAPANEL_H
#define CAPANEL_H

#include <ClanLib/core.h>

#include "caplayerview.h"

/** Represents the panel on the left of the game.
    @author Andrew Mustun
*/
class CAPanel {
public:
    CAPanel();
    ~CAPanel();

    void display();

private:
    //! Pointer array of player views. One shown for each player.

    CAPlayerView* playerView[CA_MAXPLAYERS];
};

#endif

// EOF
