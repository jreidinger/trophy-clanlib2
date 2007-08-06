#ifndef CACOLOR_H
#define CACOLOR_H

#include <ClanLib/core.h>


/** Structure for a color.
    Colors in trophy are usually used for 
    the modification of red based surfaces.
    @author Andrew Mustun
*/
struct CAColor {

    /** Constructor for a black color.
    */
    CAColor() {
        h = s = v = 0;
    }


    /** Constructor for any desired color.
    */
    CAColor( int h, int s, int v ) {
        this->h = h;
        this->s = s;
        this->v = v;
    }


    /** Operator ==
    */
    inline bool operator==( CAColor c ) {
        return (this->h==c.h && this->s==c.s && this->v==c.v);
    }


    /** Operator !=
    */
    inline bool operator!=( CAColor c ) {
        //return (this->h!=c.h || this->s!=c.s || this->v!=c.v);
        return !operator==( c );
    }


    /** Operator =
    */
    inline CAColor operator=( CAColor c ) {
        h = c.h;
        s = c.s;
        v = c.v;
        return *this;
    }

    //! Hue of this color
    int h;
    //! Saturation of this color
    int s;
    //! Value of this color
    int v;
};

#endif

// EOF
