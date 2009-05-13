
#ifdef W32_BCC551
  /* dont include stl.h for Windoof, Borland C++ 5.5.1 */
#else
  //#include <stl.h>
  #include <iostream>
  #include <string>
#endif

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include <math.h>
#include <fstream>
#include <sstream>

#include "cartype.h"
#include "cacredits.h"
#include "caconfigurekey.h"
#include "caimagemanipulation.h"
#include "cainfodialog.h"
#include "capositiontable.h"
#include "cagoody.h"
#include "catrophy.h"
#include "humanplayer.h"
#include "computerplayer.h"
#include "caplayersettingsdialog.h"
#include "capanel.h"
#include "capositiontableview.h"
#include "camenu.h"
#include "casignupscreen.h"
#include "cachampionshipscreen.h"

// Global instance of the application needed by the ClanLib main():
//
CATrophy application;


/** Global function to access the application instance.
*/
CATrophy*
theApp() {
    return &application;
}

/** The main function.
    Called by ClanLib main().
*/
int
CATrophy::main( int argc, char** argv ) 
{
    CL_ConsoleWindow *console=0;
    try 
    {
        // Default paramters:
        //
        debug = false;
        trackInfo = false;
        framesPerSec = 40;
        width = CA_WIDTH;
        height = CA_HEIGHT;
        fullScreen = false;
        fast = false;
        server = false;
        client = false;
        sound=true;
        volume=5;
        strcpy( serverIp, "0.0.0.0" );
        serverPort = 18805;
        difficulty = Medium;
        m_ConfigureKey = NULL;

        // Proccess parameters:
        //
        int c=1;
        while( c<argc ) 
        {
            // General options:
            //
            if( !strcmp(argv[c], "--debug" ) || !strcmp(argv[c], "-d") ) {
                debug = true;
				console=new CL_ConsoleWindow("Trophy debugging console");
				console->redirect_stdio();
				
                std::cout << "Debug mode on" << std::endl;
            } 
            else if( !strcmp(argv[c], "--trackinfo" ) ) {
                trackInfo = true;
            }
            else if( !strcmp(argv[c], "--help") || !strcmp(argv[c], "-h") )
            {
                std::cout << "Usage: " << argv[0] << "[options]" << std::endl;
                std::cout << "Options :" << std::endl;
                std::cout << "--debug, -d : active debugging information" << std::endl;
                std::cout << "--trackinfo"  << std::endl;
                std::cout << "--help, -h : show this help message" << std::endl;
                std::cout << "--fullscreen, -f : fullscreen" << std::endl;
                std::cout << "--fast" << std::endl;
                std::cout << "--640x480, --800x600, --1024x768 : use corresponding resolution" << std::endl;
                std::cout << "--nosound" << std::endl;
                std::cout << "--server" << std::endl;
                std::cout << "--client" << std::endl;
                std::cout << "--ip" << std::endl;
                exit(0);
            }

            // Graphic options:
            //
            else if( !strcmp(argv[c], "--fullscreen" ) || !strcmp(argv[c], "-f") ) {
                fullScreen = true;
            } else if( !strcmp(argv[c], "--fast" ) ) {
                fast = true;
            } else if( !strcmp(argv[c], "--640x480" ) ) {
                width = 640;
                height = 480;
            } else if( !strcmp(argv[c], "--800x600" ) ) {
                width = 800;
                height = 600;
            } else if( !strcmp(argv[c], "--1024x768" ) ) {
                width = 1024;
                height = 768;
            }

            // Audio options:
            //
            else if( !strcmp(argv[c], "--nosound" ) ) {
                sound = false;
            }

            // Network options:
            //
            else if( !strcmp(argv[c], "--server" ) ) {
                server = true;
            } else if( !strcmp(argv[c], "--client" ) ) {
                client = true;
            } else if( !strcmp( argv[c], "-i" ) || !strcmp( argv[c], "--ip" ) ) {
                if( c<argc-1 ) strcpy( serverIp, argv[c+1] );
            }

            ++c;
        }

        CL_SetupDisplay setup_display;
        CL_SetupCore setup_core;
#ifdef USE_SDL
        CL_SetupSDL setup_sdl;
#else
        CL_SetupGL setup_gl;
#endif

        // Some layout things:
        //
        panelWidth = 120;
        headerHeight = (int)((float)height/4.2f);

        // Init sound:
        //
        CL_SetupSound setup_sound;
        sound_output = new CL_SoundOutput(44100);

        reconfigure();

        // Load resources:
        //
        CAResources::getResources();
        CASoundEffect::playBackgroundMelody();

        //priceList.setList( "$750~$3000~$12000" );
        offsetX = offsetY = 0;

        // Init everything:
        //
        initCarTypes();
        loading.setProgress( 60 );
        initGoodies();
        initPlayers();
        // initNetwork();
        initPanel();
        initTrackList();

        track.visualMap = 0;
        track.functionMap = 0;
        track.bridge = 0;
        track.file = "";

        loading.end();

        // Run:
        //
        runMenu();

        // Deinit everything:
        //
        deinitTrack();
        deinitPanel();
        // deinitNetwork();
        deinitPlayers();
        deinitGoodies();
        deinitCarTypes();
        delete m_ConfigureKey;

        // TODO : The only way I found to restore properly the resolution before exiting
        if(fullScreen) display_window->set_windowed();
    } 
    catch (CL_Error err) 
    {
        std::cout << "An error occured: " << err.message << std::endl;
    }

    if (console)
    {
            std::cout << "Press a key to exit" << std::endl;
            console->wait_for_key();
            delete console;
            console=0;
    }

    return 0;
}

/** Gets the application title.
    Needed by ClanLib.
*/
const std::string
CATrophy::get_title() {
    return "Trophy " VERSION;
}

