#include "camenu.h"

#include "caresources.h"
#include "catrophy.h"
#include "camenulabel.h"
#include "camenuinput.h"
#include "camenuselect.h"

/** Constructor.
*/
CAMenu::CAMenu( const std::string title )
        : CAScreen() {
    this->title = title;

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
CAMenu::addMenuLabel( const std::string& label )
{
    if( item.size()<CA_MAXMENUITEMS )
        item.push_back( new CAMenuLabel( this, item.size(), label ));
    calcMenuDimensions();
}

/** Adds a menu input to the menu.
    Menu inputs let the user input any value (string).
    \param label The menu item label.
    \param defaultValue Value shown at startup of the menu.
*/
void
CAMenu::addMenuInput( const std::string& label, std::string result, int maxLength )
{
    if( item.size()<CA_MAXMENUITEMS )
        item.push_back(new CAMenuInput( this, item.size(), label, result, maxLength ));
    calcMenuDimensions();
}

/** Adds a menu input to the menu.
    Menu inputs let the user input any value (int).
    \param label The menu item label.
    \param defaultValue Value shown at startup of the menu.
*/
void
CAMenu::addMenuInput( const std::string& label, int* result, int maxLength )
{
    if( item.size()<CA_MAXMENUITEMS )
        item.push_back(new CAMenuInput( this, item.size(), label, result, maxLength ));
    calcMenuDimensions();
}

/** Adds a menu select to the menu.
    Menu selects let the user choose a value from a given list (string).
    \param label The menu item label.
    \param valueList Possible values to choose from.
    \param result Pointer to an int value which stores the selection result.
                  (0=first value selected)
*/
template<typename T>
void CAMenu::addMenuSelect( const std::string& label, const std::string valueList, T* result )
{
    if( item.size()<CA_MAXMENUITEMS )
        item.push_back(new CAMenuSelect<T>( this, item.size(), label, valueList, result ));
    calcMenuDimensions();
}


/** Runs the menu.
*/
int CAMenu::run() {
    //slot = CL_Input::sig_button_press.connect(this, &CAMenu::on_button_press);
    // TODO : Connect the right signal
    slot = CA_APP->keyboard.sig_key_up().connect(this, &CAMenu::on_key_pressed);

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

        CA_APP->display_window->flip();   // Copy framebufer to screen
        CL_KeepAlive::process();      // VERY VITAL for the system!

        CA_APP->measureFrameTime( false );
    }

    //CA_APP->fadeScreen( false, this );
    CA_APP->waitForSilence();

    return (cancel ? -1 : cursor);
}

/** Builds the menu screen.
*/
void
CAMenu::buildScreen() {
    // Menu Backgroud:
    //
    CA_RES->menu_bg.draw ( *CA_APP->graphicContext, CL_Rect (0,0, CA_APP->width,CA_APP->height) );

    // Menu Title:
    //
    //CA_RES->menu_bar->put_screen( left,top );
    //CA_RES->menu_bar->put_screen( left,top+height+12 );
    //CA_RES->menu_itemoff->put_screen( left,top );

    CA_RES->font_normal_14_white.draw_text ( *CA_APP->graphicContext,(left + right)/2, top + CA_MENUSPACE/2 -10, title);

    for( int n=0; n<int(item.size()); ++n ) {
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
    itemHeight = CA_RES->font_normal_22_white.get_font_metrics().get_height() + 6;
    //headerHeight = CA_RES->menu_itemon->get_height();
    headerHeight = 22;
    height = itemHeight * item.size() + headerHeight;
    width = CA_MENUWIDTH;

    left = (CA_APP->width - width)/2;
    right = CA_APP->width - left;
    top = (CA_APP->height - height)/2 + 30;
    bottom = CA_APP->height - top;
}

/** Called on key release.
*/
void
CAMenu::on_key_pressed (const CL_InputEvent &key, const CL_InputState &state) 
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
        if( cursor<int(item.size()-1) ) {
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

template void CAMenu::addMenuSelect<int>( const std::string& label, const std::string valueList, int* result );
template void CAMenu::addMenuSelect<bool>( const std::string& label, const std::string valueList, bool* result );
template void CAMenu::addMenuSelect<std::string>( const std::string& label, const std::string valueList, std::string* result );
// EOF
