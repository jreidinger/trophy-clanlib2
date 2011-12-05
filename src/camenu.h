#ifndef CAMENU_H
#define CAMENU_H

#include <vector>

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "utils/trophymath.h"
#include "camenuitem.h"
#include "cascreen.h"

/** A basic menu screen.
    @author Andrew Mustun
*/
class CAMenu : public CAScreen {
public:
    CAMenu( const std::string title );
    ~CAMenu();

    virtual int run();
    virtual void buildScreen();

    void addMenuLabel( const std::string& label );
    void addMenuInput( const std::string& label, std::string result, int maxLength );
    void addMenuInput( const std::string& label, int* result, int maxLength );
    template<typename T>
    void addMenuSelect( const std::string& label, const std::string valueList, T* result );

    void calcMenuDimensions();

    virtual void on_key_pressed(const CL_InputEvent &key, const CL_InputState &state);

    //! Returns the left border position.
    int getLeft() {
        return left;
    }
    //! Returns the right border position.
    int getRight() {
        return right;
    }
    //! Returns the top border position.
    int getTop() {
        return top;
    }
    //! Returns the bottom border position.
    int getBottom() {
        return bottom;
    }

    //! Returns the width.
    int getWidth() {
        return width;
    }
    //! Returns the height.
    int getHeight() {
        return height;
    }

    //! Returns the width of one item.
    int getItemHeight() {
        return itemHeight;
    }
    //! Returns the height of the menu header.
    int getHeaderHeight() {
        return headerHeight;
    }

    /** Sets wheter this is a configure menu or not. Configure menus
        apply settings immediately using the CA_APP->reconfigure() method.
    */
    void setConfigureMenu( bool on ) {
        configureMenu = on;
    }
    //! Sets that the user has changed something in the menu.
    void setChanged( bool on=true ) {
        changed = on;
    }
    //! Sets the cursor position (0 is the top entry).
    void setCursor( int pos ) {
        if( cursor>=0 && cursor<int(item.size()) ) cursor = pos;
    }
    //! Returns the current cursor position (o is the top position).
    int  getCursor() {
        return cursor;
    }
    //! Returns -1 if the menu was cancelled (ESC) otherwise the cursor position.
    int  getSelection() {
        return (cancel ? -1 : cursor);
    }

protected:
    //! User has changed something

    bool changed;
    //! Left menu position
    int left;
    //! Left menu position
    int right;
    //! Left menu position
    int top;
    //! Left menu position
    int bottom;
    //! Total width of menu text
    int width;
    //! Total height of menu text
    int height;
    //! Height of one menu item
    int itemHeight;
    //! Header height
    int headerHeight;

    // TODO: should we delete CAMenuItem?
    //! The menu items
    std::vector<CAMenuItem*> item; 
    //! Menu Title
    std::string title;

    //! Cursor position (0...numLabels)
    int cursor;
    //! Is this a configure menu?
    bool configureMenu;

    CL_Slot slot;

};

#endif
