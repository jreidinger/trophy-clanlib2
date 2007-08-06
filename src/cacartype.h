#ifndef CACARTYPE_H
#define CACARTYPE_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>

/** Structure for car types.
    @author Andrew Mustun
*/
struct CACarType {
    /** Constructor.
    */
    CACarType() {
        surface = 0;
        surface3d = 0;
    }

    /** Destructor.
    */
    ~CACarType() {
        if( surface ) {
            delete surface;
            surface = 0;
        }
        if( surface3d ) {
            delete surface3d;
            surface3d = 0;
        }
    }

    //! Car name (e.g. "Capri")
    std::string   name;

    //! Pointer to surface resource (r,g,b,m,y,c)
    CL_Surface* surface;
    //! Pointer to surface resource 3d (r,g,b,m,y,c)
    CL_Surface* surface3d;

    //! Current car length in pixel
    int length;
    //! Current car width in pixel
    int width;
    //! Half of the diagonal
    float radius;
    //! Angle to edge
    float angle;

    //! Maximum speed in pixels per second
    float maxSpeed;
    //! Minimum speed in pixels per second
    float minSpeed;
    //! Maximum turbo load in pixels
    float maxTurbo;
    //! Acceleration in pixels per square second
    float acceleration;
    //! Deceleration in pixels per square second
    float deceleration;
    //! Steering power in degrees per second
    float steeringPower;
    //! Sliding factor (1=no, 0.8=much, ...)
    float slidingFactor;

    //! Price in USD
    int   price;
};

#endif

// EOF
