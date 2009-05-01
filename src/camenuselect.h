#ifndef CAMENUSELECT_H
#define CAMENUSELECT_H

#include "camenulabel.h"
#include <vector>
#include <string>


/** A menu item for selecting a value out of a given value list (strings).
    That's kind of a combobox, isn't it?
    @author Andrew Mustun
*/
template<typename T>
class CAMenuSelect : public CAMenuLabel {
public:


    CAMenuSelect( CAMenu* menu, int pos, const std::string& label, const std::string& nvalueList, T* result );


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

    std::vector<std::string> valueList;
    
    //! Pointer to result given by constructor or 0 if result is boolean.
    T*          tResult;

   //! Index of currently selected item.
    int           selectedItem;
};

#endif

// EOF
