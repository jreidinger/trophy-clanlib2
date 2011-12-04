#ifndef TRACK_H
#define TRACK_H


#include "utils/trophymath.h"
#include <string>
#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <memory>
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
    class RP
{
    public:
        RP(const int nx, const int ny) : x(nx), y(ny) {}
        int getX() const { return x;}
        int getY() const { return y;}
    private:
        int x;
        int y;
};
    Track(const std::string& trackName, const bool debug = false);

    bool checkCoordinate( const int x, const int y ) const;

    void handleTrackCreation(const int offsetX, const int offsetY);

    void scroll(int& offsetX, int& offsetY, const int posX, const int posY, const int width, const int height, const int panelWidth);

    void displayMap(const int offsetX, const int offsetY) const;

    int getSpeedLimit( const int x, const int y ); //TODO const
    
    int getLapPart( const int x, const int y ); //TODO const

    int getLevel( const int x, const int y );

    void displayTrackPoints(const int& offsetX, const int& offsetY) const;

    void displayBridge(const int& offsetX, const int& offsetY) const;

    void getNextRoutePoint(unsigned int& routeNumber, unsigned int& routePoint, float& nx, float& ny) const;

    int getStartAngle() const {return m_startAngle;}

    CL_Color getFunctionalPixel(int x, int y);

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
    //! Route points (Usually 1 route per player with x/y)
    std::vector< std::vector<RP> > m_rp;
    //! Pointer to function map
    // TODO : check that PixelBuffer is really what we want. It could (and I think it does) introduce bugs
    // in AI because PixelBuffer height and width are power of 2
    CL_PixelBuffer  m_functionMap;
    //! Pointer to visual map
    CL_Image m_visualMap;
    //! Pointer to bridge surface or NULL if there's no bridge
    CL_Image m_bridge;
    //! Bridge position
    int m_bridgePos[2];

    int m_blocked;
    //! Routing Point (use only for track creation)
    int m_rp_number;
};

#endif

// EOF