/** Initializes car types.
*/
void
CATrophy::initCarTypes() 
{
    if(debug) std::cout << "initCarTypes begin" << std::endl;

    for( int i=0; i<CA_NUMCARTYPES; ++i ) 
    {
        if(debug) std::cout << "  car # " << i << std::endl;

        std::string mainPath = "cars/";
        std::string path;
        switch( i ) 
        {
        case 0:
            mainPath += "vw/";
            break;
        case 1:
            mainPath += "jeep/";
            break;
        case 2:
            mainPath += "z3/";
            break;
        case 3:
            mainPath += "flash2/";
            break;
        default:
            break;
        }

        if(debug) std::cout << "  name" << std::endl;

        path = mainPath + "name";
        carType[i].name = CL_String::load( path, CA_RES->resources );

        if(debug) std::cout << "  surface" << std::endl;

        path = mainPath + "surface";
        carType[i].surface = new CL_Surface( path, CA_RES->resources );

        if(debug) std::cout << "  surface3d" << std::endl;

        path = mainPath + "surface3d";
        carType[i].surface3d = new CL_Surface( path, CA_RES->resources );

        path = mainPath + "length";
        carType[i].length = CL_Integer( path, CA_RES->resources );
        path = mainPath + "width";
        carType[i].width = CL_Integer( path, CA_RES->resources );
        path = mainPath + "maxSpeed";
        carType[i].maxSpeed = 6 * CL_Integer( path, CA_RES->resources );
        path = mainPath + "minSpeed";
        carType[i].minSpeed = 6 * CL_Integer( path, CA_RES->resources );
        path = mainPath + "maxTurbo";
        carType[i].maxTurbo = CL_Integer( path, CA_RES->resources );
        path = mainPath + "acceleration";
        carType[i].acceleration = 6 * CL_Integer( path, CA_RES->resources );
        path = mainPath + "deceleration";
        carType[i].deceleration = 6 * CL_Integer( path, CA_RES->resources );
        path = mainPath + "steeringPower";
        carType[i].steeringPower = CL_Integer( path, CA_RES->resources );
        path = mainPath + "slidingFactor";
        carType[i].slidingFactor = 0.01 * CL_Integer( path, CA_RES->resources );
        path = mainPath + "price";
        carType[i].price = CL_Integer( path, CA_RES->resources );

        carType[i].radius = std::sqrt( (double)carType[i].width/2 * (double)carType[i].width/2 + (double)carType[i].length/2 * (double)carType[i].length/2 );
        carType[i].angle = atan( (double)(carType[i].width/2) / (double)(carType[i].length/2) ) * ARAD;
    }

    if(debug) std::cout << "initCarTypes end" << std::endl;
}

/** Deinitializes car types.
*/
void
CATrophy::deinitCarTypes() 
{
    if(debug) std::cout << "deinitCarTypes begin" << std::endl;

    for( int i=0; i<CA_NUMCARTYPES; ++i ) 
    {
        if( carType[i].surface ) delete carType[i].surface;
        carType[i].surface = 0;
        if( carType[i].surface3d ) delete carType[i].surface3d;
        carType[i].surface3d = 0;
    }

    if(debug) std::cout << "deinitCarTypes end" << std::endl;
}

/** Initializes goody types and instances.
*/
void
CATrophy::initGoodies() 
{
    goodyType[0].name    = "Turbo";
    goodyType[0].surface = CA_RES->goody_turbo;
    goodyType[0].life    = 30000;

    goodyType[1].name    = "Life";
    goodyType[1].surface = CA_RES->goody_life;
    goodyType[1].life    = 30000;

    goodyType[2].name    = "Money";
    goodyType[2].surface = CA_RES->goody_money;
    goodyType[2].life    = 15000;

    goodyType[3].name    = "Bullets";
    goodyType[3].surface = CA_RES->goody_bullets;
    goodyType[3].life    = 30000;

    goodyType[4].name    = "Fogbomb";
    goodyType[4].surface = CA_RES->goody_fogbomb;
    goodyType[4].life    = 10000;

    // Create goody instances:
    //
    for( int gt=0; gt<CA_NUMGOODYTYPES; gt++ ) {
        for( int gi=0; gi<CA_NUMGOODIES; gi++ ) {
            goody[gt][gi] = new CAGoody( &goodyType[gt] );
        }
    }
}

/** Deinitializes goody instances.
*/
void
CATrophy::deinitGoodies() 
{
    for( int gt=0; gt<CA_NUMGOODYTYPES; gt++ ) {
        for( int gi=0; gi<CA_NUMGOODIES; gi++ ) {
            if( goody[gt][gi] ) delete goody[gt][gi];
            goody[gt][gi] = 0;
        }
    }
}

/** Initializes the players.
    As default, player[0] is human and the others are computer
    players. As soon as a network player joins, a computer
    player gets replaced by a net player.
*/
void
CATrophy::initPlayers() 
{
    if(debug) std::cout << "initPlayers begin" << std::endl;

    int ct=0;
    for(int pl=0; pl<CA_MAXPLAYERS; ++pl)
    {
        if (pl == 0)
        {
            HumanPlayer* theHumanPlayer = new HumanPlayer( pl, "", 0 );
            player.push_back( theHumanPlayer );
            m_ConfigureKey = new CAConfigureKey(theHumanPlayer);
        }
        else
            player.push_back ( new ComputerPlayer( pl, "", 0));

        if( pl!=0 ) {
            int sat = TrophyMath::getRandomNumber( -90, 20 );
            int val = TrophyMath::getRandomNumber( -60, 0 );
            player[pl]->setColor( HSVColor((int)((float)pl/CA_MAXPLAYERS*360),sat,val) );
        }
        ++ct;
        if( ct>=CA_NUMCARTYPES ) ct=0;

        loading.setProgress( 60.0 + 40.0/CA_MAXPLAYERS*pl );
    }
    player[0]->setName( "Andrew" );
    if( CA_MAXPLAYERS>1 ) player[1]->setName( "Greenhorn" );
    if( CA_MAXPLAYERS>2 ) player[2]->setName( "Dark Rider" );
    if( CA_MAXPLAYERS>3 ) player[3]->setName( "Speedy Joe" );
    if( CA_MAXPLAYERS>4 ) player[4]->setName( "Sunnyboy" );
    if( CA_MAXPLAYERS>5 ) player[5]->setName( "Jane" );

    if(debug) std::cout << "initPlayers end" << std::endl;
}

/** Deinitializes and deletes the players.
*/
void
CATrophy::deinitPlayers() 
{
    for(int pl=0; pl<CA_MAXPLAYERS; ++pl) 
    {
        if( player[pl] ) delete player[pl];
        player[pl] = 0;
    }
}

/** Initializes the network
    (Server for all and Client for human player).
void
CATrophy::initNetwork() {
       CL_SetupNetwork::init();
    netServer = new CANetServer();
    netClient = new CANetClient( player[0] );

    // Start server directly:
    if( server ) netServer->createGame();
    // Join running game directly:
    if( client ) netClient->joinGame();
}
*/

/** Deinitializes the network.
    Deletes server and client.
void
CATrophy::deinitNetwork() {
    delete netServer;
    netServer = 0;
    delete netClient;
    netClient = 0;
}
*/

