
#ifdef W32_BCC551
  /* dont include stl.h for Windoof, Borland C++ 5.5.1 */
#else
  //#include <stl.h>
  #include <iostream>
  #include <string>
#endif

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/application.h>
#include <ClanLib/gl.h>
#include <ClanLib/gl1.h>
#include <ClanLib/swrender.h>

#include <cmath>
#include <fstream>
#include <sstream>

#include "cartype.h"
#include "cacredits.h"
#include "caconfigurekey.h"
#include "caimagemanipulation.h"
#include "cainfodialog.h"
#include "caescapedialog.h"
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
#include "caslotselectiondialog.h"
#include "shopscreen.h"
#include "windialog.h"

// Global instance of the application needed by the ClanLib main():
//
CATrophy* application;


/** Global function to access the application instance.
*/
CATrophy*
theApp() {
    return application;
}

/** The main function.
    Called by ClanLib main().
*/
void
CATrophy::start(const std::vector<CL_String> &args ) 
{
    CL_ConsoleWindow *console=0;
    try 
    {
        // Default paramters:
        //
	    display_window = NULL;
        debug = false;
        trackInfo = false;
        framesPerSec = 40;
        resolution = CA_RESOLUTION;
        m_cheatMoney = false;
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
        m_isGameStarted = false;
        m_nbTurns = 0;
        m_gameLoopState = 0;
        

        // Proccess parameters:
        //
        for( unsigned c =1; c< args.size();c++)
        {
            // General options:
            //
            if( args[c].compare("--debug" ) == 0 || args[c].compare("-d") == 0 ) {
                debug = true;
				console=new CL_ConsoleWindow("Trophy debugging console");
//				console->redirect_stdio(); TODO
				
                std::cout << "Debug mode on" << std::endl;
            } 
            else if( args[c].compare("--trackinfo") ==0 ) {
                trackInfo = true;
            }
            else if( args[c].compare("--help") == 0 || args[c].compare("-h") == 0 )
            {
                std::cout << "Usage: " << args[0].c_str() << "[options]" << std::endl;
                std::cout << "Options :" << std::endl;
                std::cout << "--debug, -d : active debugging information" << std::endl;
                std::cout << "--trackinfo"  << std::endl;
                std::cout << "--help, -h : show this help message" << std::endl;
                std::cout << "--bigmoney : start with $75,000" << std::endl;
                std::cout << "--fullscreen, -f : fullscreen" << std::endl;
                std::cout << "--fast" << std::endl;
                std::cout << "--640x480, --800x600, --1024x768 : use corresponding resolution" << std::endl;
                std::cout << "--nosound" << std::endl;
                std::cout << "--server" << std::endl;
                std::cout << "--client" << std::endl;
                std::cout << "--ip" << std::endl;
                exit(0);
            }

            else if( !strcmp(args[c].c_str(), "--bigmoney" ) )
            {
                m_cheatMoney = true;
            }

            // Graphic options:
            //
            else if( !strcmp(args[c].c_str(), "--fullscreen" ) || !strcmp(args[c].c_str(), "-f") ) {
                fullScreen = true;
            } else if( !strcmp(args[c].c_str(), "--fast" ) ) {
                fast = true;
            } else if( !strcmp(args[c].c_str(), "--640x480" ) ) {
                resolution = "640x480";
            } else if( !strcmp(args[c].c_str(), "--800x600" ) ) {
                resolution = "800x600";
            } else if( !strcmp(args[c].c_str(), "--1024x768" ) ) {
                resolution = "1024x768";
            }

            // Audio options:
            //
            else if( !strcmp(args[c].c_str(), "--nosound" ) ) {
                sound = false;
            }

            // Network options:
            //
            else if( !strcmp(args[c].c_str(), "--server" ) ) {
                server = true;
            } else if( !strcmp(args[c].c_str(), "--client" ) ) {
                client = true;
            } else if( !strcmp( args[c].c_str(), "-i" ) || !strcmp( args[c].c_str(), "--ip" ) ) {
                if( c<args.size()-1 ){
                  strcpy( serverIp, args[c+1].c_str() );
                  c++;
                }
            }
        }

        CL_SetupDisplay setup_display;
        CL_SetupGL setup_gl;
        CL_SetupGL1 setup_gl1;
        CL_SetupSWRender setup_sw;
        setup_gl.set_current();

        // Some layout things:
        //
        panelWidth = 120;


        // Init sound:
        //
        CL_SetupSound setup_sound;
        sound_output = CL_SoundOutput(44100);

        try {
          reconfigure();
        } catch (CL_Exception e) {
          std::cout << "GL2 is not supported lets try GL1." << std::endl;
          setup_gl1.set_current();
          try {
            reconfigure();
          } catch (CL_Exception e) {
            std::cout << "GL1 is not supported lets use software rendering (the slowest)." << std::endl;
            setup_sw.set_current();
            reconfigure();
          }
        }

        headerHeight = (int)((float)height/4.2f);

        // Load resources:
        //
        CAResources::getResources();
        CASoundEffect::playBackgroundMelody();

        //priceList.setList( "$750~$3000~$12000" );
        offsetX = offsetY = 0;

        // Init everything:
        //
       
        initUpgrades();
        loading.setProgress( 60 );
        initCarTypes();
        initGoodies();
        initPlayers();
        // initNetwork();
        // initPanel();
        initTrackList();

        // Create user data if needed
#ifndef WIN32
        m_homedir = std::string(getenv("HOME"));
#else
        m_homedir = std::string(getenv("APPDATA")); // For windows
#endif
        m_homedir = m_homedir + "/.trophy/";
            
        CL_DirectoryScanner checker;
        if (checker.scan(m_homedir))
        {
            if (debug) std::cout << m_homedir << " exists" << std::endl;
        }
        else
        {
            if (debug) std::cout << m_homedir << " doesn't exist" << std::endl;
            CL_Directory::create(m_homedir);
        }
            

        loading.end();

        // Run:
        //
        runMenu();

        // Deinit everything:
        //
        delete m_track;
        // deinitPanel();
        // deinitNetwork();
        deinitPlayers();
        deinitGoodies();
        deinitUpgrades();
        deinitCarTypes();
        delete m_ConfigureKey;

        // TODO : The only way I found to restore properly the resolution before exiting
//        if(fullScreen) display_window->set_windowed();
    } 
    catch (CL_Exception err) 
    {
        std::cout << "An error occured: " << err.get_message_and_stack_trace().c_str() << std::endl;
    }

    if (console)
    {
            std::cout << "Press a key to exit" << std::endl;
            console->wait_for_key();
            delete console;
            console=0;
    }
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

    std::string mainPath = "cars/";
    carType.push_back(CarType(mainPath+"vw/", CA_RES->resources, carUp, debug));
    carType.push_back(CarType(mainPath+"jeep/", CA_RES->resources, carUp, debug));
    carType.push_back(CarType(mainPath+"z3/", CA_RES->resources, carUp, debug));
    carType.push_back(CarType(mainPath+"flash2/", CA_RES->resources, carUp, debug));   

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
        carType[i].surface = CL_Sprite();
        carType[i].surface3d = CL_Image();
    }

    if(debug) std::cout << "deinitCarTypes end" << std::endl;
}

