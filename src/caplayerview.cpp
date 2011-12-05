#include <sstream>
#include "caimagemanipulation.h"
#include "caplayerview.h"
#include "caresources.h"
#include "catrophy.h"

/** Constructor.
    \param player Pointer to the appropriate player.
*/
CAPlayerView::CAPlayerView( Player* player ) {
    this->player = player;
    renderButton();
}

/** Destructor.
*/
CAPlayerView::~CAPlayerView() {
}

/** Displays the player view in the panel area.
    \param defaultPos default position if rank is 0 (race startup)
*/
void
CAPlayerView::display( const int defaultPos ) {
    // Maybe we need to re-render the button sprite:
    //
    if( currentColor!=player->getColor() ) {
        renderButton();
    }

    int lap = (int)( floor( player->getPosition() )+1.0 );
    int rank = (player->getRaceRank()==0 ? defaultPos : player->getRaceRank());
    int y = (rank-1) * 55 + 110 + 38; // The turbo jauge + the ammo jauge

    if( lap>5 ) lap=5;

    std::ostringstream ossLap, ossRank, ossNumLap;
    ossLap << lap;
    ossRank << rank;
    ossNumLap << CA_NUMLAPS;

    // Display color button:
    //
    button.draw ( *CA_APP->graphicContext,0, y);
    CA_RES->panel_life.draw ( *CA_APP->graphicContext, (int)((float)player->getLife() / 100.0 * 120.0), y );

    // Display name:
    //
    CA_RES->font_normal_11_white.draw_text( *CA_APP->graphicContext, 3, y+2, player->getName() );

    // Display laps and rank:
    //
    CA_RES->panel_infoview.draw ( *CA_APP->graphicContext,0, y+20);
    CA_RES->font_lcd_13_green.draw_text( *CA_APP->graphicContext, 50, y+22, ossLap.str() );
    CA_RES->font_lcd_13_green.draw_text( *CA_APP->graphicContext, 70, y+31, ossNumLap.str() );
    CA_RES->font_normal_11_white.draw_text( *CA_APP->graphicContext, 99, y+30, ossRank.str() );

    // Display cross for death players:
    //
    if( player->getLife()<0.1 ) {
        CL_Draw::fill( *CA_APP->graphicContext, CL_Rectf(0, y, 120, y+55), CL_Colorf(0, 0, 0, 85) );
        CA_RES->panel_death.draw ( *CA_APP->graphicContext,0, y);
    }
}

/** Renders the button for this players color.
*/
void
CAPlayerView::renderButton() {
    button = CA_RES->panel_button;
/*    button = CAImageManipulation::changeHSV( CA_RES->panel_button,
             player->getColor().h,
             player->getColor().s,
             player->getColor().v );*/
    currentColor = player->getColor();
}

// EOF