/** Initializes the panel on the left.
*/
void
CATrophy::initPanel() 
{
    if(debug) std::cout << "Create panel begin" << std::endl;
    panel = new CAPanel();
    if(debug) std::cout << "Create panel end" << std::endl;
}

/** Deinitializes the panel on the left.
*/
void
CATrophy::deinitPanel() 
{
    if( panel ) delete panel;
    panel = 0;
}

/** Initializes the track list. Track names are read fro the
    track directory (tracks/...)
*/
void
CATrophy::initTrackList() 
{
    std::istringstream iss ( CL_String::load("tracks/directories", CA_RES->resources).c_str() );
    std::string temp;
    while (std::getline(iss,temp, '~'))
       trackList.push_back(temp);
}

/** Initializes track with given name.
*/
void
CATrophy::initTrack( const std::string& trackName ) 
{
    loading.begin();

    if(debug) std::cout << "Init track begin" << std::endl;

    // Base path of track files:
    //
    std::string trackPath = std::string("tracks/") + trackName + "/";

    // Load visual map:
    //
    std::string vmapPath = trackPath + "vmap.tga";
    if( track.visualMap != NULL ) delete track.visualMap;
    try
    {
        track.visualMap = new CL_Surface(CL_ProviderFactory::load(vmapPath));
    }
    catch(CL_Error err)
    {
        trackPath = std::string("../resources/tracks/") + trackName + "/";
        vmapPath = trackPath + "vmap.tga";
        track.visualMap = new CL_Surface(CL_ProviderFactory::load(vmapPath));
    }

    loading.setProgress( 15 );

    // Load functional map:
    //
    std::string fmapPath = trackPath + "fmap.tga";
    if( track.functionMap != NULL ) delete track.functionMap;
    // We need to load a surface and then create the pixelbuffer if we don't want to loose the information
    // about the color. Don't know if it is a bug of ClanLib or the normal behavior.
    track.functionMap = new CL_PixelBuffer( ((CL_Surface)CL_ProviderFactory::load(fmapPath)).get_pixeldata() );
    // TODO : is it OK if we never unlock it ? functionMap is not meant to be drawn
    track.functionMap->lock();

    loading.setProgress( 30 );

    // Reset bridge:
    //
    if( track.bridge!=0 ) {
        delete track.bridge;
        track.bridge = 0;
    }

    // Read config file for this track:
    //
    std::string configFilePath = trackPath + "config.tck";
    //  CL_String( "tracks/" ).append( trackName ).append( "/config.tck" );

    if(debug) printf( "%s\n", configFilePath.c_str() );

    track.routePoints = 0;


    std::ifstream tckFile (configFilePath.c_str());
    if ( tckFile )
    {
        std::string line;                 // The line converted to a string
        std::string name;                 // Name of the value (Author)
        std::string value;                // Value (Andrew Mustun)
        int xv=0, yv=0, zv=0;           // Coordinates for following objects
        int iv=0;                       // Index for followint object (route point)
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
                if( name=="Author" )         iss >> track.author;
                else if( name=="Version")    iss >> track.version;
                else if( name=="Name"   )    iss >> track.name;

                // Coordinates of next object / route point:
                //
                else if( name=="x" )         iss >> xv;
                else if( name=="y" )         iss >> yv;
                else if( name=="z" )         iss >> zv;
                else if( name=="i" )         iss >> iv;

                // Start angle:
                //
                else if( name=="startAngle" ) iss >> track.startAngle;

                // Route points:
                //
                else if( name=="RP" ) {
                    int pi;
                    iss >> pi;
                    if (pi<CA_MAXROUTEPOINTS && iv<CA_MAXPLAYERS) {
                        track.rp[iv][pi][0] = xv;
                        track.rp[iv][pi][1] = yv;
                        if (pi+1>track.routePoints) track.routePoints = pi+1;
                    }
                }

                // Objects:
                //
                else if( name=="Object" && value=="bridge") {
                    std::string bridgePath = trackPath + value + ".tga";
                    track.bridge = new CL_Surface(CL_TargaProvider(bridgePath));
                    track.bridgePos[0] = xv;
                    track.bridgePos[1] = yv;
                }
            }
        }
        tckFile.close();
    }

    loading.setProgress( 40 );

    // Place and reset players:
    //
    for( int pl=0; pl<CA_MAXPLAYERS; pl++ ) {
        int rn;
        bool done;
        do {
            done = true;
            rn = TrophyMath::getRandomNumber( 0, CA_MAXPLAYERS-1 );
            for( int pl2=0; pl2<pl; ++pl2 )
            {
                if( player[pl2]->getRouteNumber() == rn )
                    done=false;
            }
        } while( !done );

        player[pl]->resetForRace();
        player[pl]->initPlayer( rn );

        loading.setProgress( 50.0 + 50.0/CA_MAXPLAYERS*pl );
    }

    // Reset starting light:
    //
    lightState = 0;

    // Reset time:
    //
    time = 0;
    timeString = "00:00.00";

    // Reset some other things which must be reset for each lap:
    //
    pause = false;
    allowShooting = false;
    firstPlayerFinished = false;
    resetGoodies();
    resetFogBombs();
    resetDustClowds();
    CA_POSITIONTABLE->resetRace();

    if(debug) std::cout << "Init track end" << std::endl;
}

/** Deinitializes and deletes current track.
*/
void
CATrophy::deinitTrack() {
    if( track.visualMap!=0 ) delete track.visualMap;
    track.visualMap = 0;

    if( track.functionMap != NULL ) 
    {
        delete track.functionMap;
        track.functionMap = NULL;
    }

    if( track.bridge!=0 ) delete track.bridge;
    track.bridge = 0;
}

/** Reconfigures parameters. Called by the configure menu.
*/
void
CATrophy::reconfigure() 
{
    static bool firstCall = true;
    static bool lastFullScreen = fullScreen;

    sound_output->set_global_volume(volume/(float)10);
    if(firstCall)
    {
        try 
        {
            display_window = new CL_DisplayWindow ( get_title(), width, height, fullScreen );
            input_context = display_window->get_ic();
        } 
        catch( CL_Error err ) 
        {
            std::cout << "Exception caught: " << err.message << std::endl;
        }
        lastFullScreen = fullScreen;
    }
    else if(lastFullScreen != fullScreen ) 
    {
        if(fullScreen) 
            display_window->set_fullscreen(width, height);
        else
            display_window->set_windowed();
        lastFullScreen = fullScreen;
    }

    // Init mouse cursor:
    //
    if( debug )
        CL_Mouse::show();
    else
        CL_Mouse::hide();

    firstCall = false;
}

