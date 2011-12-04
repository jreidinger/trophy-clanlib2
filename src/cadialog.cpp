#include "cadialog.h"
#include "catrophy.h"
#include "caresources.h"
#include "utils/trophymath.h"


/** Constructor.
    \param modal true: Dialog shown in addition to the current screen.
                 false: Dialog sown in new screen with new background (default).
    \param screen pointer to screen shown in the background if this dialog is modal.
                  zero if this dialog is not modal (default).
*/
CADialog::CADialog( const bool modal, CAScreen* screen )
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
    slot = CA_APP->keyboard.sig_key_up().connect(this, &CADialog::on_key_released);

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

    m_guiBox.display();
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

    m_guiBox.setPosition(left, top, right, bottom);
}


// EOF
