#include "track.h"
#include "catrophy.h"
#include "utils/trophymath.h"
#include "caresources.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <fstream>
#include <sstream>

Track::Track(const std::string& trackName, const bool debug):
    m_functionMap(),
    m_visualMap(),
    m_bridge(),
    m_blocked(false),
    m_rp_number(0)
{
   // Base path of track files:
    std::string trackPath = std::string("tracks/") + trackName + "/";

    // Load visual map:
    //
    std::string vmapPath = trackPath + "vmap.tga";
    try
    {
        m_visualMap = CL_Image( *CA_APP->graphicContext,vmapPath);
    }
    catch(CL_Exception err)
    {
        trackPath = std::string("../resources/tracks/") + trackName + "/";
        vmapPath = trackPath + "vmap.tga";
        m_visualMap = CL_Image( *CA_APP->graphicContext,vmapPath);
    }

   // loading.setProgress( 15 ); TODO

    // Load functional map:
    //
    std::string fmapPath = trackPath + "fmap.tga";
    // We need to load a surface and then create the pixelbuffer if we don't want to loose the information
    // about the color. Don't know if it is a bug of ClanLib or the normal behavior.
    m_functionMap = CL_PixelBuffer(fmapPath);
    // TODO : is it OK if we never unlock it ? functionMap is not meant to be drawn
    m_functionMap.lock(cl_access_read_write);

   // loading.setProgress( 30 ); TODO


    // Read config file for this track:
    //
    std::string configFilePath = trackPath + "config.tck";

    if(debug) printf( "%s\n", configFilePath.c_str() );

    std::ifstream tckFile (configFilePath.c_str());
    if ( tckFile )
    {
        std::string line;                 // The line converted to a string
        std::string name;                 // Name of the value (Author)
        std::string value;                // Value (Andrew Mustun)
        unsigned int xv=0, yv=0, zv=0;           // Coordinates for following objects
        unsigned int iv=0;                       // Index for following object (route point)
        while (std::getline(tckFile, line))
        {
            if (line.size()!=0)
            if (line[0]!='#')
            {
                int i = std::string( line ).find_first_of( " =" );
                name = line.substr( 0, i );
                i = std::string( line ).find ('"', i) + 1;
                int j = std::string( line ).find ('"', i);
                value = line.substr( i, j-i );

                std::istringstream iss(value);
                // General track info:
                //
                if( name=="Author" )         iss >> m_author;
                else if( name=="Version")    iss >> m_version;
                else if( name=="Name"   )    iss >> m_name;

                // Coordinates of next object / route point:
                //
                else if( name=="x" )         iss >> xv;
                else if( name=="y" )         iss >> yv;
                else if( name=="z" )         iss >> zv;
                else if( name=="i" )         iss >> iv;

                // Start angle:
                //
                else if( name=="startAngle" ) iss >> m_startAngle;

                // Route points:
                //
                else if( name=="RP" )
                {
                    int pi;
                    iss >> pi; // TODO : sort RP
                    while (m_rp.size() <= iv)
                    {
                        m_rp.push_back(std::vector<RP>());
                    }

                    RP rp(xv, yv);
                    m_rp[iv].push_back(rp);
                }

                // Objects:
                //
                else if( name=="Object" && value=="bridge") {
                    std::string bridgePath = trackPath + value + ".tga";
                    m_bridge = CL_Image( *CA_APP->graphicContext,bridgePath);
                    m_bridgePos[0] = xv;
                    m_bridgePos[1] = yv;
                }
            }
        }
        tckFile.close();
    }
}

void Track::handleTrackCreation(const int offsetX, const int offsetY)
{
    CL_InputDevice& mouse = CA_APP->display_window->get_ic().get_mouse();
    if( mouse.get_keycode(CL_MOUSE_LEFT) ) 
    {
        if( !m_blocked ) 
        {
            m_blocked = true;
            FILE* fp = fopen( "trackdata.txt", "at" );
            if( fp ) 
            {
                fprintf( fp, "x = \"%d\"\n", mouse.get_x()-offsetX );
                fprintf( fp, "y = \"%d\"\n", mouse.get_y()-offsetY );
                fprintf( fp, "RP = \"%d\"\n\n", m_rp_number++ );
                fclose( fp );
                /*
                 * TODO
                CL_Color fmap_pix = m_functionMap.get_pixel(mouse.get_x()-offsetX, mouse.get_y()-offsetY);

                CL_PixelBuffer pixbuf = m_visualMap.get_pixeldata();
                pixbuf.lock();
                CL_Color vmap_pix = pixbuf.get_pixel(mouse.get_x()-offsetX, mouse.get_y()-offsetY);
                pixbuf.unlock();
                std::cout << "vmap (r, g, b, a) = (" << vmap_pix.get_red() << ", " << vmap_pix.get_green() << ", " << vmap_pix.get_blue() << ", " << vmap_pix.get_alpha() << ")" << std::endl;
                std::cout << "fmap (r, g, b, a) = (" << fmap_pix.get_red() << ", " << fmap_pix.get_green() << ", " << fmap_pix.get_blue() << ", " << fmap_pix.get_alpha() << ")" << std::endl;
                */
            }
        }
    } 
    else 
    {
        m_blocked = false;
    }
}


