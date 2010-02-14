#ifndef CA_FOGBOMB
#define CA_FOGBOMB

#include <ClanLib/core.h>

/** Structure for fog bombs.
    @author Andrew Mustun
*/
struct CAFogBomb {
    //! Is this bomb active (visible)
    bool active;
    //! Has this bomb already made boom?
    bool exploded;
    //! Current frame
    float frame;
    //! X Coordinate
    int x;
    //! Y Coordinate
    int y;
    //! Is it on (true) or below (false) bridges?
    bool up;
};

#endif

// EOF
