#include "camenu.h"
#include "camenuselect.h"
#include "caresources.h"



/** Constructor for selections of more than two items.
    \param menu Pointer to menu
    \param pos  Position of the item in the menu (0=top position)
    \param label Menu item label.
    \param valueList List of possible values. Format: "value1~value2~value3~..."
    \param result Pointer to store the index of the selected item (0=first item of list)
*/
CAMenuSelect::CAMenuSelect( CAMenu* menu,
                            int pos,
                            const std::string label,
                            const std::string valueList,
                            int* result )
        : CAMenuLabel( menu, pos, label ) {
    this->valueList = new StringList( valueList );
    selectedItem = (*result);
    intResult = result;
    boolResult = 0;
}



/** Constructor for selections of two items.
    \param menu Pointer to menu
    \param pos  Position of the item in the menu (0=top position)
    \param label Menu item label.
    \param valueList List of possible values. Format: "value1~value2"
    \param result Pointer to store the index of the selected item (0=first item of list)
*/
CAMenuSelect::CAMenuSelect( CAMenu* menu,
                            int pos,
                            const std::string label,
                            const std::string valueList,
                            bool* result )
        : CAMenuLabel( menu, pos, label ) {
    this->valueList = new StringList( valueList );
    selectedItem = (int)(*result);
    boolResult = result;
    intResult = 0;
}



/** Destructor.
*/
CAMenuSelect::~CAMenuSelect() {
    delete valueList;
    valueList = 0;
}



/** Displays the item.
*/
void
CAMenuSelect::display( bool highlight ) {
    CAMenuLabel::display( highlight );

    if( font ) {
        font->set_alignment(origin_top_right, 0, 0);
        font->draw (menu->getRight() - CA_MENUSPACE/2, top + CA_MENUSPACE/2, valueList->getItem(selectedItem));
    }
}



/** Handles keys on this label.
*/
void
CAMenuSelect::handleKey (const CL_InputEvent &key) 
{
    if( key.id==CL_KEY_ENTER ||
            key.id==CL_KEY_SPACE ||
            key.id==CL_KEY_RIGHT    ) {
        selectedItem++;
        if( selectedItem>=valueList->count() ) selectedItem=0;
    }

    if( key.id==CL_KEY_LEFT ) {
        selectedItem--;
        if( selectedItem<0 ) selectedItem=valueList->count()-1;
    }

    if( intResult ) *intResult = selectedItem;
    if( boolResult ) *boolResult = (bool)(selectedItem);
    menu->setChanged();
}


// EOF

