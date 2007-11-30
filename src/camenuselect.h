#ifndef CAMENUSELECT_H
#define CAMENUSELECT_H

#include "camenulabel.h"
#include "utils/stringlist.h"


/** A menu item for selecting a value out of a given value list (strings).
    That's kind of a combobox, isn't it?
    @author Andrew Mustun
*/
class CAMenuSelect : public CAMenuLabel {
public:
    CAMenuSelect( CAMenu* menu, int pos, const char* label, const char* valueList, int* result );
    CAMenuSelect( CAMenu* menu, int pos, const char* label, const char* valueList, bool* result );
    ~CAMenuSelect();

    /** Returns rtti of this menu item.
        CA_MI_MENUSELECT for menu selects.
    */
    virtual int rtti() {
        return CA_MI_MENUSELECT;
    }

    virtual void display( bool highlight );
    virtual void handleKey (const CL_InputEvent &key);
    int  getSelectedItem() {
        return selectedItem;
    }
    void setSelectedItem( int index ) {
        selectedItem = index;
    }

private:
    //! List of values to select.

    StringList* valueList;
    //! Index of currently selected item.
    int           selectedItem;

    //! Pointer to result given by constructor or 0 if result is boolean.
    int*          intResult;
    //! Pointer to result given by constructor or 0 if result is int.
    bool*         boolResult;
};

#endif

// EOF
