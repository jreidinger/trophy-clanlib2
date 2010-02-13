#ifndef TRACK_H
#define TRACK_H


#include "utils/trophymath.h"
#include <string>
#include <ClanLib/core.h>
#include <ClanLib/application.h>
#ifdef USE_SDL
    #include <ClanLib/sdl.h>
#else
    #include <ClanLib/gl.h>
#endif

/** Structure for the current track.
    @author Andrew Mustun
    @author Matthieu Lecesne
*/
class Track 
{
    
public:
    Track(const std::string& trackName, const bool debug = false);
 
    ~Track();

    bool checkCoordinate( const int x, const int y ) const;

    void handleTrackCreation(const int offsetX, const int offsetY);

    void draw();

    void scroll(int& offsetX, int& offsetY, const int posX, const int posY, const int width, const int height, const int panelWidth);

    void displayMap(const int offsetX, const int offsetY) const;

    int getSpeedLimit( const int x, const int y ) const;
    
    int getLapPart( const int x, const int y ) const;

    int getLevel( const int x, const int y ) const;

    void displayTrackPoints(const int& offsetX, const int& offsetY) const;

    void displayBridge(const int& offsetX, const int& offsetY) const;

    void getNextRoutePoint(int& routeNumber, int& routePoint, float& nx, float& ny) const;

    int getStartAngle() const {return m_startAngle;}

    CL_Color getFunctionalPixel(int x, int y) const;

    int getWidth() const;

    int getHeight() const;
 private:   
    //! Track number
    int m_number;
    //! Track name
    std::string m_name;
    //! Version information (e.g. "0.0.1")
    std::string m_version;
    //! Author of this track
    std::string m_author;
    //! Initial angle of players (0=right, 90=bottom (!))
    int m_startAngle;
    //! Route points (Usually 1 route per player with x/y) // TODO: use vector
    int m_rp[CA_MAXPLAYERS]
    [CA_MAXROUTEPOINTS]
    [2];
    //! Number of route points in this track
    int m_nbRoutePoints; // TODO: use vector
    //! Pointer to function map
    // TODO : check that PixelBuffer is really what we want. It could (and I think it does) introduce bugs
    // in AI because PixelBuffer height and width are power of 2
    CL_PixelBuffer*  m_functionMap;
    //! Pointer to visual map
    CL_Surface* m_visualMap;
    //! Pointer to bridge surface or NULL if there's no bridge
    CL_Surface* m_bridge;
    //! Bridge position
    int m_bridgePos[2];

    int m_blocked;
    //! Routing Point (use only for track creation)
    int m_rp_number;
};

#endif

// EOF
