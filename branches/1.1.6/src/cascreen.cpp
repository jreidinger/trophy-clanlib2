#include "cascreen.h"
#include "catrophy.h"
#include "caresources.h"

/** Constructor.
*/
CAScreen::CAScreen() {
    done = false;
    cancel = false;
    title = "";
    help = "";
}

/** Destructor.
*/
CAScreen::~CAScreen() {}

/** Displays a typical title for the screen.
*/
void
CAScreen::displayTitle() 
{
    CA_RES->font_normal_11_white->set_alignment(origin_top_center, 0, 0);
    CA_RES->font_normal_11_white->draw((CA_APP->width)/2, (int)((float)CA_APP->height/4.2f), title);
}

/** Displays the bottom-help for dummies.
*/
void
CAScreen::displayHelp() 
{
    CL_Display::fill_rect( CL_Rect(0, CA_APP->height-CA_HELPHEIGHT,
                           CA_APP->width, CA_APP->height),
                           CL_Color(0, 0, 0, 127) );

    CA_RES->font_normal_11_white->set_alignment(origin_top_center, 0, 0);
    CA_RES->font_normal_11_white->draw((CA_APP->width)/2, CA_APP->height-CA_HELPHEIGHT+10, help);
}

// EOF
