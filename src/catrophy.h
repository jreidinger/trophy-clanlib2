#ifndef CATROPHY_H
#define CATROPHY_H

#include <ClanLib/core.h>

#ifdef USE_SDL
    #include <ClanLib/sdl.h>
#else
    #include <ClanLib/gl.h>
#endif

#include <ClanLib/application.h>
#include "cartype.h"
#include "cadustclowd.h"
#include "cafogbomb.h"
#include "cagoody.h"
#include "cagoodytype.h"
#include "caloadingscreen.h"
#include "utils/trophymath.h"
#include "camenu.h"
// #include "canetclient.h"
// #include "canetserver.h"
#include "cacarupgrades.h"
#include "caresources.h"
#include "track.h"
#include "cascreen.h"
#include <vector>
#include <string>


class CATrophy;
class Player;
class CAPanel;
class CAConfigureKey;

#define CA_APP theApp()            // Macro for getting a pointer to the application (CATrophy*)

CATrophy* theApp();


/** Main class of Trophy. There's only one global instance:
    'CATrophy application'.
    It's usually accessed over the macro CA_APP. E.g.: CA_APP->buildScreen();
    This class starts the Game, inits the moduls and displays things.
    @author Andrew Mustun
    @author Matthieu Lecesne
*/
class CATrophy : public CAScreen {
public:
    enum Difficulty { Easy = 0, Medium, Hard };

    void start( const std::vector<CL_String> &args );

    // Configuration methods:
    //
    virtual const std::string get_title();

    void initCarTypes();
    void deinitCarTypes();
    void initUpgrades();
    void deinitUpgrades();
    void initGoodies();
    void deinitGoodies();
    void initPlayers();
    void deinitPlayers();
    void initNetwork();
    void deinitNetwork();
    void initPanel();
    void deinitPanel();
    void initTrackList();
    void initRace( const std::string& trackName );

    void reconfigure();
    void resetGoodies();
    void resetFogBombs();
    void resetDustClowds();
    void resetPlayers();

    // Control / actions:
    //
    void runMenu();
    bool runPositionTable();
    void runCreditsScreen();
    void runServerInfo( bool success );
    void runClientInfo( bool success );
    void startNewGame();
    void gameLoop();
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
    std::string getTimeString() {
        return timeString;
    }
    void dropFogBomb( int x, int y, bool up );
    void makeDustClowd( int x, int y, bool up );
    void measureFrameTime( bool start );
    void waitForSilence();
    //void buyCars();

    // Display / view methods:
    //
    void buildScreen();
    void displayPlayers( bool up );
    void displayGoodies( bool up );
    void displayBridge();
    void displayFogBombs( bool up, bool bomb );
    void displayDustClowds( bool up );
    void displayCheckFlag();
    void displayStartingLights();
    void fadeScreen( bool in, CAScreen* screen, bool whole=true );
    void scroll();

    // Persistance:
    //
    bool saveGame();
    bool loadGame();

public:
    //! Loading screen
    CALoadingScreen loading;

    //! Game paused?
    bool pause;
    //! Sound on/off
    bool sound;
    //! Sound volume
    int  volume;
    //! Keyboard
    CL_InputDevice keyboard;
    //! Fullscreen mode?
    bool fullScreen;
    // ! big Money Mode
    bool m_cheatMoney;
    //! Fast mode (no screen fading) (command line argument --fast)
    bool fast;
    //! Are we a server (command line argument --server)
    bool server;
    //! Are we a client (command line argument --client)
    bool client;
    //! Debug mode (command line argument --debug)
    bool debug;
    //! Show trackinfo (crosses and numbers on track points) (command line argument --trackinfo)
    bool trackInfo; // TODO: should be in track

    //! Frames per second we can do at maximum
    float framesPerSec;

    //! Array of Players. Created and destroyed by this class.
    std::vector<Player*> player;
    //! Array of Players who are running in the same Race of the player
    std::vector<Player*> m_RacePlayer;
    //! Array of car types.
    std::vector<CarType> carType;
    //! Array of goodyTypes.
     std::vector<CAGoodyType*> goodyType;
    //! The goody instances. CA_NUMGOODIES of each type.
    std::vector<std::vector<CAGoody*> > goody;

    //! Chosen difficulty (easy, medium, hard)
    Difficulty   difficulty;

    // Track:
    //! Current track data
    Track* m_track;
    std::string m_trackName;
    //! Track directory names list
    std::vector<std::string> trackList;
    //! List of three race prices
    //StringList priceList;
    //! Offset of the background map in x. 
    int offsetX; // TODO : check if we could put in track
    //! Offset of the background map in y.
    int offsetY; // TODO : check if we could put in track

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

    //! resolution : 800x600, 640x480 or 1024x768
    std::string resolution;

    //! Ip set in the net options dialog or "auto"
    char serverIp[16];
    //! Server port number
    int  serverPort;

    //! Input context
    CL_InputContext *input_context;

    //! Sound output
    CL_SoundOutput *sound_output;

    //! Upgrades ressources manager
    CACarUpgrades *carUp;
    //! new clanlib graphical context
    CL_GraphicContext* graphicContext;
    //! display window, useful e.g. for flipping
    CL_DisplayWindow *display_window;
private:
    //! Current time in milliseconds since race start
    int  time;
    //! Current race time as string (e.g. "02:23")
    std::string timeString;


    CAConfigureKey* m_ConfigureKey;

    std::string m_homedir;

    bool m_isGameStarted;

    std::vector<int> m_currentTrackNumbers;

    int m_nbTurns;

    int m_gameLoopState;

    //! The server object
    // CANetServer* netServer;
    //! The client object
    // CANetClient* netClient;
};

#endif

// EOF
