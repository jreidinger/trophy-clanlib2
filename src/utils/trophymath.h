#ifndef TROPHYMATH_H
#define TROPHYMATH_H

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
namespace TrophyMath 
{
        bool   compFloat( const float f1, const float f2, const float tol=0.001 );
        float  corrAngle( const float ang );
        float  getAngle( const float x1, const float y1, const float x2, const float y2 );
        float  getDistance( const float x1, const float y1, const float x2, const float y2 );
        float  getAngleDiff( const float a1, float a2 );
        bool   getIntersection( const int ax1, const int ay1, const int ax2, const int ay2,
                const int bx1, const int by1, const int bx2, const int by2,
                int* ix, int* iy );
        int    getRandomNumber( const int min, const int max );
        void   timeToString( std::string& timeStr, const int time );
};

#endif

// EOF
