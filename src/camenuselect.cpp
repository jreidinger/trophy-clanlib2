#include "camenu.h"
#include "camenuselect.h"
#include "caresources.h"
#include <string>

  /** Constructor for selections of more than two items.
    \param menu Pointer to menu
    \param pos  Position of the item in the menu (0=top position)
    \param label Menu item label.
    \param valueList List of possible values. Format: "value1~value2~value3~..."
    \param result Pointer to store the index of the selected item (0=first item of list)
   */
    template<typename T>
    CAMenuSelect<T>::CAMenuSelect( CAMenu* menu, int pos, const std::string& label, const std::string& nvalueList, T* result ) 
    : CAMenuLabel( menu, pos, label ), tResult(result), selectedItem(0)
    {
        std::istringstream iss ( nvalueList );
        std::string temp;
        while (std::getline(iss,temp, '~'))
        valueList.push_back(temp);
    }

   /** Displays the item.
   */
    template<typename T>
    void CAMenuSelect<T>::display( bool highlight )
    {
        CAMenuLabel::display( highlight );

        if( font ) {
            font->set_alignment(origin_top_right, 0, 0);
            font->draw (menu->getRight() - CA_MENUSPACE/2, top + CA_MENUSPACE/2, valueList[selectedItem]);
        }
    }

    /** Handles keys on this label.
     */
    template<typename T>
    void CAMenuSelect<T>::handleKey(const CL_InputEvent &key)
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

        updateValue();
        menu->setChanged();
    }
    
    
    template<typename T>
    void CAMenuSelect<T>::updateValue()
    {
        *tResult = T(selectedItem);
    }

    template<>
    void CAMenuSelect<std::string>::updateValue()
    {
        *tResult = valueList[selectedItem];
    }

template class CAMenuSelect<int>;
template class CAMenuSelect<bool>;
template class CAMenuSelect<std::string>;
// EOF
