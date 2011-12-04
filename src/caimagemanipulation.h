#ifndef CAIMAGEMANIPULATION_H
#define CAIMAGEMANIPULATION_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#define ARAD 57.29577951308232

typedef unsigned int uint;

#ifdef WIN32
#define rint(a) ((int) (a+0.5f))
#endif


/** Provides some functions fo manipulate images (CL_Surfaces).
    @author Andrew Mustun <andrew@qcad.org>
    @author Benjamin Winkler <bentsch@mail.com>
*/
class CAImageManipulation {
public:
    CAImageManipulation();
    ~CAImageManipulation();

    static CL_Sprite changeHSV( CL_Sprite sprite, int hue, int saturation, int value );

    static void rgbToHsv( int r, int g, int b,
                          int *h, int *s, int *v );
    static void hsvToRgb( int h, int s, int v,
                          int *r, int *g, int *b );

	/*
    static void getExactColor( unsigned char* data,
                               int width, int height,
                               float x, float y,
                               int *r, int *g, int *b, int *a,
                               bool exact=true );
							   */

    //static int getCoordinateX( int width, int pos );
    //static int getCoordinateY( int width, int pos );
    //static int getDataPosition( int width, int x, int y );
    //static float getDistance( float x1, float y1, float x2, float y2 );
};

#endif
