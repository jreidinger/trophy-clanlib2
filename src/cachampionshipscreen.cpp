#include "cachampionshipscreen.h"
#include "catrophy.h"
#include "player.h"
#include <functional>
#include <algorithm>
#include <sstream>

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
CAChampionshipScreen::CAChampionshipScreen(Player* humanPlayer, const std::vector<Player*> player, const std::vector<std::vector<Player*> > runningPlayer,
                                                                           CL_Image background, CL_Image button, CL_Image buttonEasy, CL_Image buttonMedium, CL_Image buttonHard, CL_Font_Sprite font)
   : CAScreen(),
   m_humanPlayer(humanPlayer),
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
    barHeight = m_font.get_font_metrics().get_height() + 6;
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
    slot = CA_APP->keyboard.sig_key_up().connect(this, &CAChampionshipScreen::on_key_released);

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

        CA_APP->display_window->flip();   // Copy framebufer to screen
        CL_KeepAlive::process(-1);      // VERY VITAL for the system!

        CA_APP->measureFrameTime( false );
    }

    CA_APP->fadeScreen( false, this );
    CA_APP->waitForSilence();

    //CL_Input::chain_button_release.remove( this );

    if (m_player[0] == m_humanPlayer)
        return 0;
    else
        return 1;
}

/** Builds the screen.
*/
void
CAChampionshipScreen::buildScreen() 
{
    // Background:
    //
    m_background.draw ( *CA_APP->graphicContext, CL_Rect(0, 0, CA_APP->width, CA_APP->height) );
    displayTitle();

    CL_Draw::fill( *CA_APP->graphicContext, CL_Rectf(left, top, right, bottom), CL_Colorf(0, 0, 0, 64) );
    
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
        m_button.draw ( *CA_APP->graphicContext, CL_Rect(marginLeft+rankLeft, marginTop+barHeight*upPos, marginLeft+rankLeft+rankWidth, marginTop+(barHeight*(upPos+1))) );
        m_button.draw ( *CA_APP->graphicContext, CL_Rect(marginLeft+nameLeft, marginTop+barHeight*upPos, marginLeft+nameLeft+nameWidth, marginTop+(barHeight*(upPos+1))) );
        if (m_displayMode != DISPLAY_CHAMPIONSHIP)
        {
            for (int i=0; i<= int(m_displayMode); i++)
            {
                std::vector<Player*>::const_iterator it = std::find(m_runningPlayers[i].begin(), m_runningPlayers[i].end(), m_player[rank]);
                if (it != m_runningPlayers[i].end() && m_player[rank]->getRacePoints() != 0)
                {
                    CL_Image buttonToUse = (i==0 ? m_buttonEasy : (i==1)? m_buttonMedium : m_buttonHard); // Choose the button color
                    buttonToUse.draw ( *CA_APP->graphicContext, CL_Rect(marginLeft+addPtLeft, marginTop+barHeight*upPos, marginLeft+addPtLeft+addPtWidth, marginTop+(barHeight*(upPos+1))) );
                    ossRacePoints << "+" << m_player[rank]->getRacePoints();
                }
            }
        }
        m_button.draw ( *CA_APP->graphicContext, CL_Rect(marginLeft+totalPtLeft, marginTop+barHeight*upPos, marginLeft+totalPtLeft+totalPtWidth,  marginTop+(barHeight*(upPos+1))) );

        // Texts:
        //
        const int textPosX = marginLeft;
        const int textPosY = marginTop + 4 + barHeight*upPos;
        m_font.draw_text( *CA_APP->graphicContext, textPosX+rankLeft+rankWidth/2, textPosY,  ossRankStr.str());
        m_font.draw_text( *CA_APP->graphicContext, textPosX+nameLeft+nameWidth/2, textPosY, m_player[rank]->getName() );
        m_font.draw_text( *CA_APP->graphicContext, textPosX+addPtLeft+addPtWidth/2, textPosY, ossRacePoints.str() );
        m_font.draw_text( *CA_APP->graphicContext, textPosX+totalPtLeft+totalPtWidth/2, textPosY, ossTotalPoints.str() );
    }
}

/** Called on key release.
*/
void
CAChampionshipScreen::on_key_released (const CL_InputEvent &key, const CL_InputState &state) 
{
    switch( key.id ) 
    {
        // Activate:
        //
    case CL_KEY_ENTER:
    case CL_KEY_SPACE:
    case CL_KEY_ESCAPE:
        if (m_displayMode != DISPLAY_CHAMPIONSHIP)
        {
            int displayInt = int(m_displayMode) + 1;
            m_displayMode = DisplayMode(displayInt);
            if (m_displayMode == DISPLAY_CHAMPIONSHIP)
            {
                for (unsigned int pl=0; pl < m_player.size(); pl++)
                {
                    m_player[pl]->addMoney( m_player[pl]->getRaceMoney() );
                    m_player[pl]->setTotalPoints( m_player[pl]->getTotalPoints() + m_player[pl]->getRacePoints() );
                    m_player[pl]->resetForRace(0, NULL); // Player doesn't belong to a race for now    
                }
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
