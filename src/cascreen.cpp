#include "cascreen.h"
#include "catrophy.h"
#include "caresources.h"

/** Constructor.
*/
CAScreen::CAScreen(const std::string& n_title, const std::string& n_help)
:
    done   (false),
    cancel (false),
    title  (n_title),
    help   (n_help)
{
}

/** Destructor.
*/
CAScreen::~CAScreen() {}

/** Displays a typical title for the screen.
*/
void
CAScreen::displayTitle() 
{
    CA_RES->font_normal_11_white.draw_text( *CA_APP->graphicContext,(CA_APP->width)/2, (int)((float)CA_APP->height/4.2f), title);
}

/** Displays the bottom-help for dummies.
*/
void
CAScreen::displayHelp() 
{
    CL_Draw::fill( *CA_APP->graphicContext, CL_Rectf(0, CA_APP->height-CA_HELPHEIGHT,
                           CA_APP->width, CA_APP->height),
                           CL_Colorf(0, 0, 0, 127) );

    CA_RES->font_normal_11_white.draw_text( *CA_APP->graphicContext,(CA_APP->width)/2, CA_APP->height-CA_HELPHEIGHT+10, help);
}

// EOF