/** Initializes car upgrades
*/
void CATrophy::initUpgrades()
{
     if(debug) std::cout << "initUpgrades begin" << std::endl;
     carUp = new CACarUpgrades();
     if(debug) std::cout << "initUpgrades end" << std::endl;
}

/** Deinitializes car upgrades
*/
void CATrophy::deinitUpgrades() 
{
    if(debug) std::cout << "deinitUpgrades begin" << std::endl;
    delete carUp;
    carUp = 0;
    if(debug) std::cout << "deinitUpgrades end" << std::endl;
}


/** Initializes goody types and instances.
*/
void
CATrophy::initGoodies() 
{
    goodyType.push_back(new CAGoodyTypeDerived<Turbo>        (CA_RES->goody_turbo, 30000, CA_RES->effectGoodyTurbo));
    goodyType.push_back(new CAGoodyTypeDerived<Life>           (CA_RES->goody_life, 30000, CA_RES->effectGoodyLife));
    goodyType.push_back(new CAGoodyTypeDerived<Money>       (CA_RES->goody_money, 15000, CA_RES->effectGoodyMoney));
    goodyType.push_back(new CAGoodyTypeDerived<Bullets>       (CA_RES->goody_bullets, 30000, CA_RES->effectGoodyBullets));
    goodyType.push_back(new CAGoodyTypeDerived<FogBombs> (CA_RES->goody_fogbomb, 10000, CA_RES->effectGoodyFogbomb));
    
    // Create goody instances:
    //
    for( unsigned int gt=0; gt<goodyType.size(); gt++ )
    {
        std::vector<CAGoody*> vecGoody;
        goody.push_back(vecGoody);
        for( int gi=0; gi<CA_NUMGOODIES; gi++ )
        {
            goody[gt].push_back(new CAGoody( goodyType[gt] ));
        }
    }
}

/** Deinitializes goody instances.
*/
void
CATrophy::deinitGoodies() 
{
    for( unsigned int gt=0; gt<goody.size(); gt++ )
        for( unsigned int gi=0; gi<goody[gt].size(); gi++ )
           if( goody[gt][gi] )
           {
               delete goody[gt][gi];
               goody[gt][gi] = 0;
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


        loading.setProgress( 60.0 + 40.0/CA_MAXPLAYERS*pl );
    }

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
    std::istringstream iss ( CA_RES->resources.get_string_resource("tracks/directories","").c_str() );
    std::string temp;
    while (std::getline(iss,temp, '~'))
       trackList.push_back(temp);
}