/** Resets all goodies. Done before a new race.
*/
void
CATrophy::resetGoodies() {
    for( int gt=0; gt<CA_NUMGOODYTYPES; gt++ ) {
        for( int gi=0; gi<CA_NUMGOODIES; gi++ ) {
            goody[gt][gi]->setActive( false );
        }
    }
}

/** Resets all fog bombs. This happens before a new race.
*/
void
CATrophy::resetFogBombs() {
    fogBombCounter = 0;
    for( int c=0; c<CA_MAXFOGBOMBS; ++c ) {
        fogBomb[c].active = false;
        fogBomb[c].exploded = false;
    }
}

/** Resets all dust clowds. This happens before a new race.
*/
void
CATrophy::resetDustClowds() {
    dustClowdCounter = 0;
    for( int c=0; c<CA_MAXDUSTCLOWDS; ++c ) {
        dustClowd[c].active = false;
        dustClowd[c].faded = false;
    }
}

/** Resets all players. Happens before a new game
    (car types, points, money).
*/
void
CATrophy::resetPlayers() {
    for( int pl=0; pl<CA_MAXPLAYERS; ++pl ) {
        player[pl]->reset();
        player[pl]->setTotalPoints(pl*4+pl*3);
        player[pl]->setCarNumber( 0, player[pl]->getCarNumber()!=0 );
    }
}

/** Called by CATrophy::main() to start the menu in an std::endless loop.
*/
void
CATrophy::runMenu() 
{
    bool done=false;

    //CAMenu netOptionsMenu( "Network Options" );
    //netOptionsMenu.addMenuInput( "IP", CA_APP->serverIp, 15 );
    //netOptionsMenu.addMenuInput( "Port", &(CA_APP->serverPort), 5 );
    //netOptionsMenu.addMenuLabel( "Previous Menu" );

    CAMenu* mainMenu;
    CAMenu* raceMenu;
    //CAMenu* netMenu;
    int mainMenuSelection, raceMenuSelection;
    //netMenuSelection, configMenuSelection;

    do 
    {
        mainMenu = new CAMenu( "Main Menu" );
        mainMenu->addMenuLabel( "Start Racing" );
        //mainMenu->addMenuLabel( "Multiplayer Race" );
        mainMenu->addMenuLabel( "Configure" );
        mainMenu->addMenuLabel( "See Hall Of Fame" );
        mainMenu->addMenuLabel( "Credits" );
        mainMenu->addMenuLabel( "Exit To System" );

        mainMenu->run();
        mainMenuSelection = mainMenu->getSelection();
        delete mainMenu;

        switch( mainMenuSelection ) 
        {
            // Start Racing:
            //
        case 0:
            do 
            {
                raceMenu = new CAMenu( "Start Racing" );
                raceMenu->addMenuLabel( "Start A New Game" );
                //raceMenu->addMenuLabel( "Load Game" );
                //raceMenu->addMenuLabel( "Save Game" );
                raceMenu->addMenuLabel( "See Statistics" );
                // TODO : Previous Menu ? End Current Game ? Which one is best ?
                //  - Previous Menu seems more logic, but it doesn't make it very clear that you lose your game (maybe when there will be save/load ?)
                //  - End Current Game doesn't tell us that we're going back to the main menu
                raceMenu->addMenuLabel( "End Current Game" );
                //raceMenu->addMenuLabel( "Previous Menu" );
                raceMenu->run();
                raceMenuSelection = raceMenu->getSelection();
                delete raceMenu;

                switch( raceMenuSelection ) 
                {
                case 0:  // Start new game

                    startNewGame();
                    break;

                case 1:  // See Statistics

                    runPositionTable( true );
                    break;

                case 2:  // End current Game (if the number change, raceMenuSelection comparison should also change)

                    break;

                default:
                    break;
                }
            } while( raceMenuSelection != 2 );
            break;

            // Multiplayer:
            //
            /*
            case 1:
              do {
                netMenu = new CAMenu( "Multiplayer Race" );
                netMenu->addMenuLabel( "Initialize A New Game" );
                netMenu->addMenuLabel( "Join An Existing Game" );
                netMenu->addMenuLabel( "Network Options" );
                netMenu->addMenuLabel( "Previous Menu" );
                netMenu->run();
                netMenuSelection = netMenu.getSelection();
                delete netMenu;

                switch( netMenuSelection ) {
                  case 0:  // Init new Game
                    startServer();
                    break;

                  case 1:  // Join Game
                    chooseNetGame();
                    break;

                  case 2:  // Net Options
                    netOptionsMenu.run();
                    break;
                }
              } while( netMenuSelection!=3 );

              break;
              */

            // Configure:
            //
        case 1:
        {
            CAMenu configMenu( "Configure" ) ;
            configMenu.addMenuSelect( "Fullscreen", "off~on", &CA_APP->fullScreen );
            configMenu.addMenuSelect( "Sound", "off~on", &CA_APP->sound );
            configMenu.addMenuSelect( "Sound volume", "0%~10%~20%~30%~40%~50%~60%~70%~80%~90%~100%", &CA_APP->volume );
            configMenu.addMenuLabel( "Configure Keyboard" );
            configMenu.addMenuLabel( "Previous Menu" );
            configMenu.setConfigureMenu( true );
            configMenu.run();
            if (configMenu.getSelection() == 3)
            {
                m_ConfigureKey->run();           
            }
        }
            break;


            // See Hall Of Fame:
            //
        case 2:
            runPositionTable( false );
            break;


            // Credits:
            //
        case 3:
            runCreditsScreen();
            break;


            // Exit:
            //
        case 4:
            done=true;
            break;
        }
    } while( !done );
}

/** Called by CATrophy::runMenu() to start the position table in an std::endless loop.
    \param race true: Show table for last race / false: Total results table
    \return true on success (User pressed Enter or Space) otherwise false (User pressed ESC)
*/
bool
CATrophy::runPositionTable( bool race ) {
    CAPositionTableView positionTableView( race );
    return positionTableView.run();
}

/** Called by CATrophy::runMenu() to start the credits screen in an std::endless loop.
*/
void
CATrophy::runCreditsScreen() {
    CACredits credits;
    credits.run();
}

/** Called by CATrophy::runMenu() to start the server
    info screen in an std::endless loop.
    \param success true: Show info that the server is running.
                   false: Show error message.
*/
void
CATrophy::runServerInfo( bool success ) {
    std::string text;
    CAInfoDialog::InfoType type;

    if( success ) {
        text = "Server running on port ";
        text += CA_APP->serverPort;
        type = CAInfoDialog::Info;
    } else {
        text = "Can't run server on port ";
        text += CA_APP->serverPort;
        text += ".~Maybe there's already a server~running on this port.";
        type = CAInfoDialog::Warning;
    }
    CAInfoDialog iDlg( "Server Info", text.c_str(), type );
    iDlg.run();
}

