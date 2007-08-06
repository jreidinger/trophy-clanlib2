#include "cadialog.h"
#include "catrophy.h"
#include "caresources.h"
#include "camath.h"


/** Constructor.
    \param modal true: Dialog shown in addition to the current screen.
                 false: Dialog sown in new screen with new background (default).
    \param screen pointer to screen shown in the background if this dialog is modal.
                  zero if this dialog is not modal (default).
*/
CADialog::CADialog( bool modal, CAScreen* screen )
        : CAScreen() {
    width = height = 0;
    left = top = right = bottom = 0;
    this->modal = modal;
    this->screen = screen;
    done = false;
    cancel = false;
}



/** Destructor.
*/
CADialog::~CADialog() {}



/** Runs the dialog.
*/
int
CADialog::run() {
    if( CA_APP->debug ) std::cout << "CADialog::run() begin" << std::endl;

    //CL_Input::chain_button_release.push_back( this );

    //slot = CL_Input::sig_button_release.connect(thCreateSlot(this, &CADialog::on_button_release));
    //slot = CL_Input::sig_button_press.connect(this, &CADialog::on_button_release);
    slot = CL_Keyboard::sig_key_up().connect(this, &CADialog::on_key_released);

    //if( !modal ) CA_APP->fadeScreen( true, this );
    done = false;
    cancel = false;

    // Dialog loop:
    //
    while( !done ) {
        CA_APP->measureFrameTime( true );

        if( modal && screen ) screen->buildScreen();
        buildScreen();

        // Play background sound:
        CASoundEffect::playBackgroundMelody();

        CL_Display::flip();   // Copy framebufer to screen
        CL_System::keep_alive();      // VERY VITAL for the system!

        CA_APP->measureFrameTime( false );
    }

    //if( !modal ) CA_APP->fadeScreen( false, this );
    CA_APP->waitForSilence();

    //CL_Input::chain_button_release.remove( this );

    if( CA_APP->debug ) std::cout << "CADialog::run() end" << std::endl;

    CL_Keyboard::sig_key_up().disconnect(slot);
    return (int)(!cancel);
}



/** Builds the dialog screen.
*/
void
CADialog::buildScreen() {
    // Menu Backgroud:
    //
    if( !modal ) CA_RES->menu_bg->draw( CL_Rect(0,0, CA_APP->width,CA_APP->height) );

    int ew = CA_RES->gui_edge2->get_width();
    int eh = CA_RES->gui_edge2->get_height();
    int bw = CA_RES->gui_border2->get_width();
    int bh = CA_RES->gui_border1->get_height();

    CL_Display::fill_rect( CL_Rect(left+bw, top+bh, right-bw, bottom-bh), CL_Color(0, 0, 0, 64) );

    // Edges:
    //
    CA_RES->gui_edge1->draw( left, top );
    CA_RES->gui_edge2->draw( right-ew, top );
    CA_RES->gui_edge3->draw( left, bottom-eh );
    CA_RES->gui_edge4->draw( right-ew, bottom-eh );

    // Borders:
    //
    CA_RES->gui_border1->draw( CL_Rect(left+ew, top, left+ew+width-2*ew, top+bh) );
    CA_RES->gui_border2->draw( CL_Rect(right-bw, top+eh, right-bw+bw, top+eh+height-2*eh) );
    CA_RES->gui_border3->draw( CL_Rect(left+ew, bottom-bh, left+ew+width-2*ew, bottom-bh+bh) );
    CA_RES->gui_border4->draw( CL_Rect(left, top+eh, left+bw, top+eh+height-2*eh) );
}



/** Resizes the dialog.
*/
void
CADialog::resize( int w, int h ) {
    width = w;
    height = h;
    left = (CA_APP->width - width) / 2;
    right = left + width;
    top = (CA_APP->height - CA_APP->headerHeight - CA_HELPHEIGHT - height) / 2
          + CA_APP->headerHeight;
    bottom = top + height;
}


// EOF
