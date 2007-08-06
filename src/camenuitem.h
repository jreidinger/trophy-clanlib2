#ifndef CAMENUITEM_H
#define CAMENUITEM_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#define CA_MI_VIRTUAL     0
#define CA_MI_MENULABEL   1
#define CA_MI_MENUSELECT  2
#define CA_MI_MENUINPUT   3

class CAMenu;

/** Base class for items which can appear in menus (labels, inputs or selectors).
    @author Andrew Mustun
*/
class CAMenuItem {
public:
    CAMenuItem( CAMenu* menu, int pos, const char* label );
    virtual ~CAMenuItem();

    /** Returns rtti of this menu item.
        CA_MI_VIRTUAL for virtual menu items (should never happen).
    */
    virtual int rtti() {
        return CA_MI_VIRTUAL;
    }

    virtual void display( bool highlight ) = 0;
    virtual void handleKey( const CL_InputEvent &key ) = 0;
    //! Sets a new label for this menu item.
    void setLabel( const char* l ) {
        label = l;
    }

protected:
    //! Pointer to menu.

    CAMenu* menu;
    //! Position of this menu item (0 is the top position)
    int     pos;
    //! Top position of this menu item (in pixel)
    int     top;

    //! The label of this menu item.
    std::string label;
};

#endif

// EOF