/** Starts the client info screen in an endless loop.
    \param success true: Show info that the client is running.
                   false: Show error message.
*/
void
CATrophy::runClientInfo( bool success ) {
    std::string text;
    CAInfoDialog::InfoType type;

    if( success ) {
        text = "Server found on ip ";
        text += CA_APP->serverIp;
        text += "~running on port ";
        text += CA_APP->serverPort;
        text += " :-)";
        type = CAInfoDialog::Info;
    } else {
        text = "No server found on ip ";
        text += CA_APP->serverIp;
        text += "~and port ";
        text += CA_APP->serverPort;
        text += " :-(";
        type = CAInfoDialog::Warning;
    }
    CAInfoDialog iDlg( "Client Info", text.c_str(), type );
    iDlg.run();
}

/** Starts a new game. Called by the menu.
*/
void
CATrophy::startNewGame() 
{
    if( debug ) std::cout << "startNewGame() begin" << std::endl;

    CAMenu* difficultyMenu = new CAMenu( "Select Difficulty:" );
    difficultyMenu->addMenuLabel( "Speed makes me dizzy" );
    difficultyMenu->addMenuLabel( "I live to ride" );
    difficultyMenu->addMenuLabel( "Petrol in my veins" );
    difficultyMenu->setCursor( 1 );

    resetPlayers();

    // Player settings dialog:
    //
    CAPlayerSettingsDialog playerSettingsDialog;
    if( playerSettingsDialog.run()==1 ) 
    {
        player[0]->setName( playerSettingsDialog.getPlayerName() );
        player[0]->setColor( HSVColor( playerSettingsDialog.getPlayerHue(),0,0 ) );

        // Choose difficulty:
        //
        int dif = difficultyMenu->run();
        delete difficultyMenu;
        if( dif >= 0 ) 
        {
            difficulty = (Difficulty)dif;
            bool goon=true;

            // Choose track:
            //
            CASignUpScreen* signUpScreen;
            int trackNumber;
            do 
            {
                
                signUpScreen = new CASignUpScreen();
                trackNumber = signUpScreen->run();
                delete signUpScreen;
                if( trackNumber != -1 ) 
                {
                    track.file = trackList[trackNumber];
                    run();
                    goon = runPositionTable( true );
                    {
                        CAChampionshipScreen myChampionShip(player, CA_RES->menu_bg, CA_RES->gui_button, CA_RES->font_normal_14_white);
                        myChampionShip.run();
                    }
                }
            } while( trackNumber!=-1 && goon );
        }
    }

    if( debug ) std::cout << "startNewGame() end" << std::endl;
}

/** Starts a trophy server on this computer.
void
CATrophy::startServer() {
    bool success = netServer->createGame();
    runServerInfo( success );
}
*/

/** Choose a running net-game.
void
CATrophy::chooseNetGame() {
    bool success = netClient->joinGame();
    runClientInfo( success );
}
*/

/** Adds a new net player and returns the id for it.
    Only called if we're a server.
    \return id of new net player or -1 if no player was
            available.
int
CATrophy::addNetPlayer() {
    int ret=-1;

    for( int pl=0; pl<CA_MAXPLAYERS; ++pl ) {
        if( player[pl] &&
                player[pl]->getControlMode()==Player::Computer ) {
            ret = pl;
            player[pl]->setControlMode( Player::Network );
            break;
        }
    }

    return ret;
}
*/

/** Controls the net player with given id.
    \param id Net players ID
    \param xpos x position of net player
    \param ypos y position of net player
    \param dir Direction (angle) of net player
*/
void
CATrophy::controlNetPlayer( int id,
                            int xpos,
                            int ypos,
                            int dir ) {
    player[id]->move( xpos, ypos );
    player[id]->setDirection( dir );
}

