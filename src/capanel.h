#ifndef CAPANEL_H
#define CAPANEL_H

#include <ClanLib/core.h>

#include "caplayerview.h"
#include <vector>

/** Represents the panel on the left of the game.
    @author Andrew Mustun
    @author Matthieu Lecesne
*/
class CAPanel
{
    public:
        CAPanel();
        ~CAPanel();

        void display();

    private:
        //! Pointer array of player views. One shown for each player.
        std::vector<CAPlayerView*> m_playerView;
};

#endif

// EOF
