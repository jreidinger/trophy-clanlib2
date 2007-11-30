#include "camenu.h"

#include "caresources.h"
#include "catrophy.h"
#include "camenulabel.h"
#include "camenuinput.h"
#include "camenuselect.h"

/** Constructor.
*/
CAMenu::CAMenu( const char* title )
        : CAScreen() {
    this->title = title;

    numItems = 0;
    cursor = 0;
    done = false;
    cancel = false;
    changed = false;

    calcMenuDimensions();
}

/** Destructor.
*/
CAMenu::~CAMenu() {}

/** Adds a simple menu label to the menu.
*/
void
CAMenu::addMenuLabel( const char* label ) {
    item[numItems] = new CAMenuLabel( this, numItems, label );
    if( numItems<CA_MAXMENUITEMS ) numItems++;
    calcMenuDimensions();
}

/** Adds a menu input to the menu.
    Menu inputs let the user input any value (string).
    \param label The menu item label.
    \param defaultValue Value shown at startup of the menu.
*/
void
CAMenu::addMenuInput( const char* label, char* result, int maxLength ) {
    item[numItems] = new CAMenuInput( this, numItems, label, result, maxLength );
    if( numItems<CA_MAXMENUITEMS ) numItems++;
    calcMenuDimensions();
}

/** Adds a menu input to the menu.
    Menu inputs let the user input any value (int).
    \param label The menu item label.
    \param defaultValue Value shown at startup of the menu.
*/
void
CAMenu::addMenuInput( const char* label, int* result, int maxLength ) {
    item[numItems] = new CAMenuInput( this, numItems, label, result, maxLength );
    if( numItems<CA_MAXMENUITEMS ) numItems++;
    calcMenuDimensions();
}

/** Adds a menu select to the menu.
    Menu selects let the user choose a value from a given list (string).
    \param label The menu item label.
    \param valueList Possible values to choose from.
    \param result Pointer to an int value which stores the selection result.
                  (0=first value selected)
*/
void
CAMenu::addMenuSelect( const char* label, const char* valueList, int* result ) {
    item[numItems] = new CAMenuSelect( this, numItems, label, valueList, result );
    if( numItems<CA_MAXMENUITEMS ) numItems++;
    calcMenuDimensions();
}

/** Adds a menu select to the menu.
    Menu selects let the user choose a value from a given list (string).
    \param label The menu item label.
    \param valueList Possible values to choose from.
    \param result Pointer to a bool value which stores the selection result.
                  (false=first value selected)
*/
void
CAMenu::addMenuSelect( const char* label, const char* valueList, bool* result ) {
    item[numItems] = new CAMenuSelect( this, numItems, label, valueList, result );
    if( numItems<CA_MAXMENUITEMS ) numItems++;
    calcMenuDimensions();
}

/** Runs the menu.
*/
int
CAMenu::run() {
    //slot = CL_Input::sig_button_press.connect(this, &CAMenu::on_button_press);
    // TODO : Connect the right signal
    slot = CL_Keyboard::sig_key_up().connect(this, &CAMenu::on_key_pressed);

    //CA_APP->fadeScreen( true, this );
    done = false;
    cancel = false;

    // Menu loop:
    //
    while( !done ) {
        CA_APP->measureFrameTime( true );

        buildScreen();

        // Play background sound:
        CASoundEffect::playBackgroundMelody();

        if( configureMenu && changed ) {
            CA_APP->reconfigure();
        }

        changed = false;

        CL_Display::flip();   // Copy framebufer to screen
        CL_System::keep_alive();      // VERY VITAL for the system!

        CA_APP->measureFrameTime( false );
    }

    //CA_APP->fadeScreen( false, this );
    CA_APP->waitForSilence();

    CL_Keyboard::sig_key_up().disconnect(slot);
    return (cancel ? -1 : cursor);
}

/** Builds the menu screen.
*/
void
CAMenu::buildScreen() {
    // Menu Backgroud:
    //
    CA_RES->menu_bg->draw ( CL_Rect (0,0, CA_APP->width,CA_APP->height) );

    // Menu Title:
    //
    //CA_RES->menu_bar->put_screen( left,top );
    //CA_RES->menu_bar->put_screen( left,top+height+12 );
    //CA_RES->menu_itemoff->put_screen( left,top );

    CA_RES->font_normal_14_white->set_alignment(origin_top_center, 0, 0);
    CA_RES->font_normal_14_white->draw ((left + right)/2, top + CA_MENUSPACE/2 -10, title);

    for( int n=0; n<numItems; ++n ) {
        if( item[n]->rtti()!=CA_MI_VIRTUAL ) {
            item[n]->display( n==cursor );
        }
    }
}



/** Calculates the max menu text dimesnions.
*/
void
CAMenu::calcMenuDimensions() {
    //itemHeight = CA_RES->menu_itemon->get_height();
    itemHeight = CA_RES->font_normal_22_white->get_height() + 6;
    //headerHeight = CA_RES->menu_itemon->get_height();
    headerHeight = 22;
    height = itemHeight * numItems + headerHeight;
    width = CA_MENUWIDTH;

    left = (CA_APP->width - width)/2;
    right = CA_APP->width - left;
    top = (CA_APP->height - height)/2 + 30;
    bottom = CA_APP->height - top;
}

/** Called on key release.
*/
void
CAMenu::on_key_pressed (const CL_InputEvent &key) 
{
    switch( key.id ) {

        // Cancel (ESC):
        //
    case CL_KEY_ESCAPE:
        cancel = true;
        done = true;
        break;

        // Up:
        //
    case CL_KEY_UP:
    case CL_KEY_E:
        if( cursor>0 ) {
            cursor--;
            if( CA_APP->sound ) CA_RES->effectMenu->play( 2 );
        }

        break;

        // Down:
        //
    case CL_KEY_DOWN:
    case CL_KEY_D:
        if( cursor<numItems-1 ) {
            cursor++;
            if( CA_APP->sound ) CA_RES->effectMenu->play( 2 );
        }

        break;

        // Activate:
        //
    case CL_KEY_ENTER:
    case CL_KEY_SPACE:
        if( item[cursor]->rtti()==CA_MI_MENULABEL ) {
            done = true;
        }
        break;

    default:
        break;
    }

    // Maybe the current item is also interested in our event:
    //
    item[cursor]->handleKey( key );
}

// EOF