/** Called by CATrophy::runMenu() to start the
    game in an std::endless loop.
*/
int
CATrophy::run() 
{
    if(debug) std::cout << "Game Running" << std::endl;

    int  gameStartTime;     // Race started
    int  goodyTime;         // Last goody placed at...

    bool blocked = false;   // mouse blocked?
    int  rp = 0;            // rounting point
    int  rhythm = 0;        // Rhythm for networking.
    // Keep up Server on 0, 2, ...
    // Keep up Client on 1, 3, ...
    int  c;                 // Counter
    bool raceOver = false;  // Set to true if race is over
    int  raceOverTime = 0;  // Time stamp at the time when the race was over.

    // Init track:
    //
    initTrack( track.file );

    // Init map offset:
    //
    scroll();

    // Fade in:
    //
    fadeScreen( true, this );

    goodyTime = gameStartTime = CL_System::get_time();

    if( debug ) std::cout << "Start game loop" << std::endl;

    // Loop until 'Escape' pressed or race is over:
    //
    while( CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false &&
            (!raceOver || CL_System::get_time()-raceOverTime<3000) ) 
    {
        measureFrameTime( true );

        if( CL_Keyboard::get_keycode(CL_KEY_P) ) {
            pause = !pause;
        }

        if( lightState==3 && !pause ) 
        {
            // Control players:
            //
            raceOver = true;
            for( c=0; c<CA_MAXPLAYERS; ++c ) {
                player[c]->pilot();

                if( player[c]->hasFinished() ) {       // Check if first player has finished
                    firstPlayerFinished = true;
                } else if( c==0 && player[c]->getLife()>0.0 ) {  // Check if race is over...
                    raceOver=false;
                }
            }
            if(raceOver && raceOverTime==0) raceOverTime = CL_System::get_time();

            // 'Advance' goodies:
            //
            for( int gt=0; gt<CA_NUMGOODYTYPES; gt++ ) {
                for( int gi=0; gi<CA_NUMGOODIES; gi++ ) {
                    goody[gt][gi]->advance();
                }
            }

            // Advance players:
            //
            for(c=0; c<CA_MAXPLAYERS; ++c) {
                player[c]->advance();
            }

            // Place goodies:
            //
            if( CL_System::get_time() > (unsigned int)(goodyTime+CA_GOODYTIME) ) {
                goodyTime = CL_System::get_time();
                placeGoody();
            }

            // Uptime time:
            //
            time = CL_System::get_time()-gameStartTime;
            TrophyMath::timeToString( timeString, time );
        }

        // Find out new ranks:
        //
        setRanks();

        // Mouse clicks for track creation:
        //
        if( debug && trackInfo ) 
        {
            if( CL_Mouse::get_keycode(CL_MOUSE_LEFT) ) 
            {
                if( !blocked ) 
                {
                    blocked = true;
                    FILE* fp = fopen( "trackdata.txt", "at" );
                    if( fp ) 
                    {
                        fprintf( fp, "x = \"%d\"\n", CL_Mouse::get_x()-offsetX );
                        fprintf( fp, "y = \"%d\"\n", CL_Mouse::get_y()-offsetY );
                        fprintf( fp, "RP = \"%d\"\n\n", rp++ );
                        fclose( fp );
                        CL_Color fmap_pix = track.functionMap->get_pixel(CL_Mouse::get_x()-offsetX, CL_Mouse::get_y()-offsetY);
                        CL_PixelBuffer pixbuf = track.visualMap->get_pixeldata();
                        pixbuf.lock();
                        CL_Color vmap_pix = pixbuf.get_pixel(CL_Mouse::get_x()-offsetX, CL_Mouse::get_y()-offsetY);
                        pixbuf.unlock();
                        std::cout << "vmap (r, g, b, a) = (" << vmap_pix.get_red() << ", " << vmap_pix.get_green() << ", " << vmap_pix.get_blue() << ", " << vmap_pix.get_alpha() << ")" << std::endl;
                        std::cout << "fmap (r, g, b, a) = (" << fmap_pix.get_red() << ", " << fmap_pix.get_green() << ", " << fmap_pix.get_blue() << ", " << fmap_pix.get_alpha() << ")" << std::endl;
                    }
                }
            } 
            else 
            {
                blocked = false;
            }
        }

        // Scroll to center player 0:
        //
        scroll();

        // Build the whole screen:
        //
        buildScreen();

        // Advance starting light:
        //
        if( lightState!=3 && (CL_System::get_time()-gameStartTime) > 1500 ) 
        {
            gameStartTime = CL_System::get_time();
            ++lightState;
            if(sound) 
            {
                CA_RES->effectLight->play( 3 );
            }
        }

        // Allow shooting after 3 seconds:
        //
        if( !allowShooting && (CL_System::get_time()-gameStartTime) > 3000 ) {
            allowShooting=true;
        }

        // Keep up web serving every other loop:
        /*
        if( rhythm==0 ) {
            netServer->keepUp();
        } else if( rhythm==1 ) {
            netClient->keepUp();
        }
	*/

        rhythm++;
        if( rhythm==3 ) rhythm=0;

        CL_Display::flip();   // Copy framebufer to screen
        CL_System::keep_alive();      // VERY VITAL for the system!

        measureFrameTime( false );
    }

    if( debug ) std::cout << "Stop game loop" << std::endl;

    waitForSilence();

    // Finish all players manually
    //
    for( int rank=1; rank<=CA_MAXPLAYERS; ++rank ) {
        // Players which are alive:
        //
        for( c=0; c<CA_MAXPLAYERS; ++c ) {
            if( !player[c]->hasFinished() &&
                    !player[c]->isDeath() &&
                    player[c]->getRaceRank()==rank ) {
                CA_POSITIONTABLE->playerFinishedRace( player[c] );
                break;
            }
        }
    }

    // Show race over dialog:
    //
    CAInfoDialog raceOverDlg( "Race over!",
                              "Press Enter to continue...",
                              CAInfoDialog::Info,
                              true, this );
    raceOverDlg.run();

    buyCars();

    fadeScreen( false, this );

    return 0;
}

/** Places a new goody (not always successful)
*/
void
CATrophy::placeGoody() {
    int x[4];    // 4 edge-points of new goody
    int y[2];

    int sx = TrophyMath::getRandomNumber( 8, track.visualMap->get_width()-8 );
    int sy = TrophyMath::getRandomNumber( 8, track.visualMap->get_height()-8 );
    int gt = TrophyMath::getRandomNumber( 0, CA_NUMGOODYTYPES-1 );
    int level = TrophyMath::getRandomNumber( 0, 1 );
    bool validPlace = true;
    int referenceLevel = 0;

    x[0] = sx - 8;
    y[0] = sy - 8;
    x[1] = sx + 8;
    y[1] = sy + 8;

    for( int ix=0; ix<2; ++ix ) {
        for( int iy=0; iy<2; ++iy ) {
            if( ix==0 && iy==0 ) referenceLevel = getLevel( x[ix],y[iy] );
            if( getSpeedLimit( x[ix],y[iy] )<15 ||
                    getLevel( x[ix],y[iy] )!=referenceLevel ) {
                validPlace = false;
            }
        }
    }

    if( validPlace ) {
        for( int gi=0; gi<CA_NUMGOODIES; gi++ ) {
            if( !goody[gt][gi]->isActive() ) {
                goody[gt][gi]->move( sx, sy, (referenceLevel==2 ? level : (referenceLevel==1)) );
                goody[gt][gi]->setActive( true );
                break;
            }
        }
    }
}

/** Sets the ranks of the players during race.
*/
void
CATrophy::setRanks() {
    float thisPos;
    float pos;

    int nextRank;
    int rank=1;
    int pl;

    // Clear ranks for players in the race and adjust first available rank
    //
    for( pl=0; pl<CA_MAXPLAYERS; ++pl ) {
        if( !player[pl]->isDeath() ) {
            if( player[pl]->hasFinished() ) {
                if( player[pl]->getRaceRank()>=rank ) rank = player[pl]->getRaceRank()+1;
            } else {
                player[pl]->setRaceRank(0);
            }
        }
    }

    do {
        pos = 0.0;
        nextRank=-1;
        for( pl=0; pl<CA_MAXPLAYERS; ++pl ) {
            if( player[pl]->getRaceRank()==0 ) {
                thisPos = player[pl]->getPosition();
                if( thisPos>pos ) {
                    nextRank = pl;
                    pos = thisPos;
                }
            }
        }

        if( nextRank!=-1 ) {
            if( !player[nextRank]->hasFinished() && !player[nextRank]->isDeath() ) {
                player[nextRank]->setRaceRank( rank );
            }
        }

        rank++;
    } while( nextRank!=-1 && rank<=CA_MAXPLAYERS );
}

/** Gets the speed limit for a coordinate of the map
*/
int
CATrophy::getSpeedLimit( int x, int y ) 
{
    if( checkCoordinate( x,y ) ) 
    {
        CL_Color tmp = track.functionMap->get_pixel( x,y );
        unsigned int g = tmp.get_green();
        return ((g)&0xF0)>>4;
    }

    return 0;
}

