#include "caslotselectiondialog.h"
#include "catrophy.h"



/** Constructor.
    \param title Title of the dialog (Usually "Save..." or "Load...")
*/
CASlotSelectionDialog::CASlotSelectionDialog( const std::string title )
        : CADialog() {
    cursor = 0;
    this->title = title;
    help = "Use Arrow Keys to change selection and press Enter to\nconfirm";
    left = (CA_APP->width - 320)/2;
    right = CA_APP->width - left;
    top = CA_APP->headerHeight;
    bottom = top + 400;
}



/** Destructor.
*/
CASlotSelectionDialog::~CASlotSelectionDialog() {}



/** Builds the screen.
*/
void
CASlotSelectionDialog::buildScreen() {
    // Counter for cursor animation:
    //static float cursorAnim = 0.0;

    // Backgroud:
    //
    CA_RES->menu_bg->draw ( CL_Rect(0, 0, CA_APP->width, CA_APP->height) );

    // Title / help:
    //
    displayTitle();
    displayHelp();



    // Cursor:
    //
    /*CL_Display::fill_rect( racePreview[cursor]->getLeft()-12, racePreview[cursor]->getTop()-28,
                           racePreview[cursor]->getRight()+12, racePreview[cursor]->getBottom()+12,
                           1.0,0.85,0.33, (cursorAnim/2) );
    */

    //CA_RES->advanceAnimation( &cursorAnim, 1, 2.0, CAResources::Revolving );
}



/** Called on key release.
*/
void
CASlotSelectionDialog::on_key_released (const CL_InputEvent &key) 
{
    switch( key.id ) {

        // Cancel (ESC):
        //
    case CL_KEY_ESCAPE:
        cancel = true;
        done = true;
        break;

        // Left:
        //
    case CL_KEY_UP:
        if( cursor>0 ) {
            cursor--;
            if( CA_APP->sound ) CA_RES->effectMenu->play( 2 );
        }
        break;

        // Right:
        //
    case CL_KEY_DOWN:
        if( cursor<CA_NUMSLOTS-1 ) {
            cursor++;
            if( CA_APP->sound ) CA_RES->effectMenu->play( 2 );
        }
        break;

        // Activate:
        //
    case CL_KEY_ENTER:
    case CL_KEY_SPACE:
        done = true;
        break;

    default:
        break;
    }
}


// EOF
