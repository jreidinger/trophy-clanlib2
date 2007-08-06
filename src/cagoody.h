#ifndef CAGOODY_H
#define CAGOODY_H

#include "cagoodytype.h"


/** A goody (life, turbo, ...)
    @author Andrew Mustun
*/
class CAGoody {
public:
    CAGoody( CAGoodyType* goodyType );
    ~CAGoody();

    void reset();
    void display( int offsetX, int offsetY );
    void move( int x, int y, bool ups );
    void advance();

    //! Returns true if this goody is active (= curently in use).
    bool isActive() {
        return active;
    }
    //! Sets the active status of this goody.
    void setActive( bool on ) {
        active=on;
    }

    //! Gets the x position of this goody.
    int  getX() {
        return x;
    }
    //! Gets the y position of this goody.
    int  getY() {
        return y;
    }

    //! Returns wheter this goody is on bridges (true) or below bridges (false).
    bool isUp() {
        return up;
    }

    //! Returns a pointer to the goody type of the current goody.
    CAGoodyType* getType() {
        return goodyType;
    }

private:
    //! Goody type

    CAGoodyType* goodyType;
    //! Is the goody currently active?
    bool active;
    //! X Position
    int  x;
    //! Y Position
    int  y;
    //! Goody level (up/down)
    bool up;
    //! Timestamp of creation
    unsigned int birthday;
};

#endif
