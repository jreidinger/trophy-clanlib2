#ifndef HSVCOLOR_H
#define HSVCOLOR_H

#include <ClanLib/core.h>

/** Structure for a color.
  Colors in trophy are usually used for 
  the modification of red based surfaces.
  @author Andrew Mustun
  */
class HSVColor 
{
    public:
    /** Constructor for a black color.
    */
    HSVColor() 
    {
        h = s = v = 0;
    }

    /** Constructor for any desired color.
    */
    HSVColor( int h, int s, int v ) 
    {
        this->h = h;
        this->s = s;
        this->v = v;
    }

    /** Operator !=
    */
    inline bool operator!=( const HSVColor& c ) 
    {
        return (this->h!=c.h || this->s!=c.s || this->v!=c.v);
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
