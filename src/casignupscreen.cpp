#include "casignupscreen.h"
#include "catrophy.h"
#include "player.h"
#include "upgradespanel.h"
#include "caimageview.h"
#include "cachampionshipscreen.h" // For RankPredicate
#include <vector>
#include <algorithm>

struct FindPlayer:public std::binary_function<std::vector<Player*>, Player*, bool>
{
    bool operator()(std::vector<Player*>& racePlayers, Player* player) const
    {
       std::vector<Player*>::const_iterator it = std::find(racePlayers.begin(), racePlayers.end(), player);
       return (it != racePlayers.end());
    }
};

/** Constructor.
*/
CASignUpScreen::CASignUpScreen(std::vector<Player*> player, std::vector<int> trackNumbers)
 : CAScreen(),
    m_Player(player)
{
    cursor = 0;
    offset = 0;
    title = "P R E S S   E N T E R   T O   S I G N   U P";
    help = "Use Arrow Keys to change selection and press Enter to\nconfirm";
    m_selected = false;

    left = (CA_APP->width - 640)/2;
    right = CA_APP->width - left;
    top = CA_APP->headerHeight + 30;
    bottom = top + 400;

    numTracks = CA_APP->trackList.size();

    for( int i=0; i<3; ++i )
    {
        image[i] = CL_Image();
        racePreview[i] = 0;
    }

    //setOffset( 0 );
    std::sort(m_Player.begin(), m_Player.end(), RankPredicate()); // We sort player by points
    int beginRandom = m_Player.size()/2;
    int endRandom = m_Player.size() -1;
    for (int race=0; race<3; race++)
    {
        int rn = 0;
        if (trackNumbers.size() == 3)
            rn =  trackNumbers[race];
        else
        {
            std::vector<std::string>::const_iterator it = m_trackList.begin();
            do
            {
                rn = TrophyMath::getRandomNumber( 0, numTracks-1 );
                it = std::find(m_trackList.begin(), m_trackList.end(), CA_APP->trackList[rn]);
            }
            while (it != m_trackList.end());
        }
        m_trackList.push_back(CA_APP->trackList[rn]);
        m_trackNumbers.push_back(rn);

	    m_RacePlayer.push_back(std::vector<Player*>());
	    m_StringRacePlayer.push_back(std::vector<std::string>());

        for (int pl=0; pl<CA_RACEMAXPLAYERS; pl++)
        {
            std::vector<std::vector<Player*> >::const_iterator it;
            int rn = 0;
            do
            {
                rn = TrophyMath::getRandomNumber( beginRandom, endRandom );
                it = std::find_if(m_RacePlayer.begin(), m_RacePlayer.end(), std::bind2nd(FindPlayer(), m_Player[rn]));
            }
            while ( it != m_RacePlayer.end() || m_Player[rn] == CA_APP->player[0]);
            m_RacePlayer[race].push_back(m_Player[rn]);
            std::ostringstream oss;
            oss << rn+1 << ") " <<  m_Player[rn]->getName();
            m_StringRacePlayer[race].push_back(oss.str());
        }
        beginRandom -= m_Player.size()/4;
        endRandom -= m_Player.size()/4;
    }
    setOffset( 0 );
}

/** Destructor.
*/
CASignUpScreen::~CASignUpScreen() {
    reset();
}

/** Resets/destroys the images and previews.
*/
void
CASignUpScreen::reset() 
{
    for( int i=0; i<3; ++i ) 
    {
        if( racePreview[i] ) {
            delete racePreview[i];
            racePreview[i] = 0;
        }
        if( !image[i].is_null() ) {
            image[i] = CL_Image();
        }
        // CAImageData doesn't delete image[i] !!
    }
}

/** Set new offset for the shown tracks. Offset 2 means
    that tracks 2-4 are shown.
    \param o New Offset.
*/
void
CASignUpScreen::setOffset( int offset ) 
{
    std::string trackPath = "tracks/";

    this->offset = offset;
    reset();

    for( int i=0; i<3; ++i ) 
    {
        if( i<numTracks ) 
        {
            try
            {
                image[i] = CL_Image ( *CA_APP->graphicContext, trackPath + m_trackList[i] + "/thumb.tga" );
                
            }
            catch(CL_Exception err)
            {
                trackPath = "../resources/tracks/";
                image[i] = CL_Image ( *CA_APP->graphicContext, trackPath + m_trackList[i] + "/thumb.tga" );
            }
            std::ostringstream oss;
            oss << "$" << (i==0 ? CA_PRIZE : (i==1 ? CA_PRIZE_MEDIUM : CA_PRIZE_HARD));
            
            //racePreview[i] = new CAImageView( CA_APP->trackList[i+offset], oss.str(), image[i], false );
            racePreview[i] = new CAImageView( m_trackList[i], oss.str(), image[i], false );

            racePreview[i]->setImageSize( 150, 92 );
            racePreview[i]->move( left + 195*i, top + 50 );
        } 
        else 
        {
            image[i] = CL_Image();
            racePreview[i] = 0;
        }
    }
}

/** get the players in the same race of the player)
*/
std::vector<Player*> CASignUpScreen::getRacePlayers()
{
    return m_RacePlayer[cursor];
}

/** get all players running in the 3 races (easy, medium and hard)
*/
std::vector<std::vector<Player*> > CASignUpScreen::getAllRunningPlayers()
{
    return m_RacePlayer;
}


