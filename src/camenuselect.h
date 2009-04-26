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

   /** Constructor for selections of more than two items.
    \param menu Pointer to menu
    \param pos  Position of the item in the menu (0=top position)
    \param label Menu item label.
    \param valueList List of possible values. Format: "value1~value2~value3~..."
    \param result Pointer to store the index of the selected item (0=first item of list)
   */
    CAMenuSelect( CAMenu* menu, int pos, const std::string& label, const std::string& nvalueList, T* result ) 
    : CAMenuLabel( menu, pos, label ), tResult(result), selectedItem(*result) {
    std::istringstream iss ( nvalueList );
    std::string temp;
    while (std::getline(iss,temp, '~'))
       valueList.push_back(temp);
}


    /** Returns rtti of this menu item.
        CA_MI_MENUSELECT for menu selects.
    */
    virtual int rtti() {
        return CA_MI_MENUSELECT;
    }

   /** Displays the item.
   */
    virtual void display( bool highlight )
    {
        CAMenuLabel::display( highlight );

        if( font ) {
            font->set_alignment(origin_top_right, 0, 0);
            font->draw (menu->getRight() - CA_MENUSPACE/2, top + CA_MENUSPACE/2, valueList[selectedItem]);
        }
    }

    /** Handles keys on this label.
     */
    virtual void handleKey (const CL_InputEvent &key)
    {
        if( key.id==CL_KEY_ENTER ||
                key.id==CL_KEY_SPACE ||
                key.id==CL_KEY_RIGHT    ) {
            selectedItem++;
            if( selectedItem>=int(valueList.size()) ) selectedItem=0;
        }

        if( key.id==CL_KEY_LEFT ) {
            selectedItem--;
            if( selectedItem<0 ) selectedItem=int(valueList.size())-1;
        }

        *tResult = (T)(selectedItem);
        menu->setChanged();
    }


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
