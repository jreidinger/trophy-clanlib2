#ifndef CAMATH_H
#define CAMATH_H

#include <math.h>
#include <stdlib.h>
#include <ClanLib/core.h>

#define ARAD 57.29577951308232

#define MAX(A,B) (A>=B ? A : B)
#define MIN(A,B) (A<=B ? A : B)

// Version dependent settings:

#define CA_MAXTRACKS      64      // Max number of tracks
#define CA_NUMCARTYPES     4      // Max number of car types

#define CA_WIDTH         800      // Width of screen
#define CA_HEIGHT        600      // Height      "

#define CA_HELPHEIGHT     50      // Height of help-frame at the bottom

#define CA_MAXMENUITEMS   10      // Max number of menu entries
#define CA_MENUSPACE      12      // Space between menu texts
#define CA_MENUWIDTH     360      // Menu width

#define CA_MAXFOGBOMBS    10      // Max number of fog bombs
#define CA_MAXDUSTCLOWDS  50      // Max number of dust clowds
#define CA_NUMGOODYTYPES   5      // Number of different goodie items
#define CA_NUMGOODIES      6      // Number of goodies of each type (max)
#define CA_GOODYTIME     250      // Pace a goody all ... millisecs

#define CA_MAXPLAYERS      6      // Max number of players in total

#define CA_TURBOFACTOR     1.5    // Speed factor for turbo
#define CA_SHOOTINGRANGE 200      // Shooting range in pixel
#define CA_MAXHITPOINTS    5      // Max number of hit points for one screen and one player

#define CA_MAXROUTEPOINTS 32      // Max number of route points for computer players

#define CA_NUMLAPS         5      // Number of laps to do for each race

#define CA_NUMSLOTS       10      // Number of slots for saving games.

#define CA_FPR            72      // Number of frames used for one rotation of the sprite

#define CA_PRIZE        3000      // Money for first player

/** Mathematical methods for generic use.
    @author Andrew Mustun
*/
class CAMath 
{
    public:
        static bool   compFloat( float f1, float f2, float tol=0.001 );
        static float  corrAngle( float ang );
        static float  getAngle( float x1, float y1, float x2, float y2 );
        static float  getDistance( float x1, float y1, float x2, float y2 );
        static float  getAngleDiff( float a1, float a2 );
        static bool   getIntersection( int ax1, int ay1, int ax2, int ay2,
                int bx1, int by1, int bx2, int by2,
                int* ix, int* iy );
        static int    getRandomNumber( int min, int max );
        static void   timeToString( char* string, int time );
};

#endif

// EOF
