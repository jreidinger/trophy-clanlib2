#ifndef TRACK_H
#define TRACK_H

#include <ClanLib/core.h>

/** Structure for the current track.
    @author Andrew Mustun
*/
struct Track 
{
    //! Track number
    int number;
    //! Track directory name (e.g. "Loops")
    std::string file;
    //! Track name
    std::string name;
    //! Version information (e.g. "0.0.1")
    std::string version;
    //! Author of this track
    std::string author;
    //! Initial angle of players (0=right, 90=bottom (!))
    int startAngle;
    //! Route points (Usually 1 route per player with x/y)
    int rp[CA_MAXPLAYERS]
    [CA_MAXROUTEPOINTS]
    [2];
    //! Number of route points in this track
    int routePoints;
    //! Pointer to function map
    // TODO : check that PixelBuffer is really what we want. It could (and I think it does) introduce bugs
    // in AI because PixelBuffer height and width are power of 2
    CL_PixelBuffer*  functionMap;
    //! Pointer to visual map
    CL_Surface* visualMap;
    //! Pointer to bridge surface or NULL if there's no bridge
    CL_Surface* bridge;
    //! Bridge position
    int bridgePos[2];
};

#endif

// EOF
