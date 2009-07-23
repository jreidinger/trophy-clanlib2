#include "caimagemanipulation.h"
#include "caplayerview.h"
#include "caresources.h"

/** Constructor.
    \param player Pointer to the appropriate player.
*/
CAPlayerView::CAPlayerView( Player* player ) {
    this->player = player;
    button = 0;
    renderButton();
}

/** Destructor.
*/
CAPlayerView::~CAPlayerView() {
    delete button;
    button = 0;
}

/** Displays the player view in the panel area.
    \param defaultPos default position if rank is 0 (race startup)
*/
void
CAPlayerView::display( int defaultPos ) {
    // Maybe we need to re-render the button sprite:
    //
    if( currentColor!=player->getColor() ) {
        renderButton();
    }

    int lap = (int)( floor( player->getPosition() )+1.0 );
    int rank = (player->getRaceRank()==0 ? defaultPos : player->getRaceRank());
    int y = (rank-1) * 55 + 110 + CA_RES->panel_turbo->get_height()+CA_RES->panel_turbolabel->get_height(); // The ammo label has been added

    if( lap>5 ) lap=5;

    std::ostringstream ossLap, ossRank, ossNumLap;
    ossLap << lap;
    ossRank << rank;
    ossNumLap << CA_NUMLAPS;

    // Display color button:
    //
    button->draw (0, y);
    CA_RES->panel_life->draw ( (int)((float)player->getLife() / 100.0 * 120.0), y );

    // Display name:
    //
    CA_RES->font_normal_11_white->set_alignment(origin_top_left, 0, 0);
    CA_RES->font_normal_11_white->draw( 3, y+2, player->getName() );

    // Display laps and rank:
    //
    CA_RES->panel_infoview->draw (0, y+20);
    CA_RES->font_lcd_13_green->set_alignment(origin_top_left, 0, 0);
    CA_RES->font_lcd_13_green->draw( 50, y+22, ossLap.str() );
    CA_RES->font_lcd_13_green->set_alignment(origin_top_left, 0, 0);
    CA_RES->font_lcd_13_green->draw( 70, y+31, ossNumLap.str() );
    CA_RES->font_normal_11_white->set_alignment(origin_top_left, 0, 0);
    CA_RES->font_normal_11_white->draw( 99, y+30, ossRank.str() );

    // Display cross for death players:
    //
    if( player->getLife()<0.1 ) {
        CL_Display::fill_rect( CL_Rect(0, y, 120, y+55), CL_Color(0, 0, 0, 85) );
        CA_RES->panel_death->draw (0, y);
    }
}

/** Renders the button for this players color.
*/
void
CAPlayerView::renderButton() {
    if( button ) delete button;
    button = CAImageManipulation::changeHSV( CA_RES->panel_button,
             player->getColor().h,
             player->getColor().s,
             player->getColor().v );
    currentColor = player->getColor();
}

// EOF
