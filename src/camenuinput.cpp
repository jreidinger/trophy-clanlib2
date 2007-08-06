#include <stdio.h>

#include "camenuinput.h"
#include "camenu.h"
#include "caresources.h"

/** Constructor which takes a string as result.
*/
CAMenuInput::CAMenuInput( CAMenu* menu,
                          int pos, const char* label,
                          char* result, int maxLength )
        : CAMenuLabel( menu, pos, label ) {
    this->maxLength = maxLength;
    resultStr = result;
    edit = new CALineEdit( resultStr,
                           maxLength, CAWidget::Right,
                           CA_RES->font_normal_22_gray );

    resultInt = 0;
}



/** Constructor which takes an int as result.
*/
CAMenuInput::CAMenuInput( CAMenu* menu,
                          int pos, const char* label,
                          int* result, int maxLength )
        : CAMenuLabel( menu, pos, label ) {
    this->maxLength = maxLength;
    resultInt = result;

    char intStr[16];
    sprintf( intStr, "%d", *resultInt );
    edit = new CALineEdit( intStr,
                           maxLength, CAWidget::Right,
                           CA_RES->font_normal_22_gray );
    resultStr = 0;
}



/** Destructor.
*/
CAMenuInput::~CAMenuInput() {
    delete edit;
    edit = 0;
}



/** Displays the item.
*/
void
CAMenuInput::display( bool highlight ) {
    CAMenuLabel::display( highlight );
    edit->move( menu->getRight() - CA_MENUSPACE/2, top + CA_MENUSPACE/2 );
    edit->display( highlight );
}



/** Handles key inputs.
*/
void
CAMenuInput::handleKey (const CL_InputEvent &key) 
{
    edit->handleKey( key );
    if( resultStr ) strncpy( resultStr, edit->getText(), maxLength );
    if( resultInt ) *resultInt = atoi( edit->getText() );
    menu->setChanged();
}


// EOF
