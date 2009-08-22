#ifndef CARTYPE_H
#define CARTYPE_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "utils/trophymath.h"

/** Structure for car types.
    @author Andrew Mustun
*/
struct CarType 
{
    /** Constructor.
    */
    CarType() 
    {
        surface = 0;
        surface3d = 0;
    }

    CarType(const std::string& mainPath, CL_ResourceManager* resources, bool debug = false)
    {
        if(debug) std::cout << "  name" << std::endl;

        std::string path = mainPath + "name";
        name = CL_String::load( path, resources );

        if(debug) std::cout << "  surface" << std::endl;

        path = mainPath + "surface";
        surface = new CL_Surface( path, resources );

        if(debug) std::cout << "  surface3d" << std::endl;

        path = mainPath + "surface3d";
        surface3d = new CL_Surface( path, resources );

        path = mainPath + "length";
        length = CL_Integer( path, resources );
        path = mainPath + "width";
        width = CL_Integer( path, resources );
        path = mainPath + "maxSpeed";
        maxSpeed = 6 * CL_Integer( path, resources );
        path = mainPath + "minSpeed";
        minSpeed = 6 * CL_Integer( path, resources );
        path = mainPath + "maxTurbo";
        maxTurbo = CL_Integer( path, resources );
        path = mainPath + "acceleration";
        acceleration = 6 * CL_Integer( path, resources );
        path = mainPath + "deceleration";
        deceleration = 6 * CL_Integer( path, resources );
        path = mainPath + "steeringPower";
        steeringPower = CL_Integer( path, resources );
        path = mainPath + "slidingFactor";
        slidingFactor = 0.01 * CL_Integer( path, resources );
        path = mainPath + "price";
        price = CL_Integer( path, resources );

        radius = std::sqrt( (double)width/2 * (double)width/2 + (double)length/2 * (double)length/2 );
        angle = atan( (double)(width/2) / (double)(length/2) ) * ARAD;
    }

    /** Destructor.
    */
/*    ~CarType() 
    {
        if( surface ) 
        {
            delete surface;
            surface = 0;
        }
        if( surface3d ) 
        {
            delete surface3d;
            surface3d = 0;
        }
    }
*/



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
