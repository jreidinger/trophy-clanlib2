#ifndef CAPLAYERVIEW_H
#define CAPLAYERVIEW_H

#include "player.h"
#include "utils/hsvcolor.h"

/** A player view in the panel.
    @author Andrew Mustun
*/
class CAPlayerView {
public:
    CAPlayerView( Player* player );
    ~CAPlayerView();

    void display( const int defaultPos );
    void renderButton();

private:
    //! Pointer to player.

    Player* player;
    //! The button in players color. Created and destroyed by this class.
    CL_Image button;
    /** Current color of this button. Needed for detecting if we must
        re-render the button.
    */
    HSVColor currentColor;
};

#endif