void CASignUpScreen::addVirtualPoints()
{
    for( int i=0; i<3; ++i ) 
    {
        if (i!=cursor)
        {
            int random =TrophyMath::getRandomNumber(0,5);
            unsigned int playerFinished = m_RacePlayer[i].size();
            if (random <= 0)
                playerFinished--;
             if (random <= 1)
                playerFinished--;
            for (unsigned int j=0; j<playerFinished; j++)
                m_RacePlayer[i][j]->setRacePoints (((CA_RACEMAXPLAYERS)-j) * (i +1));
        }
    }
}

/** Runs the screen.
*/
int
CASignUpScreen::run() 
{
    slot = CA_APP->keyboard.sig_key_up().connect(this, &CASignUpScreen::on_key_released);

    CA_APP->fadeScreen( true, this );
    done = false;
    cancel = false;

    // Screen loop:
    //
    while( !done ) 
    {
        CA_APP->measureFrameTime( true );

        buildScreen();

        // Play background sound:
        CASoundEffect::playBackgroundMelody();

        CA_APP->display_window->flip();   // Copy framebuffer to screen
        CL_KeepAlive::process(-1);      // VERY VITAL for the system!

        CA_APP->measureFrameTime( false );
    }

    CA_APP->fadeScreen( false, this );
    CA_APP->waitForSilence();

    //CL_Input::chain_button_release.remove( this );

    std::vector<std::string>::const_iterator it = std::find(CA_APP->trackList.begin(), CA_APP->trackList.end(), m_trackList[cursor]);
    
    //return (cancel ? -1 : cursor+offset);
    return (cancel ? -1 : it-CA_APP->trackList.begin());
}

int CASignUpScreen::getRaceLevel()
{
    return cursor;
}

/** Builds the screen.
*/
void
CASignUpScreen::buildScreen() 
{
    // Counter for cursor animation:
    static float cursorAnim = 0.0;

    // Backgroud:
    //
    CA_RES->menu_bg.draw ( *CA_APP->graphicContext, CL_Rect(0, 0, CA_APP->width, CA_APP->height) );

    // Title / help:
    //
    displayTitle();
    displayHelp();

    // Cursor:
    //
    CL_Draw::fill( *CA_APP->graphicContext, CL_Rectf(racePreview[cursor]->getLeft()-12, racePreview[cursor]->getTop()-12,
                           racePreview[cursor]->getRight()+12, racePreview[cursor]->getBottom()+12),
                           CL_Colorf (255, 216, 84, (int)((cursorAnim/2)*255) ));

    CA_RES->advanceAnimation( &cursorAnim, 1, 2.0, CAResources::Revolving );

    // Race previews:
    //
    for( int race=0; race<3; ++race )
    {
        if( racePreview[race] )
        {
            racePreview[race]->display();
            CA_RES->font_normal_14_white.draw_text( *CA_APP->graphicContext, racePreview[race]->getHCenter(),
                                                racePreview[race]->getTop()-22,
                                                (race==0 ? "Easy" : (race==1 ? "Medium" : "Hard")) );
        }
        if (m_selected == true)
        {
            for (unsigned int pl=0; pl<m_RacePlayer[race].size(); pl++)
            {
                CA_RES->font_normal_14_white.draw_text ( *CA_APP->graphicContext,racePreview[race]->getHCenter(),
                                                    racePreview[race]->getBottom()+22*(pl+1), m_StringRacePlayer[race][pl] );
            }
        }
    }

  
    //
    // UpgradesPanel
    UpgradesPanel uPanel(CA_APP->player[0], CA_RES->font_normal_14_white, CA_RES->font_lcd_13_green, racePreview[2]->getRight() + 32, racePreview[0]->getTop()-22);
    uPanel.display();
    
}

/** Called on key release.
*/
void
CASignUpScreen::on_key_released (const CL_InputEvent &key, const CL_InputState &state) 
{
    switch( key.id ) 
    {
        // Cancel (ESC):
        //
    case CL_KEY_ESCAPE:
        {
            if (!m_selected)
            {
                cancel = true;
                done = true;
            }
            else
                if( CA_APP->sound ) CA_RES->effectHorn->play( 2 ); // do not allow to echap when the user has selected
        }
        break;

        // Left:
        //
    case CL_KEY_LEFT:
    case CL_KEY_N:
        if (!m_selected)
        {
            if( cursor>0 ) {
                cursor--;
                playSwitchSound();
            } /*else if( offset>0 ) {
                setOffset( offset-1 );
                playSwitchSound();
            }*/
        }
        break;

        // Right:
        //
    case CL_KEY_RIGHT:
    case CL_KEY_M:
        if (!m_selected)
        {
            if( cursor<2 && cursor<numTracks-1 ) {
                cursor++;
                playSwitchSound();
            } /*else if( offset<numTracks-3 ) {
                setOffset( offset+1 );
                playSwitchSound();
            }*/
        }
        break;

        // Activate:
        //
    case CL_KEY_ENTER:
    case CL_KEY_SPACE:
        {
            if (m_selected == true)
            {
                done = true;
            }
            else
            {
                    m_RacePlayer[cursor][0] = CA_APP->player[0];
                    std::vector<Player*>::const_iterator it = std::find(m_Player.begin(), m_Player.end(), m_RacePlayer[cursor][0]); // we trying to find the rank of the player
                    std::ostringstream oss;
                    oss << it - m_Player.begin() +1 << ") " <<  (*it)->getName();
                    m_StringRacePlayer[cursor][0] = oss.str();  
                    m_selected = true;
                    help = "press Enter to continue";
            }
        }
        break;

    default:
        break;
    }
}

void
CASignUpScreen::playSwitchSound() {
    if( CA_APP->sound ) CA_RES->effectMenu->play( 2 );
}

std::vector<int>
CASignUpScreen::getTrackNumbers() const
{
    return m_trackNumbers;
}

// EOF
