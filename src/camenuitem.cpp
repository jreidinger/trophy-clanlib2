#include "camenu.h"
#include "camenuitem.h"

/** Constructor.
*/
CAMenuItem::CAMenuItem( CAMenu* menu, int pos, const char* label ) {
    this->label = label;
    this->menu = menu;
    this->pos = pos;
}



/** Destructor.
*/
CAMenuItem::~CAMenuItem() {}

// EOF

