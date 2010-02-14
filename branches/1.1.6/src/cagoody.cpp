#include "cagoody.h"

CAGoody::CAGoody( CAGoodyType* goodyType ) {
    this->goodyType = goodyType;
    reset();
}



CAGoody::~CAGoody() {}


/** Reset this goody.
*/
void
CAGoody::reset() {
    active = false;
    x = -100;
    y = -100;
    birthday = 0;
    up = false;
}


/** Displays this goody (if active) on it's current position.
*/
void
CAGoody::display( int offsetX, int offsetY ) {
    if( active ) {
        goodyType->surface->draw( x-8 + offsetX, y-8 + offsetY );
    }
}


/** Moves the goody.
*/
void
CAGoody::move( int x, int y, bool up ) {
    this->x = x;
    this->y = y;
    this->up = up;
}


/** Removes obsolete goodies.
*/
void
CAGoody::advance() {
    if( active ) {
        if( birthday==0 ) {
            birthday = CL_System::get_time();
        } else if( birthday + goodyType->life < CL_System::get_time() ) {
            reset();
        }
    }
}

// EOF
