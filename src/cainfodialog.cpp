#include "cainfodialog.h"
#include "caresources.h"
#include "catrophy.h"


/** Constructor.
    \param title Dialog title
    \param text The info text.
    \param type Info type (Info, Warning)
    \param modal true: Dialog shown in addition to the current screen.
                 false: Dialog sown in new screen with new background (default).
    \param screen pointer to screen shown in the background if this dialog is modal.
                  zero if this dialog is not modal (default).
*/
CAInfoDialog::CAInfoDialog( const char* title,
                            const char* text,
                            InfoType type,
                            bool modal,
                            CAScreen* screen )
        : CADialog( modal, screen ) {
    if( CA_APP->debug ) std::cout << "CAInfoDialog() begin" << std::endl;

    this->title = title;
    this->type = type;
    textList.setList( text );

    switch( type ) {
    case Info:
        icon = CA_RES->misc_info;
        // CL_Surface::load( "misc/info", CA_RES->resources );
        break;

    case Warning:
        icon = CA_RES->misc_caution;
        // CL_Surface::load( "misc/caution", CA_RES->resources );
        break;

    default:
        icon = 0;
        break;
    }

    resize( 400, textList.count()*16 + 96 );

    if( CA_APP->debug ) std::cout << "CAInfoDialog() end" << std::endl;
}



/** Destructor.
*/
CAInfoDialog::~CAInfoDialog() {
    //delete icon;
}



/** Builds the info dialog screen.
*/
void
CAInfoDialog::buildScreen() {
    CADialog::buildScreen();

    int x = (icon ? 112 : 32);

    CA_RES->font_normal_14_white->set_alignment(origin_top_left, 0, 0);
    CA_RES->font_normal_14_white->draw (left+x, top+32, title);

    for( int i=0; i<textList.count(); ++i ) {
        CA_RES->font_normal_11_white->set_alignment(origin_top_left, 0, 0);
        CA_RES->font_normal_11_white->draw (left+x, top+64+i*16, textList.getItem(i));
    }

    if( icon ) {
        icon->draw (left+32, top+32);
    }
}



/** Called on key released.
*/
void
CAInfoDialog::on_key_released (const CL_InputEvent &key) 
{
    switch( key.id ) {
    case CL_KEY_ENTER:
    case CL_KEY_ESCAPE:
        done = true;
        break;

    default:
        break;
    }
}

// EOF
