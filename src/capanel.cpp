#include "capanel.h"
#include "catrophy.h"

/** Constructor.
*/
CAPanel::CAPanel() {
    for( int c=0; c<CA_MAXPLAYERS; ++c ) {
        playerView[c] = new CAPlayerView( CA_APP->player[c] );
    }
}


/** Displays the panel.
*/
void
CAPanel::display() 
{
    CL_Display::fill_rect( CL_Rect(0, 0, 120, CA_APP->height), CL_Color(0, 0, 0, 255) );

    CA_RES->panel_label->draw (0,0);

    // Display time:
    //
    CA_RES->font_lcd_13_green->set_alignment(origin_top_left, 0, 0);
    CA_RES->font_lcd_13_green->draw(20, 30, CA_APP->getTimeString());

    // Display speed view:
    //
    int speedIndex = (int)fabs(CA_APP->player[0]->getSpeed() / CA_APP->player[0]->getCar()->getMotor()->getMaxSpeed() * 10.0);
    if( speedIndex>10 ) speedIndex = 10;
    CA_RES->panel_speed->set_frame(speedIndex);
    CA_RES->panel_speed->draw (0, 57);

    // Display turbo view:
    //
    CA_RES->panel_turbolabel->draw (0, 88);
    int turboIndex = (int)fabs(CA_APP->player[0]->getTurbo() / CA_APP->player[0]->getCar()->maxTurbo * 9.0);
    CA_RES->panel_turbo->set_frame(turboIndex);
    CA_RES->panel_turbo->draw (0, 100);

    // Display ammo view
    int ammoIndex = (int)fabs(CA_APP->player[0]->getBullets() / 500.0 * 20.0);
    CA_RES->panel_ammo->set_frame(ammoIndex);
    CA_RES->panel_ammo->draw (0, 110);

    // Display player views:
    //
    for( int c=0; c<CA_MAXPLAYERS; ++c ) {
        playerView[c]->display( c+1 );
    }

    // Display frames per second:
    //
    static int counter=0;
    static int fps=0;
    counter++;

    if( counter > 200 ) {
        counter=0;
        fps = (int)(CA_APP->framesPerSec);
    }

    char str[16];
    sprintf( str, "FPS: %d", fps );
    CL_Display::fill_rect( CL_Rect(10, CA_APP->height-20, 110, CA_APP->height-10), CL_Color(0, 0, 0, 255) );
    CA_RES->font_normal_14_white->set_alignment(origin_top_left, 0, 0);
    CA_RES->font_normal_14_white->draw(10, CA_APP->height-20, str);
}

// EOF