/** Gets the lap part for a coordinate of the map
*/
int
CATrophy::getLapPart( int x, int y ) 
{
    if( checkCoordinate( x,y ) ) 
    {
        CL_Color tmp = track.functionMap->get_pixel( x,y );
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
int
CATrophy::getLevel( int x, int y ) 
{
    if( checkCoordinate( x,y ) ) 
    {
        CL_Color tmp = track.functionMap->get_pixel( x,y );
        unsigned int g = tmp.get_green();
        if( (g&0x02)!=0 ) return 1;
        if( (g&0x01)!=0 ) return 0;
    }

    return 2;
}

/** Checks if the gifen coordinate is on the map and returns true if so.
*/
bool
CATrophy::checkCoordinate( int x, int y ) 
{
    // TODO get_width and get_height won't return the right value if the track size isn't a power of 2
    // when functionMap is a PixelBuffer
    //return ( x>=0 && y>=0 && x<(int)track.functionMap->get_width() && y<(int)track.functionMap->get_height() );
    return ( x>=0 && y>=0 && x<(int)track.visualMap->get_width() && y<(int)track.visualMap->get_height() );
}

/** Drops a fog bomb on the given place. This function
    is called from evil players.
*/
void
CATrophy::dropFogBomb( int x, int y, bool up ) {
    fogBomb[fogBombCounter].active = true;
    fogBomb[fogBombCounter].x = x;
    fogBomb[fogBombCounter].y = y;
    fogBomb[fogBombCounter].up = up;
    fogBomb[fogBombCounter].frame = 0.0;
    fogBomb[fogBombCounter].exploded = false;
    fogBombCounter++;
    if( fogBombCounter>=CA_MAXFOGBOMBS ) {
        fogBombCounter = 0;
    }
}

/** Created a dust clowd on the given place. This function
    is called from sliding players.
*/
void
CATrophy::makeDustClowd( int x, int y, bool up ) {
    dustClowd[dustClowdCounter].active = true;
    dustClowd[dustClowdCounter].x = x;
    dustClowd[dustClowdCounter].y = y;
    dustClowd[dustClowdCounter].up = up;
    dustClowd[dustClowdCounter].frame = 0.0;
    dustClowd[dustClowdCounter].faded = false;
    dustClowdCounter++;
    if( dustClowdCounter>=CA_MAXDUSTCLOWDS ) {
        dustClowdCounter = 0;
    }
}

/** Measures the time we needed to draw a frame. Used
    for calculating the frame rate.
*/
void
CATrophy::measureFrameTime( bool start ) {
    static int frameStart = 0;

    if( start ) {
        frameStart = CL_System::get_time();
    } else {
        int timeElapsed = CL_System::get_time()-frameStart;
        if( timeElapsed>0 ) framesPerSec = 1000.0 / timeElapsed;
        if( framesPerSec<1.0 ) framesPerSec = 1.0;
    }
}

/** Waits until no key is pressed.
*/
void
CATrophy::waitForSilence() {
    bool done;
    do {
        done = true;
        if( CL_Keyboard::get_keycode( CL_KEY_ENTER ) ||
                CL_Keyboard::get_keycode( CL_KEY_ESCAPE ) ) {
            done = false;
        }
        CL_System::keep_alive();      // VERY VITAL for the system!
    }
    while( !done );
}

/** Distributes cars to players which have enough money.
*/
void
CATrophy::buyCars() {
    // Players with a lot of money get new cars:
    //
    if( debug ) std::cout << "Buy new cars begin" << std::endl;

    for( int pl=0; pl<CA_MAXPLAYERS; ++pl ) {
        for( int ca=CA_NUMCARTYPES-1; ca>=0; --ca ) {
            if( player[pl]->getMoney() >= carType[ca].price && player[pl]->getCarNumber()<ca ) {
                player[pl]->setCarNumber( ca );
                player[pl]->setMoney( player[pl]->getMoney()-carType[ca].price );
                player[pl]->setNewCar( true );
            }
        }
    }

    if( debug ) std::cout << "Buy new cars end" << std::endl;
}

/** Builds the whole game screen. ONLY this function is called
    for this purpose.
*/
void
CATrophy::buildScreen() 
{
    static CL_Rect crAll( 0,0, width, height );
    static CL_Rect crField( panelWidth,0, width, height );
    static CL_Rect crPanel( 0,0, panelWidth, height );

    // Do the game field:
    //
    CL_Display::set_cliprect(crField);

    displayMap();

    // Things under bridge:
    displayGoodies( false );
    displayFogBombs( false, true );
    displayDustClowds( false );
    displayPlayers( false );
    displayFogBombs( false, false );

    // Bridge:
    displayBridge();

    // Things on bridge:
    displayGoodies( true );
    displayFogBombs( true, true );
    displayDustClowds( true );
    displayPlayers( true );
    displayFogBombs( true, false );

    // Display tracks for debugging:
    //
    if( trackInfo ) {
        displayTrackPoints();
    }

    displayStartingLights();
    displayCheckFlag();

    // Do the panel:
    //
    CL_Display::set_cliprect(crPanel);
    panel->display();
    CL_Display::set_cliprect(crAll);
}

/** Displays the race map.
*/
void
CATrophy::displayMap()
{
    track.visualMap->draw (offsetX,offsetY);
}

/** Displays the goodies.
*/
void
CATrophy::displayGoodies( bool up ) {
    for( int gt=0; gt<CA_NUMGOODYTYPES; gt++ ) {
        for( int gi=0; gi<CA_NUMGOODIES; gi++ ) {
            if( goody[gt][gi]->isUp()==up ) goody[gt][gi]->display( offsetX, offsetY );
        }
    }
}

/** Displays the bridge if there is any.
*/
void
CATrophy::displayBridge() {
    if( track.bridge!=0 ) {
        track.bridge->draw (track.bridgePos[0]+offsetX, track.bridgePos[1]+offsetY);
    }
}

/** Displays players.
    \param up true: Players on bridges / false: Players under bridges
*/
void
CATrophy::displayPlayers( bool up ) {
    for( int c=0; c<CA_MAXPLAYERS; ++c) {
        if( player[c]->isUp()==up ) player[c]->display( offsetX, offsetY );
    }
}

/** Displays fog bombs.
    \param up true: Fog bombs on bridges / false: Fog bombs under bridges
    \param bomb true: The bombs themselves / false the fog of exploded bombs
*/
void
CATrophy::displayFogBombs( bool up, bool bomb ) {
    for( int c=0; c<CA_MAXFOGBOMBS; ++c) {
        if( (bomb  && (int)(fogBomb[c].frame)<=3) ||
                (!bomb && (int)(fogBomb[c].frame)>=4)    ) {
            if( fogBomb[c].active && fogBomb[c].up==up ) {

                // Bomb explodes (hit players):
                //
                if( !fogBomb[c].exploded && (int)(fogBomb[c].frame)==4 ) {
                    fogBomb[c].exploded=true;
                    CA_RES->effectFogBomb->play( 2 );
                    for( int pl=0; pl<CA_MAXPLAYERS; ++pl) {
                        int dist = (int)TrophyMath::getDistance( fogBomb[c].x, fogBomb[c].y, player[pl]->getX(), player[pl]->getY() );
                        if( player[pl]->isUp()==up && dist<90 ) {
                            player[pl]->hit( 30.0-(dist/3.0) );
                        }
                    }
                }

                CA_RES->misc_fog->set_frame((int)fogBomb[c].frame);
                CA_RES->misc_fog->draw ( fogBomb[c].x + offsetX - CA_RES->misc_fog->get_width()/2,
                                              fogBomb[c].y + offsetY - CA_RES->misc_fog->get_height()/2);
                if( CA_RES->advanceAnimation( &fogBomb[c].frame, CA_RES->misc_fog->get_frame_count(), 6.0 ) )
                {
                    fogBomb[c].active=false;
                }
            }
        }
    }
}

/** Displays dust clowds.
    \param up true: Dust clowds on bridges / false: Dust clowds under bridges
*/
void
CATrophy::displayDustClowds( bool up ) 
{
    int frame, x, y;
    for( int c=0; c<CA_MAXDUSTCLOWDS; ++c) 
    {
        if( dustClowd[c].active && dustClowd[c].up==up ) 
        {
            frame = (int)dustClowd[c].frame;
            x = dustClowd[c].x + offsetX - CA_RES->misc_dust->get_width()/2;
            y = dustClowd[c].y + offsetY - CA_RES->misc_dust->get_height()/2;

            if( frame >= 0 &&
                    frame < (int)CA_RES->misc_dust->get_frame_count() &&
                    checkCoordinate( x,y ) ) 
            {
                CA_RES->misc_dust->set_frame(frame);
                CA_RES->misc_dust->draw (x, y);
            }

            if( CA_RES->advanceAnimation( &dustClowd[c].frame, CA_RES->misc_dust->get_frame_count(), 8.0 ) ) 
            {
                dustClowd[c].active = false;
            }
        }
    }
}

/** Displays a waving check flag when the first player has
    reached the finish.
*/
void
CATrophy::displayCheckFlag() 
{
    static int flagPos = -CA_RES->misc_checkflag->get_height();
    if( firstPlayerFinished ) 
    {
        CA_RES->misc_checkflag->draw (width-CA_RES->misc_checkflag->get_width()-32,
                                            //panelWidth+(width-panelWidth-CA_RES->misc_checkflag->get_width())/2,
                                            flagPos);
        if( flagPos<32 ) flagPos+=2;

        CA_RES->misc_checkflag->update();
    }
}

/** Displays the starting light animation.
*/
void
CATrophy::displayStartingLights() {
    static int lightPos = 32;
    if( lightState<4 ) {
        if( lightState==3 ) {
            if( lightPos > (-(int)CA_RES->misc_light->get_height()) ) {
                lightPos-=2;
            }
        } else {
            lightPos = 0;
        }

        CA_RES->misc_light->set_frame(lightState);
        CA_RES->misc_light->draw (width-CA_RES->misc_light->get_width()-32,
                                        //panelWidth+(width-panelWidth-CA_RES->misc_light->get_width())/2,
                                        lightPos);
    }
}

/** Displays track points.
*/
void
CATrophy::displayTrackPoints() {
    char str[16];
    for( int r=0; r<track.routePoints; ++r ) {
        for( int t=0; t<CA_MAXPLAYERS; ++t ) {
            sprintf( str, "%d/%d", r, t );
            CA_RES->misc_cross->draw (track.rp[t][r][0]+offsetX-8, track.rp[t][r][1]+offsetY-8);
            CA_RES->font_normal_11_white->set_alignment(origin_top_left, 0, 0);
            CA_RES->font_normal_11_white->draw( track.rp[t][r][0]+offsetX, track.rp[t][r][1]+offsetY+5, str );
        }
    }
}

/** Fades the screen in (true) or out (false).
    \param in Fade in (true) or out (false)
    \param screen Pointer to the screen which is of type
                  CATrophy, CAMenu, CAPositionTableView or another
                  class derived from CAScreen.
    \param whole  Fade the whole screen? Otherwise the top part (TROPHY) won't fade.
*/
void
CATrophy::fadeScreen( bool in, CAScreen* screen, bool whole ) {
    // Dont do that anymore - Since Clanlib 0.5 it's bloody slow...
    return;

    if( debug ) std::cout << "fadeScreen begin" << std::endl;

    screen->buildScreen();

    if( fast ) return;

    int b, tb, y;

    for( b=-16; b<16; ( in ? b+=4 : b+=2) ) {
        if( in ) screen->buildScreen();

        tb = ( in ? 16-b : b );
        for( y=height; y>=0; y-=32 ) {
            if( tb>0 )
                CL_Display::fill_rect( CL_Rect(0, y-tb, width, y+tb), CL_Color(0, 0, 0, 255) );
            tb = ( in ? tb-1 : tb+1 );
        }

        CL_Display::flip();
        CL_System::keep_alive();
    }

    if( debug ) std::cout << "fadeScreen end" << std::endl;
}

/** Scrolls the map to center player 0.
*/
void
CATrophy::scroll() {
    int maxOffsetX = panelWidth;
    int minOffsetX = -(track.visualMap->get_width()-(width-panelWidth)) + panelWidth;
    int maxOffsetY = 0;
    int minOffsetY = -(track.visualMap->get_height()-height);

    offsetX = - ((int)(player[0]->getX()) - (width-panelWidth)/2) + panelWidth;
    offsetY = - ((int)(player[0]->getY()) - height/2);
    if( offsetX>maxOffsetX ) offsetX = maxOffsetX;
    if( offsetX<minOffsetX ) offsetX = minOffsetX;
    if( offsetY>maxOffsetY ) offsetY = maxOffsetY;
    if( offsetY<minOffsetY ) offsetY = minOffsetY;
}

// EOF
