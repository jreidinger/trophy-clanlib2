#include <sstream>

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
CAInfoDialog::CAInfoDialog( const std::string& title,
                            const std::string& text,
                            const InfoType type,
                            bool modal,
                            CAScreen* screen )
        : CADialog( modal, screen ) {
    if( CA_APP->debug ) std::cout << "CAInfoDialog() begin" << std::endl;

    this->title = title;
    this->type = type;
    
    std::istringstream iss ( text );
    std::string temp;
    while (std::getline(iss,temp, '~'))
       textList.push_back(temp);
       

    switch( type ) {
    case Warning:
        icon = CA_RES->misc_caution;
        break;

    case Info: 
    default: //use Info as default
        icon = CA_RES->misc_info;
        break;
    }

    resize( 400, textList.size()*16 + 96 );

    if( CA_APP->debug ) std::cout << "CAInfoDialog() end" << std::endl;
}


/** Builds the info dialog screen.
*/
void
CAInfoDialog::buildScreen() {
    CADialog::buildScreen();

    int x = 112; //FIXME magic number

    CA_RES->font_normal_14_white.draw_text (*CA_APP->graphicContext,left+x, top+32, title);

    for( unsigned int i=0; i<textList.size(); ++i ) {
        CA_RES->font_normal_11_white.draw_text (*CA_APP->graphicContext,left+x, top+64+i*16, textList[i]);
    }

    icon.draw (*CA_APP->graphicContext,left+32, top+32);
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