/** Initializes track with given name.
*/
void
CATrophy::initRace( const std::string& trackName ) 
{
    loading.begin();

    if(debug) std::cout << "Init track begin" << std::endl;

    m_track = new Track(trackName, debug);

    loading.setProgress( 40 );

    std::vector<int> possiblesRn;
    for( int i=0; i<CA_RACEMAXPLAYERS; i++ )
    {
         possiblesRn.push_back(i);
    }

    for( int pl=0; pl<CA_RACEMAXPLAYERS; pl++ )
    {
        int rn;
        int pos = TrophyMath::getRandomNumber( 0, possiblesRn.size() -1);
        rn = possiblesRn[pos];
        possiblesRn.erase(possiblesRn.begin()+pos);
        m_RacePlayer[pl]->resetForRace( rn, m_track);
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
    CAPositionTable::getPositionTable()->resetRace();

    if(debug) std::cout << "Init track end" << std::endl;
}

/** Reconfigures parameters. Called by the configure menu.
*/
void
CATrophy::reconfigure() 
{
    sound_output.set_global_volume(volume/(float)10);
    std::size_t pos = resolution.find('x');
    std::istringstream issW (resolution.substr(0, pos));
    issW >> width;
    std::istringstream issH (resolution.substr(pos+1));
    issH >> height;
    CL_DisplayWindowDescription desc;
    desc.set_title(get_title());
    desc.set_size(CL_Size(width,height),true);
    if (fullScreen)
    {
      desc.set_fullscreen(true);
      desc.set_decorations(false);
    }
    else
    {
      desc.set_allow_resize(true);
    }
    if (display_window) delete display_window;
    display_window = new CL_DisplayWindow ( desc );
    input_context = display_window->get_ic();
    graphicContext = &display_window->get_gc();
    keyboard = display_window->get_ic().get_keyboard();
    // Init mouse cursor:
    //
    /*
    if( debug )
        input_context.get_mouse().show();
    else
        input_context.get_mouse().hide();
        */
}

/** Resets all goodies. Done before a new race.
*/
void
CATrophy::resetGoodies() {
    for( unsigned int gt=0; gt<goody.size(); gt++ ) {
        for( unsigned int gi=0; gi<goody[gt].size(); gi++ ) {
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
CATrophy::resetPlayers()
{
    for( int pl=1; pl<CA_MAXPLAYERS; ++pl )
    {
        player[pl]->reset();

        int sat = TrophyMath::getRandomNumber( -90, 20 );
        int val = TrophyMath::getRandomNumber( -60, 0 );
        player[pl]->setColor( HSVColor((int)((float)pl/CA_MAXPLAYERS*360),sat,val) );
		
        player[pl]->setTotalPoints(pl*4+pl*3);
        if (pl > 3*CA_MAXPLAYERS/4)
        {
            player[pl]->setCarNumber( 3 );
            player[pl]->getCar()->getTires()->setCurrent(2); // Use value of original 1.1.5 version
            
        }
        else if (pl > CA_MAXPLAYERS/2)
        {
             player[pl]->setCarNumber( 2 );
             player[pl]->getCar()->getTires()->setCurrent(3); // Use value of original 1.1.5 version
        }
        else if (pl > CA_MAXPLAYERS/4)
        {
            player[pl]->setCarNumber( 1 );
            player[pl]->getCar()->getTires()->setCurrent(4); // Use value of original 1.1.5 version
        }
        else
        {
            player[pl]->setCarNumber( 0 );
            player[pl]->getCar()->getTires()->setCurrent(3); // Use value of original 1.1.5 version
        }

    }
    player[0]->reset();
    player[0]->setCarNumber(0); // human player always start with the worst car
	player[0]->setName( "Andrew" );
    if( CA_MAXPLAYERS>1 ) player[1]->setName( "Greenhorn" );
    if( CA_MAXPLAYERS>2 ) player[2]->setName( "Dark Rider" );
    if( CA_MAXPLAYERS>3 ) player[3]->setName( "Speedy Joe" );
    if( CA_MAXPLAYERS>4 ) player[4]->setName( "Sunnyboy" );
    if( CA_MAXPLAYERS>5 ) player[5]->setName( "Jane Zaine" );
    if( CA_MAXPLAYERS>6 ) player[6]->setName( "John Doe" );
    if( CA_MAXPLAYERS>7 ) player[7]->setName( "Sky Carter" );
    if( CA_MAXPLAYERS>8 ) player[8]->setName( "Billy Kid" );
    if( CA_MAXPLAYERS>9 ) player[9]->setName( "Echotango" );
    if( CA_MAXPLAYERS>10 ) player[10]->setName( "Cletus Fuego" );
    if( CA_MAXPLAYERS>11 ) player[11]->setName( "Crazy Slider" );
    if( CA_MAXPLAYERS>12 ) player[12]->setName( "Stream Jet" );
    if( CA_MAXPLAYERS>13 ) player[13]->setName( "Firestorm" );
    if( CA_MAXPLAYERS>14 ) player[14]->setName( "Old Beetle" );
    if( CA_MAXPLAYERS>15 ) player[15]->setName( "Blaze Shaw" );
    if( CA_MAXPLAYERS>16 ) player[16]->setName( "Bonnie T." );
    if( CA_MAXPLAYERS>17 ) player[17]->setName( "Freeze Bee" );
    if( CA_MAXPLAYERS>18 ) player[18]->setName( "Double Tee" );
    if( CA_MAXPLAYERS>19 ) player[19]->setName( "Harkness" );
    if( CA_MAXPLAYERS>20 ) player[20]->setName( "O'Brian" );
    if( CA_MAXPLAYERS>21 ) player[21]->setName( "Holly Wood" );
    if( CA_MAXPLAYERS>22 ) player[22]->setName( "Toshiko Ito" );
    if( CA_MAXPLAYERS>23 ) player[23]->setName( "White Ghost" );
    if( CA_MAXPLAYERS>24 ) player[24]->setName( "Li Mei Zhao" );
    if( CA_MAXPLAYERS>25 ) player[25]->setName( "Furious Bird" );
    if( CA_MAXPLAYERS>26 ) player[26]->setName( "Mister X" );
    if( CA_MAXPLAYERS>27 ) player[27]->setName( "Longhorn" );
    if( CA_MAXPLAYERS>28 ) player[28]->setName( "Fast Tracker" );
    if( CA_MAXPLAYERS>29 ) player[29]->setName( "Bill Speed" );
    if( CA_MAXPLAYERS>30 ) player[30]->setName( "Thunderspot" );
    if (m_cheatMoney)
    {
        player[0]->addMoney (74000);
    }
}

/** Called by CATrophy::main() to start the menu in an endless loop.
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
            {
                int nextCursor = 0;
                do 
                {
                    raceMenu = new CAMenu( "Start Racing" );
                    if (m_isGameStarted)
                        raceMenu->addMenuLabel( "Continue Current Game" );
                    else
                        raceMenu->addMenuLabel( "Start A New Game" );
                    raceMenu->addMenuLabel( "Load Game" );
                    raceMenu->addMenuLabel( "Save Game" );
                    // TODO : Previous Menu ? End Current Game ? Which one is best ?
                    //  - Previous Menu seems more logic, but it doesn't make it very clear that you lose your game (maybe when there will be save/load ?)
                    //  - End Current Game doesn't tell us that we're going back to the main menu
                    raceMenu->addMenuLabel( "End Current Game" );
                    //raceMenu->addMenuLabel( "Previous Menu" );
                        raceMenu->setCursor (nextCursor);
                    raceMenu->run();
                    raceMenuSelection = raceMenu->getSelection();
                    delete raceMenu;

                    switch( raceMenuSelection ) 
                    {
                        case 0:  // Start new game / Continue game
                        {
                            nextCursor = 0;
                            if (m_isGameStarted)
                                gameLoop();
                            else
                                startNewGame();
                        }
                        break;

                        case 1:
                        {
                            nextCursor = 0;
                            if (loadGame())
                            {
                                gameLoop();
                            }
                        }   
                        break;

                        case 2:
                        {
                            if (m_isGameStarted)
                            {
                                nextCursor = 0;
                                if (saveGame())
                                    gameLoop();
                            }
                            else
                            {
                                nextCursor = 2;
                                if( CA_APP->sound ) CA_RES->effectHorn->play( 2 );
                            }
                            
                        }
                        break;

                        case 3:// End current Game (if the number change, raceMenuSelection comparison should also change)
                        {
                            nextCursor = 3;
                            m_isGameStarted = false;
                            m_currentTrackNumbers.clear();
                        }    
                        break;

                        default:
                        {
                            nextCursor = 3;
                        }
                        break;
                    }
                }
                while( raceMenuSelection != 3 );
            }
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
            configMenu.addMenuSelect( "Resolution", "640x480~800x600~1024x768", &CA_APP->resolution);
            configMenu.addMenuSelect( "Sound", "off~on", &CA_APP->sound );
            configMenu.addMenuSelect( "Sound volume", "0%~10%~20%~30%~40%~50%~60%~70%~80%~90%~100%", &CA_APP->volume );
            configMenu.addMenuLabel( "Configure Keyboard" );
            configMenu.addMenuLabel( "Previous Menu" );
            configMenu.setConfigureMenu( true );
            configMenu.run();
            if (configMenu.getSelection() == 4)
            {
                m_ConfigureKey->run();           
            }
        }
            break;


            // Credits:
            //
        case 2:
            runCreditsScreen();
            break;


            // Exit:
            //
        case 3:
            done=true;
            break;
        }
    } while( !done );
}

/** Called by CATrophy::runMenu() to start the position table in an endless loop.
    \return true on success (User pressed Enter or Space) otherwise false (User pressed ESC)
*/
bool
CATrophy::runPositionTable()
{
    CAPositionTableView positionTableView;
    return positionTableView.run();
}

/** Called by CATrophy::runMenu() to start the credits screen in an std::endless loop.
*/
void
CATrophy::runCreditsScreen()
{
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

    resetPlayers();

    // Player settings dialog:
    //
    CAPlayerSettingsDialog playerSettingsDialog;
    if( playerSettingsDialog.run()==1 ) 
    {
        m_nbTurns = 0;
        player[0]->setName( playerSettingsDialog.getPlayerName() );
        player[0]->setColor( HSVColor( playerSettingsDialog.getPlayerHue(),0,0 ) );

        // Choose difficulty:
        //
		CAMenu* difficultyMenu = new CAMenu( "Select Difficulty:" );
		difficultyMenu->addMenuLabel( "Speed makes me dizzy" );
		difficultyMenu->addMenuLabel( "I live to ride" );
		difficultyMenu->addMenuLabel( "Petrol in my veins" );
		difficultyMenu->setCursor( 1 );

        int dif = difficultyMenu->run();
        delete difficultyMenu;
        if( dif >= 0 ) 
        {
            difficulty = Difficulty (dif);
            gameLoop();
        }
    }

    if( debug ) std::cout << "startNewGame() end" << std::endl;
}


/** Starts a new game. Called by the menu.
*/
bool
CATrophy::saveGame() 
{
    bool isOk = false;
    if( debug ) std::cout << "saveGame() begin" << std::endl;
    CASlotSelectionDialog slotDialog("Save Game", CA_RES->font_normal_14_white, m_homedir);
    std::string saveFileName = slotDialog.display();
    if (saveFileName == "")
        return isOk; // TODO: handle this (or not)      
    std::string saveFileString = m_homedir + saveFileName;
    if (debug) std::cout << "saving " << saveFileString << std::endl;
    std::ofstream saveFile(saveFileString.c_str());
    if (saveFile)
    {
        saveFile << VERSION << std::endl;
        saveFile << difficulty << std::endl;
        saveFile << m_nbTurns << std::endl;
        if (m_currentTrackNumbers.size() != 3)
        {
            for (unsigned int i=0; i<3; ++i)
                saveFile << -1 << std::endl;
        }
        else 
        {
            for (unsigned int i=0; i<3; ++i)
                saveFile << m_currentTrackNumbers[i] << std::endl;
        }
        // TODO: This code should probably be in player.cpp
        for( unsigned int c=0; c<CA_MAXPLAYERS; ++c )
        {
            saveFile << player[c]->getName() << std::endl;
            saveFile << player[c]->getCarNumber() << std::endl;
            saveFile << player[c]->getCar()->getMotor()->getCurrent() << std::endl;
            saveFile << player[c]->getCar()->getTires()->getCurrent() << std::endl;
            saveFile << player[c]->getCar()->getArmor()->getCurrent() << std::endl;
            saveFile << player[c]->getColor().h << std::endl;
            saveFile << player[c]->getColor().s << std::endl;
            saveFile << player[c]->getColor().v << std::endl;
            saveFile << player[c]->getMoney() << std::endl;
            saveFile << player[c]->getTotalPoints() << std::endl;
        }
        isOk = true;
        CAInfoDialog saveOKDlg( "Game Saved",
                               "Press Enter to continue...",
                               CAInfoDialog::Info,
                               false, &slotDialog );
        saveOKDlg.run();
    }
    if( debug ) std::cout << "saveGame() end" << std::endl;
    return isOk;
}

/** Starts a new game. Called by the menu.
*/
bool
CATrophy::loadGame() 
{
    if( debug ) std::cout << "LoadGame() begin" << std::endl;
    bool isOk = false;

    CASlotSelectionDialog slotDialog("load Game", CA_RES->font_normal_14_white, m_homedir);
    std::string loadFileName = slotDialog.display();

    if (loadFileName == "")
    {
        if (!slotDialog.canceled())
            std::cerr << "Unable to load Game. File corrupted?" << std::endl; // TODO
        return isOk; // TODO: handle this 
    }
    
    std::string loadFileString = m_homedir + loadFileName;
    if (debug) std::cout << "loading " << loadFileString << std::endl;
    
    std::ifstream loadFile(loadFileString.c_str());
    if (loadFile)
    {
        std::string version;
        loadFile >> version;

        int dif;
        loadFile >> dif;
        if (dif == 0)
            difficulty = Easy;
        else if (dif == 1)
            difficulty = Medium;
        else if (dif == 2)
            difficulty = Hard;

        loadFile >> m_nbTurns;

        m_currentTrackNumbers.clear();
        for (int i=0; i<3; ++i)
        {
            int trackNumber;
            loadFile >> trackNumber;
            if (trackNumber != -1)
                m_currentTrackNumbers.push_back(trackNumber);
        }

        // TODO: This code should probably be in player.cpp
        for( unsigned int c=0; c<CA_MAXPLAYERS; ++c )
        {
            std::string name;
            std::getline( loadFile, name ); // end of line
            std::getline( loadFile, name ); // name
            int carNumber, currentMotor, currentTires, currentArmor, h, s, v, money, totalPoints;
            loadFile >> carNumber >> currentMotor >> currentTires >> currentArmor >> h >> s >> v >> money >> totalPoints;
            player[c]->setName(name);
            player[c]->setCarNumber(carNumber);
            player[c]->getCar()->getMotor()->setCurrent(currentMotor);
            player[c]->getCar()->getTires()->setCurrent(currentTires);
            player[c]->getCar()->getArmor()->setCurrent(currentArmor);
            HSVColor color(h, s, v);
            player[c]->setColor(color);
            player[c]->setMoney(money);
            player[c]->setTotalPoints(totalPoints);
        }
        isOk = true;
        CAInfoDialog loadOKDlg( "Game Loaded",
                               "Press Enter to continue...",
                               CAInfoDialog::Info,
                               false, &slotDialog );
        loadOKDlg.run();
    }
    if( debug ) std::cout << "loadGame() end" << std::endl;
    return isOk;
}


/** Main game Loop. Called by startNewGame and LoadGame
*/
void CATrophy::gameLoop()
{
    if( debug ) std::cout << "gameLoop() begin" << std::endl;
    m_isGameStarted = true;
    // Choose track:
    //
    int trackNumber = 1;

    bool goOn = true;
    std::vector<std::vector<Player*> > allRunningPlayers;

    while (goOn)
    {
        if (m_gameLoopState == 0)
        {
            if (m_nbTurns == 0)
                ++m_gameLoopState; // we don't use Shop at First Race
            else
            {
                ShopScreen myShop(player[0], CA_RES->menu_bg, CA_RES->gui_button, CA_RES->font_normal_14_white, carUp);
                myShop.run();
                if (myShop.canceled())
                {
                    goOn = false;
                }
                else
                ++m_gameLoopState;
            }
        }
        if (m_gameLoopState == 1)
        {
            CASignUpScreen signUpScreen(player, m_currentTrackNumbers);
            trackNumber = signUpScreen.run();
            m_currentTrackNumbers = signUpScreen.getTrackNumbers();
            if (signUpScreen.canceled())
            {
                if (m_nbTurns == 0)
                    goOn = false;
                else
                    --m_gameLoopState;
                
            }
            else
            {
                RaceLevel raceLevel = RaceLevel(signUpScreen.getRaceLevel());
                m_RacePlayer = signUpScreen.getRacePlayers();

                if( trackNumber != -1 ) 
                {
                    CAPositionTable::getPositionTable()->setRaceLevel(raceLevel);
                    m_trackName = trackList[trackNumber];
                    run(); // This is where the race start
                    m_currentTrackNumbers.clear();
                    runPositionTable();
                    signUpScreen.addVirtualPoints();
                    allRunningPlayers = signUpScreen.getAllRunningPlayers();
                    ++m_gameLoopState;
                }
                else
                    std::cerr << "Internal Error" << std::endl; // TODO: Check if it is really impossible
            }
        }
        if (m_gameLoopState == 2)
        {
            CAChampionshipScreen myChampionShip(player[0], player, allRunningPlayers, CA_RES->menu_bg, CA_RES->gui_button, CA_RES->gui_button_green,  CA_RES->gui_button_blue, CA_RES->gui_button_red, CA_RES->font_normal_11_white);
            if (myChampionShip.run())
                ++m_gameLoopState;
            else
            {
                WinDialog win(player[0]->getName(), m_nbTurns);
                win.run();
                ++m_gameLoopState;
                goOn = false;
            }
                
        }

        m_gameLoopState = m_gameLoopState%3;
    }
    if( debug ) std::cout << "gameLoop() end" << std::endl;
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
    game in an endless loop. this is the endless loop of the race
*/
int
CATrophy::run() 
{
    if(debug) std::cout << "Game Running" << std::endl;
    m_nbTurns++;

    int  gameStartTime;  // Race started
    int  goodyTime;         // Last goody placed at...
    int  rhythm = 0;        // Rhythm for networking.
    // Keep up Server on 0, 2, ...
    // Keep up Client on 1, 3, ...
    int  c;                 // Counter
    bool raceOver = false;  // Set to true if race is over
    int  raceOverTime = 0;  // Time stamp at the time when the race was over.

    // Init track and choose player for race
    //
    initRace( m_trackName );

    // Init the panel
    //
    initPanel();

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
	bool isEscapePressed = false;
    while( !isEscapePressed &&
            (!raceOver || CL_System::get_time()-raceOverTime<3000) ) 
    {
        measureFrameTime( true );

        if( input_context.get_keyboard().get_keycode(CL_KEY_P) )
        {
            pause = !pause;
        }

        if( lightState==3 && !pause ) 
        {
            // Control players:
            //
            raceOver = true;
            for( c=0; c<CA_RACEMAXPLAYERS; ++c )
            {
                m_RacePlayer[c]->pilot();

                if( m_RacePlayer[c]->hasFinished() )
                {       // Check if first player has finished
                    firstPlayerFinished = true;
                }
                else if( c==0 && !m_RacePlayer[c]->isDeath() && !m_RacePlayer[c]->isLapped() )
                {  // Check if race is over...
                    raceOver=false;
                }
            }
            if(raceOver && raceOverTime==0) raceOverTime = CL_System::get_time();

            // 'Advance' goodies:
            //
            for( unsigned int gt=0; gt<goody.size(); gt++ )
            {
                for( unsigned int gi=0; gi<goody[gt].size(); gi++ )
                {
                    goody[gt][gi]->advance();
                }
            }

            // Advance players:
            //
            for(c=0; c<CA_RACEMAXPLAYERS; ++c)
            {
                m_RacePlayer[c]->advance();
            }

            // Place goodies:
            //
            if( CL_System::get_time() > (unsigned int)(goodyTime+CA_GOODYTIME) )
            {
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
       
       if( debug && trackInfo ) // TODO: trackInfo should be in Track
        {
             m_track->handleTrackCreation(offsetX, offsetY);
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
        if( !allowShooting && (CL_System::get_time()-gameStartTime) > 3000 )
        {
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

        display_window->flip();   // Copy framebufer to screen
        CL_KeepAlive::process();      // VERY VITAL for the system!

        measureFrameTime( false );
		isEscapePressed = input_context.get_keyboard().get_keycode(CL_KEY_ESCAPE);

        if (isEscapePressed)
        {
            CAEscapeDialog escapeDlg( "Quit Race?",
                                     "Press Y to quit race",
                                     CAInfoDialog::Info,
                                     true, this );
            escapeDlg.run();

            if( escapeDlg.isMustQuit () )
            {
                m_RacePlayer[0]->kill();
            }
            else
            {
                isEscapePressed = false;
            }
        }
    }

    if( debug ) std::cout << "Stop game loop" << std::endl;

    waitForSilence();

    // Finish all players manually
    //
    for( int rank=1; rank<=CA_RACEMAXPLAYERS; ++rank )
    {
        // Players which are alive:
        //
        for( c=0; c<CA_RACEMAXPLAYERS; ++c )
        {
            if( !m_RacePlayer[c]->hasFinished() &&
                    !m_RacePlayer[c]->isDeath() &&
                    !m_RacePlayer[c]->isLapped() &&
                    m_RacePlayer[c]->getRaceRank()==rank )
            {
                CAPositionTable::getPositionTable()->playerFinishedRace( m_RacePlayer[c] );
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

    //buyCars();
    for( int pl=0; pl<CA_MAXPLAYERS; ++pl )
    {
        player[pl]->OnRaceOver();
    }
    
    fadeScreen( false, this );

    deinitPanel();

    return 0;
}

/** Places a new goody (not always successful)
*/
void
CATrophy::placeGoody() {
    int x[4];    // 4 edge-points of new goody
    int y[2];
    
    int sx = TrophyMath::getRandomNumber( 8, m_track->getWidth()-8 );
    int sy = TrophyMath::getRandomNumber( 8, m_track->getHeight()-8 );
    int gt = TrophyMath::getRandomNumber( 0, goodyType.size()-1 );
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
        for( unsigned int gi=0; gi<goody[gt].size(); gi++ ) {
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
    for( pl=0; pl<CA_RACEMAXPLAYERS; ++pl ) {
        if( !m_RacePlayer[pl]->isDeath() && !m_RacePlayer[pl]->isLapped()) {
            if( m_RacePlayer[pl]->hasFinished() ) {
                if( m_RacePlayer[pl]->getRaceRank()>=rank )
					rank = m_RacePlayer[pl]->getRaceRank()+1;
            } else {
                m_RacePlayer[pl]->setRaceRank(0);
            }
        }
    }

    do {
        pos = 0.0;
        nextRank=-1;
        for( pl=0; pl<CA_RACEMAXPLAYERS; ++pl ) {
            if( m_RacePlayer[pl]->getRaceRank()==0 ) {
                thisPos = m_RacePlayer[pl]->getPosition();
                if( thisPos>pos ) {
                    nextRank = pl;
                    pos = thisPos;
                }
            }
        }

        if( nextRank!=-1 ) {
            if( !m_RacePlayer[nextRank]->hasFinished() && !m_RacePlayer[nextRank]->isDeath() && !m_RacePlayer[nextRank]->isLapped() ) {
                m_RacePlayer[nextRank]->setRaceRank( rank );
            }
        }

        rank++;
    } while( nextRank!=-1 && rank<=CA_RACEMAXPLAYERS );
}

/** Gets the speed limit for a coordinate of the map
*/
int
CATrophy::getSpeedLimit( int x, int y ) 
{
    return m_track->getSpeedLimit(x,y);
}

/** Gets the lap part for a coordinate of the map
*/
int
CATrophy::getLapPart( int x, int y ) 
{
    return m_track->getLapPart(x,y);
}

/** Gets the level for a coordinate of the map.
    \return 0: Level on the point is down
            1: Level on the point is up
            2: Level on the point is not defined
*/
int
CATrophy::getLevel( int x, int y ) 
{
    return m_track->getLevel(x, y);
}

/** Checks if the gifen coordinate is on the map and returns true if so.
*/
bool
CATrophy::checkCoordinate( int x, int y ) 
{
    return m_track->checkCoordinate(x,y);
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

    if( start )
    {
        frameStart = CL_System::get_time();
    }
    else
    {
        int timeElapsed = CL_System::get_time()-frameStart;
        if( timeElapsed>0 ) framesPerSec = 1000.0 / timeElapsed;
        if( framesPerSec<10.0 ) framesPerSec = 10.0;
    }
}

/** Waits until no key is pressed.
*/
void
CATrophy::waitForSilence() {
    bool done;
    do {
        done = true;
        if( input_context.get_keyboard().get_keycode( CL_KEY_ENTER ) ||
                input_context.get_keyboard().get_keycode( CL_KEY_ESCAPE ) ) {
            done = false;
        }
        CL_KeepAlive::process();      // VERY VITAL for the system!
    }
    while( !done );
}

/** Distributes cars to players which have enough money.
*/
/*void
CATrophy::buyCars() {
    // Players with a lot of money get new cars:
    //
    if( debug ) std::cout << "Buy new cars begin" << std::endl;

    for( int pl=0; pl<CA_MAXPLAYERS; ++pl ) {
        player[pl]->OnRaceOver();
    }

    if( debug ) std::cout << "Buy new cars end" << std::endl;
}*/

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
    graphicContext->set_cliprect(crField);

    m_track->displayMap(offsetX,offsetY); 

    // Things under bridge:
    displayGoodies( false );
    displayFogBombs( false, true );
    displayDustClowds( false );
    displayPlayers( false );
    displayFogBombs( false, false );

    // Bridge:
    m_track->displayBridge(offsetX, offsetY);

    // Things on bridge:
    displayGoodies( true );
    displayFogBombs( true, true );
    displayDustClowds( true );
    displayPlayers( true );
    displayFogBombs( true, false );

    // Display tracks for debugging:
    //
    if( trackInfo ) 
        m_track->displayTrackPoints(offsetX, offsetY);

    displayStartingLights();
    displayCheckFlag();

    // Do the panel:
    //
    graphicContext->set_cliprect(crPanel);
    panel->display();
    graphicContext->set_cliprect(crAll);
}


/** Displays the goodies.
*/
void
CATrophy::displayGoodies( bool up ) 
{
    for( unsigned int gt=0; gt<goody.size(); gt++ )
        for( unsigned int gi=0; gi<goody[gt].size(); gi++ )
            if( goody[gt][gi]->isUp()==up ) goody[gt][gi]->display( offsetX, offsetY );
}


/** Displays players.
    \param up true: Players on bridges / false: Players under bridges
*/
void
CATrophy::displayPlayers( bool up ) {
    for( int c=0; c<CA_RACEMAXPLAYERS; ++c) {
        if( m_RacePlayer[c]->isUp()==up ) m_RacePlayer[c]->display( offsetX, offsetY );
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
                    for( int pl=0; pl<CA_RACEMAXPLAYERS; ++pl) {
                        int dist = (int)TrophyMath::getDistance( fogBomb[c].x, fogBomb[c].y, m_RacePlayer[pl]->getX(), m_RacePlayer[pl]->getY() );
                        if( m_RacePlayer[pl]->isUp()==up && dist<90 ) {
                            m_RacePlayer[pl]->hit( 30.0-(dist/3.0) );
                        }
                    }
                }

                CA_RES->misc_fog.set_frame((int)fogBomb[c].frame);
                CA_RES->misc_fog.draw (*graphicContext, fogBomb[c].x + offsetX - CA_RES->misc_fog.get_width()/2,
                                              fogBomb[c].y + offsetY - CA_RES->misc_fog.get_height()/2);
                if( CA_RES->advanceAnimation( &fogBomb[c].frame, CA_RES->misc_fog.get_frame_count(), 6.0 ) )
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
            x = dustClowd[c].x + offsetX - CA_RES->misc_dust.get_width()/2;
            y = dustClowd[c].y + offsetY - CA_RES->misc_dust.get_height()/2;

            if( frame >= 0 &&
                    frame < (int)CA_RES->misc_dust.get_frame_count() &&
                    checkCoordinate( x,y ) ) 
            {
                CA_RES->misc_dust.set_frame(frame);
                CA_RES->misc_dust.draw (*graphicContext, x, y);
            }

            if( CA_RES->advanceAnimation( &dustClowd[c].frame, CA_RES->misc_dust.get_frame_count(), 8.0 ) ) 
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
    static int flagPos = -CA_RES->misc_checkflag.get_height();
    if( firstPlayerFinished ) 
    {
        CA_RES->misc_checkflag.draw (*graphicContext, width-CA_RES->misc_checkflag.get_width()-32,
                                            //panelWidth+(width-panelWidth-CA_RES->misc_checkflag->get_width())/2,
                                            flagPos);
        if( flagPos<32 ) flagPos+=2;

        CA_RES->misc_checkflag.update();
    }
}

/** Displays the starting light animation.
*/
void
CATrophy::displayStartingLights() {
    static int lightPos = 32;
    if( lightState<4 ) {
        if( lightState==3 ) {
            if( lightPos > (-(int)CA_RES->misc_light.get_height()) ) {
                lightPos-=2;
            }
        } else {
            lightPos = 0;
        }

        CA_RES->misc_light.set_frame(lightState);
        CA_RES->misc_light.draw (*graphicContext,width-CA_RES->misc_light.get_width()-32,
                                        //panelWidth+(width-panelWidth-CA_RES->misc_light->get_width())/2,
                                        lightPos);
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
                CL_Draw::fill( *graphicContext, CL_Rectf(0, y-tb, width, y+tb), CL_Colorf(0, 0, 0, 255) );
            tb = ( in ? tb-1 : tb+1 );
        }

        display_window->flip();
        CL_KeepAlive::process();
    }

    if( debug ) std::cout << "fadeScreen end" << std::endl;
}

/** Scrolls the map to center player 0.
*/
void CATrophy::scroll()
{
    m_track->scroll(offsetX, offsetY, (int)(player[0]->getX()), (int)(player[0]->getY()), width, height, panelWidth);
}

class Program
{
public:
  static int main(const std::vector<CL_String> &args)
  {
    CL_SetupCore setup_core;

    CATrophy ca_trophy;
    application = &ca_trophy;
    ca_trophy.start(args);
    return 0;
  }
};

CL_ClanApplication app(&Program::main);

// EOF
