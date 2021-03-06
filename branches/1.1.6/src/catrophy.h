#ifndef CATROPHY_H
#define CATROPHY_H

#include <ClanLib/core.h>

#ifdef USE_SDL
    #include <ClanLib/sdl.h>
#else
    #include <ClanLib/gl.h>
#endif

#include <ClanLib/application.h>
#include "cacartype.h"
#include "cadustclowd.h"
#include "cafogbomb.h"
#include "cagoody.h"
#include "cagoodytype.h"
#include "caloadingscreen.h"
#include "camath.h"
#include "camenu.h"
// #include "canetclient.h"
// #include "canetserver.h"
#include "caresources.h"
#include "catrack.h"
#include "cascreen.h"
#include "castringlist.h"

class CATrophy;
class CAPlayer;
class CAPanel;

#define CA_APP theApp()            // Macro for getting a pointer to the application (CATrophy*)

CATrophy* theApp();


/** Main class of Trophy. There's only one global instance:
    'CATrophy application'.
    It's usually accessed over the macro CA_APP. E.g.: CA_APP->buildScreen();
    This class starts the Game, inits the moduls and displays things.
    @author Andrew Mustun
*/
class CATrophy : public CL_ClanApplication,
    public CAScreen {
public:
    enum Difficulty { Easy, Medium, Hard };

    virtual int main( int argc, char** argv );

    // Configuration methods:
    //
    virtual const char *get_title();

    void initCarTypes();
    void deinitCarTypes();
    void initGoodies();
    void deinitGoodies();
    void initPlayers();
    void deinitPlayers();
    void initNetwork();
    void deinitNetwork();
    void initPanel();
    void deinitPanel();
    void initTrackList();
    void initTrack( std::string trackName );
    void deinitTrack();

    void reconfigure();
    void resetGoodies();
    void resetFogBombs();
    void resetDustClowds();
    void resetPlayers();

    // Control / actions:
    //
    void runMenu();
    bool runPositionTable( bool race );
    void runCreditsScreen();
    void runServerInfo( bool success );
    void runClientInfo( bool success );
    void startNewGame();
    void startServer();

    void chooseNetGame();
    int  addNetPlayer();
    void controlNetPlayer( int id, int xpos, int ypos, int dir );

    int  run();

    void placeGoody();
    void setRanks();
    int  getSpeedLimit( int x, int y );
    int  getLapPart( int x, int y );
    int  getLevel( int x, int y );
    bool checkCoordinate( int x, int y );
    //! Returns race time in msec.
    int  getTime() {
        return time;
    }
    //! Race time as string.
    char* getTimeString() {
        return timeString;
    }
    void dropFogBomb( int x, int y, bool up );
    void makeDustClowd( int x, int y, bool up );
    void measureFrameTime( bool start );
    void waitForSilence();
    void buyCars();

    // Display / view methods:
    //
    void buildScreen();
    void displayMap();
    void displayPlayers( bool up );
    void displayGoodies( bool up );
    void displayBridge();
    void displayFogBombs( bool up, bool bomb );
    void displayDustClowds( bool up );
    void displayCheckFlag();
    void displayStartingLights();
    void displayTrackPoints();
    void fadeScreen( bool in, CAScreen* screen, bool whole=true );
    void scroll();

    // Persistance:
    //
    void saveGame();
    void loadGame();

public:
    //! Loading screen

    CALoadingScreen loading;

    //! Game paused?
    bool pause;
    //! Sound on/off
    bool sound;
    //! Sound volume
    int  volume;
    //! Fullscreen mode?
    bool fullScreen;
    //! Fast mode (no screen fading) (command line argument --fast)
    bool fast;
    //! Are we a server (command line argument --server)
    bool server;
    //! Are we a client (command line argument --client)
    bool client;
    //! Debug mode (command line argument --debug)
    bool debug;
    //! Show trackinfo (crosses and numbers on track points) (command line argument --trackinfo)
    bool trackInfo;

    //! Frames per second we can do at maximum
    float framesPerSec;

    //! Array of Players. Created and destroed by this class.
    CAPlayer* player[CA_MAXPLAYERS];
    //! Array of car types.
    CACarType carType[CA_NUMCARTYPES];
    //! Array of goodyTypes.
    CAGoodyType goodyType[CA_NUMGOODYTYPES];
    //! The goody instances. CA_NUMGOODIES of each type.
    CAGoody* goody[CA_NUMGOODYTYPES][CA_NUMGOODIES];

    //! Chosen difficulty (easy, medium, hard)
    Difficulty   difficulty;

    // Track:
    //! Current track data
    CATrack track;
    //! Track directory names list
    CAStringList trackList;
    //! List of three race prices
    //CAStringList priceList;
    //! Offset of the background map in x.
    int offsetX;
    //! Offset of the background map in y.
    int offsetY;

    //! Panel. Created and destroyed in this class.
    CAPanel* panel;
    //! Width of panel (can change)
    int panelWidth;

    //! Height of header in pixels
    int headerHeight;

    /** State of the start lights
        0: off  1: 1/3 on  2: 2/3 on  3: on  4: sliding away
    */
    int lightState;

    //! first player has finished race
    bool firstPlayerFinished;

    //! In the beginning of each race, shooting is disabled
    bool allowShooting;

    //! Array of fog bombs
    CAFogBomb fogBomb[CA_MAXFOGBOMBS];
    //! current index for the fogBomb array.
    int  fogBombCounter;

    //! Array of dust clowds
    CADustClowd dustClowd[CA_MAXDUSTCLOWDS];
    //! current index for the dust Clowd array.
    int  dustClowdCounter;

    //! Total width (screen resolution in x)
    int width;
    //! Total height (screen resolution in y)
    int height;

    //! Ip set in the net options dialog or "auto"
    char serverIp[16];
    //! Server port number
    int  serverPort;

    //! Input context
    CL_InputContext *input_context;

private:
    //! Current time in milliseconds since race start

    int  time;
    //! Current race time as string (e.g. "02:23")
    char timeString[16];

    CL_DisplayWindow *display_window;

    //! The server object
    // CANetServer* netServer;
    //! The client object
    // CANetClient* netClient;
};

#endif

// EOF
