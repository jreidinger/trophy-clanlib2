#include "capanel.h"
#include "catrophy.h"

/** Constructor.
*/
CAPanel::CAPanel()
{
    for( int c=0; c<CA_RACEMAXPLAYERS; ++c )
    {
        m_playerView.push_back(new CAPlayerView( CA_APP->m_RacePlayer[c] ));
    }
}

CAPanel::~CAPanel()
{
    for( unsigned int c=0; c<m_playerView.size(); ++c )
    {
        delete m_playerView[c];
    }
}


/** Displays the panel.
*/
void
CAPanel::display() 
{
    CL_RoundedRect round_rec(CL_Sizef(120, CA_APP->height));
    round_rec.fill( *CA_APP->graphicContext, CL_Pointf(0,0), CL_Colorf(0, 0, 0, 255));

    CA_RES->panel_label.draw ( *CA_APP->graphicContext,0,0);

    // Display time:
    //
    CA_RES->font_lcd_13_green.draw_text( *CA_APP->graphicContext, 20, 30, CA_APP->getTimeString());

    // Display speed view:
    //
    int speedIndex = (int)fabs(CA_APP->player[0]->getSpeed() / CA_APP->player[0]->getCar()->getMotor()->getMaxSpeed() * 10.0);
    if( speedIndex>10 ) speedIndex = 10;
    CA_RES->panel_speed.set_frame(speedIndex);
    CA_RES->panel_speed.draw ( *CA_APP->graphicContext,0, 57);

    // Display turbo view:
    //
    CA_RES->panel_turbolabel.draw ( *CA_APP->graphicContext,0, 88);
    int turboIndex = (int)fabs(CA_APP->player[0]->getTurbo() / CA_APP->player[0]->getCar()->maxTurbo * 9.0);
    CA_RES->panel_turbo.set_frame(turboIndex);
    CA_RES->panel_turbo.draw ( *CA_APP->graphicContext,0, 100);

    // Display ammo view
    int ammoIndex = (int)fabs(CA_APP->player[0]->getBullets() / 500.0 * 20.0);
    CA_RES->panel_ammo.set_frame(ammoIndex);
    CA_RES->panel_ammo.draw ( *CA_APP->graphicContext,0, 110);

    // Display player views:
    //
    for( unsigned int c=0; c<m_playerView.size(); ++c )
    {
        m_playerView[c]->display( c+1 );
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
    CL_RoundedRect rect(CL_Sizef(100,10));
    rect.fill( *CA_APP->graphicContext,CL_Pointf(10,CA_APP->height-20), CL_Colorf(0, 0, 0, 255) );
    CA_RES->font_normal_14_white.draw_text( *CA_APP->graphicContext,10, CA_APP->height-20, str);
}

// EOF
