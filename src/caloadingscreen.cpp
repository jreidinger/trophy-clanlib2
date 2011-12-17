#include "caloadingscreen.h"
#include "catrophy.h"

/** Constructor.
*/
CALoadingScreen::CALoadingScreen() {
    progress = 0.0;
}



/** Destructor.
*/
CALoadingScreen::~CALoadingScreen() {}



/** Starts the screen.
*/
void
CALoadingScreen::begin() {
    progress = 0.0;
}




/** Sets the progress in percent.
*/
void
CALoadingScreen::setProgress( float p ) {
    progress = p;
    buildScreen();

    CA_APP->display_window->flip();   // Copy framebufer to screen
    CL_KeepAlive::process();      // VERY VITAL for the system!
}



/** Adds progress in percent.
*/
void
CALoadingScreen::addProgress( float p ) {
    setProgress( progress+p );
}



/** End the screen.
*/
void
CALoadingScreen::end() {
    progress = 100.0;
    CA_APP->fadeScreen( false, this );
}



/** Builds the screen.
*/
void
CALoadingScreen::buildScreen() {
    CA_APP->graphicContext->clear();

    int right = (CA_APP->width-CA_RES->gui_loading.get_width())/2;
    int top = (CA_APP->height-CA_RES->gui_loading.get_height())/2;
    CA_RES->gui_loading.draw ( *CA_APP->graphicContext,right, top);

    CA_RES->gui_progressbar.draw ( *CA_APP->graphicContext, CL_Rect(right+35, top+87, right+35+(int)(249.0 / 100.0 * progress), top+87+26) );
}

// EOF

