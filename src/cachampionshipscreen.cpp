#include "cachampionshipscreen.h"
#include "catrophy.h"
#include "player.h"
#include <functional>
#include <algorithm>

bool RankPredicate::operator()(const Player* const p1, const Player* const p2)
   {
       // In case of level, use alphabetical order
       if (p1->getTotalPoints() == p2->getTotalPoints())
          return (p1->getName() < p2->getName()); 
       else
          return (p1->getTotalPoints() > p2->getTotalPoints());
   }


/** Constructor.
*/
CAChampionshipScreen::CAChampionshipScreen(const std::vector<Player*> player, const std::vector<std::vector<Player*> > runningPlayer,
                                                                           CL_Surface* background, CL_Surface* button, CL_Surface* buttonEasy, CL_Surface* buttonMedium, CL_Surface* buttonHard, CL_Font* font)
   : CAScreen(),
   m_player(player),
   m_runningPlayers(runningPlayer),
   m_background(background),
   m_font(font),
   m_button(button),
   m_buttonEasy(buttonEasy),
   m_buttonMedium(buttonMedium),
   m_buttonHard(buttonHard),
   m_displayMode(DISPLAY_ADD_POINTS_EASY)
{
    left = 0;
    right = CA_APP->width;
    top = CA_APP->headerHeight + 30;
    bottom = CA_APP->headerHeight + 400;
    barHeight = m_font->get_height() + 6;
    std::sort(m_player.begin(), m_player.end(), RankPredicate());
    title = "C H A M P I O N S H I P";
}

/** Destructor.
*/
CAChampionshipScreen::~CAChampionshipScreen()
{}



/** Runs the screen.
*/
int
CAChampionshipScreen::run() 
{
    //CL_Input::chain_button_release.push_back( this );
    //slot = CL_Input::sig_button_release.connect(thCreateSlot(this, &CAChampionshipScreen::on_button_release));
    //slot = CL_Input::sig_button_press.connect(this, &CAChampionshipScreen::on_button_release);
    slot = CL_Keyboard::sig_key_up().connect(this, &CAChampionshipScreen::on_key_released);

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

        CL_Display::flip();   // Copy framebufer to screen
        CL_System::keep_alive();      // VERY VITAL for the system!

        CA_APP->measureFrameTime( false );
    }

    CA_APP->fadeScreen( false, this );
    CA_APP->waitForSilence();

    //CL_Input::chain_button_release.remove( this );

    CL_Keyboard::sig_key_up().disconnect(slot);
    return (cancel);
}

/** Builds the screen.
*/
void
CAChampionshipScreen::buildScreen() 
{
    // Background:
    //
    m_background->draw ( CL_Rect(0, 0, CA_APP->width, CA_APP->height) );
    displayTitle();

    CL_Display::fill_rect( CL_Rect(left, top, right, bottom), CL_Color(0, 0, 0, 64) );
    
    const int rankWidth = 30;
    const int rankLeft = 0;

    const int nameWidth = 200;
    const int nameLeft = 50;

    const int addPtWidth = 30;
    const int addPtLeft = 260;

    const int totalPtWidth = 50;
    const int totalPtLeft = 300;

    const int caWidth = (totalPtLeft + totalPtWidth)*2;
    const int middleMargin = 30;
   

    const int caHeight = (barHeight * m_player.size())/2;
    const int marginTop = (top+bottom)/2 - caHeight/2;

    for (unsigned int rank=0; rank < m_player.size(); rank++)
    {
        std::ostringstream ossRankStr, ossTotalPoints, ossRacePoints;
        ossRankStr << rank + 1 << ".";
        ossTotalPoints << m_player[rank]->getTotalPoints();
        int marginLeft = (right+left)/2 + middleMargin/2;
        int upPos = rank - m_player.size()/2; 
        if (rank < m_player.size()/2 )
        {
            marginLeft -= caWidth/2 + middleMargin;
            upPos = rank;
        }
        // Buttons:
        //
        m_button->draw ( CL_Rect(marginLeft+rankLeft, marginTop+barHeight*upPos, marginLeft+rankLeft+rankWidth, marginTop+(barHeight*(upPos+1))) );
        m_button->draw ( CL_Rect(marginLeft+nameLeft, marginTop+barHeight*upPos, marginLeft+nameLeft+nameWidth, marginTop+(barHeight*(upPos+1))) );
        if (m_displayMode != DISPLAY_CHAMPIONSHIP)
        {
            for (int i=0; i<= int(m_displayMode); i++)
            {
                std::vector<Player*>::const_iterator it = std::find(m_runningPlayers[i].begin(), m_runningPlayers[i].end(), m_player[rank]);
                if (it != m_runningPlayers[i].end() && m_player[rank]->getRacePoints() != 0)
                {
                    CL_Surface* buttonToUse = (i==0 ? m_buttonEasy : (i==1)? m_buttonMedium : m_buttonHard); // Choose the button color
                    buttonToUse->draw ( CL_Rect(marginLeft+addPtLeft, marginTop+barHeight*upPos, marginLeft+addPtLeft+addPtWidth, marginTop+(barHeight*(upPos+1))) );
                    ossRacePoints << "+" << m_player[rank]->getRacePoints();
                }
            }
        }
        m_button->draw ( CL_Rect(marginLeft+totalPtLeft, marginTop+barHeight*upPos, marginLeft+totalPtLeft+totalPtWidth,  marginTop+(barHeight*(upPos+1))) );

        // Texts:
        //
        const int textPosX = marginLeft;
        const int textPosY = marginTop + 4 + barHeight*upPos;
        m_font->set_alignment(origin_top_center, 0, 0);
        m_font->draw( textPosX+rankLeft+rankWidth/2, textPosY,  ossRankStr.str());
        m_font->draw( textPosX+nameLeft+nameWidth/2, textPosY, m_player[rank]->getName() );
        m_font->draw( textPosX+addPtLeft+addPtWidth/2, textPosY, ossRacePoints.str() );
        m_font->draw( textPosX+totalPtLeft+totalPtWidth/2, textPosY, ossTotalPoints.str() );
    }
}

/** Called on key release.
*/
void
CAChampionshipScreen::on_key_released (const CL_InputEvent &key) 
{
    switch( key.id ) 
    {
        // Cancel (ESC):
        //
    case CL_KEY_ESCAPE:
        cancel = true;
        done = true;
        break;

        // Activate:
        //
    case CL_KEY_ENTER:
    case CL_KEY_SPACE:
        if (m_displayMode != DISPLAY_CHAMPIONSHIP)
        {
            int displayInt = int(m_displayMode) + 1;
            m_displayMode = DisplayMode(displayInt);
            if (m_displayMode == DISPLAY_CHAMPIONSHIP)
            for (unsigned int pl=0; pl < m_player.size(); pl++)
            {
                m_player[pl]->setTotalPoints( m_player[pl]->getTotalPoints() + m_player[pl]->getRacePoints() );
                // players points have changed so we sort the player list again
                std::sort(m_player.begin(), m_player.end(), RankPredicate());
            }
        }
        else
        {
            done = true;
        }
        break;

    default:
        break;
    }
}

// EOF
