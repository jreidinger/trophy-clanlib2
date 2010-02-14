#ifndef CADUSTCLOWD_H
#define CADUSTCLOWD_H

/** Structure for dust clowds.
    @author Andrew Mustun
*/
struct CADustClowd {
    //! Is this dust clowd active (visible)
    bool active;
    //! Has this clowd already disapeared?
    bool faded;
    //! Current frame
    float frame;
    //! X Coordinate
    int x;
    //! Y Coordinate
    int y;
    //! On or below bridges?
    bool up;
};

#endif