/** Checks if the given coordinate is on the map and returns true if so.
*/
bool Track::checkCoordinate( const int x, const int y ) const
{
    return ( x>=0 && y>=0 && x<(int)m_visualMap.get_width() && y<(int)m_visualMap.get_height() );
}

/** Displays the race map.
*/
void Track::displayMap(const int offsetX, const int offsetY) const
{
    m_visualMap.draw ( *CA_APP->graphicContext,offsetX,offsetY);
}


void Track::scroll(int& offsetX, int& offsetY, const int posX, const int posY, const int width, const int height, const int panelWidth)
{
    const int maxOffsetX = panelWidth;
    const int minOffsetX = -(m_visualMap.get_width()-(width-panelWidth)) + panelWidth;
    const int maxOffsetY = 0;
    const int minOffsetY = -(m_visualMap.get_height()-height);

    offsetX = - ((int)(posX) - (width-panelWidth)/2) + panelWidth;
    offsetY = - ((int)(posY) - height/2);
    if( offsetX>maxOffsetX ) offsetX = maxOffsetX;
    if( offsetX<minOffsetX ) offsetX = minOffsetX;
    if( offsetY>maxOffsetY ) offsetY = maxOffsetY;
    if( offsetY<minOffsetY ) offsetY = minOffsetY;
}

/** Gets the speed limit for a coordinate of the map
*/
int Track::getSpeedLimit( const int x, const int y )
{
    if( checkCoordinate( x,y ) ) 
    {
        CL_Color tmp = m_functionMap.get_pixel( x,y );
        unsigned int g = tmp.get_green();
        return ((g)&0xF0)>>4;
    }

    return 0;
}

/** Gets the lap part for a coordinate of the map
*/
int Track::getLapPart( const int x, const int y )
{
    if( checkCoordinate( x,y ) ) 
    {
        CL_Color tmp = m_functionMap.get_pixel( x,y );
        unsigned int b = tmp.get_blue();
        return ((b)&0x1F);
    }

    return 0;
}

/** Gets the level for a coordinate of the map.
    \return 0: Level on the point is down
            1: Level on the point is up
            2: Level on the point is not defined
*/
int Track::getLevel( const int x, const int y )
{
    if( checkCoordinate( x,y ) ) 
    {
        CL_Color tmp = m_functionMap.get_pixel( x,y );
        unsigned int g = tmp.get_green();
        if( (g&0x02)!=0 ) return 1;
        if( (g&0x01)!=0 ) return 0;
    }

    return 2;
}

/** Displays track points.
*/
void Track::displayTrackPoints(const int& offsetX, const int& offsetY) const
{
    char str[16];
    for( unsigned int t=0; t<m_rp.size(); ++t )
    {
        for( unsigned int r=0; r<m_rp[t].size(); ++r )
        {
            sprintf( str, "%d/%d", r, t );
            CA_RES->misc_cross.draw ( *CA_APP->graphicContext,m_rp[t][r].getX()+offsetX-8, m_rp[t][r].getY()+offsetY-8);
            CA_RES->font_normal_11_white.draw_text( *CA_APP->graphicContext, m_rp[t][r].getX()+offsetX, m_rp[t][r].getY()+offsetY+5, str );
        }
    }
}

/** Displays the bridge if there is any.
*/
void Track::displayBridge(const int& offsetX, const int& offsetY) const
{
    if( !m_bridge.is_null() )
    {
        m_bridge.draw ( *CA_APP->graphicContext,m_bridgePos[0]+offsetX, m_bridgePos[1]+offsetY);
    }
}


void Track::getNextRoutePoint(unsigned int& routeNumber, unsigned int& routePoint, float& nx, float& ny) const
{
    // Choose new route by shuffle:
    //
    if( routePoint >= m_rp[routeNumber].size() ) 
    {
        routePoint=0;
        routeNumber = TrophyMath::getRandomNumber( 0, m_rp.size()-1 );
    }

    // Next coordinate to locate
    //
    nx = m_rp[routeNumber][routePoint].getX();
    ny = m_rp[routeNumber][routePoint].getY();
}


CL_Color Track::getFunctionalPixel(int x, int y)
{
    return m_functionMap.get_pixel(x, y);
}

int Track::getWidth() const
{
     return m_visualMap.get_width();
}

int Track::getHeight() const
{
     return m_visualMap.get_height();
}
