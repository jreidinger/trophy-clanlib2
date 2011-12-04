#ifndef CAMENULABEL_H
#define CAMENULABEL_H

#include "camenuitem.h"

/**A simple label in a menu.
  *@author Andrew Mustun
  */

class CAMenuLabel : public CAMenuItem  {
public:
    CAMenuLabel( CAMenu* menu, int pos, const std::string label );
    ~CAMenuLabel();

    /** Returns rtti of this menu item.
        CA_MI_MENULABEL for menu labels.
    */
    virtual int rtti() {
        return CA_MI_MENULABEL;
    }

    virtual void display( bool highlight );
    virtual void handleKey (const CL_InputEvent &key);

protected:
    CL_Font font;
};

#endif

// EOF
