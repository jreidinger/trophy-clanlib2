#include "camenulabel.h"
#include "camenu.h"
#include "caresources.h"
#include "catrophy.h"

/** Constructor.
*/
CAMenuLabel::CAMenuLabel( CAMenu* menu, int pos, const std::string label )
        : CAMenuItem( menu, pos, label ) {
    font=0;
}


/** Destructor.
*/
CAMenuLabel::~CAMenuLabel() {}



/** Displays the menu label.
*/
void
CAMenuLabel::display( bool highlight ) {
    //static float frame = 0.0;
    top = menu->getTop() + pos*menu->getItemHeight() + menu->getHeaderHeight();

    if( highlight ) {
        font = CA_RES->font_normal_22_white;
        //CA_RES->menu_cursorani->draw (menu->getLeft()-40, top+5, (int)frame);
        CA_RES->menu_cursorani.draw ( *CA_APP->graphicContext,menu->getLeft()-40, top+5);
        //CA_RES->advanceAnimation( &frame, CA_RES->menu_cursorani->get_frame_surface(CA_RES->menu_cursorani->get_current_frame()), 10.0 );
        CA_RES->menu_cursorani.update();
    } else {
        font = CA_RES->font_normal_22_gray;
    }

    font.draw_text( *CA_APP->graphicContext, menu->getLeft() + CA_MENUSPACE/2,
                      top + CA_MENUSPACE/2,
                      label );
}



/** Handles keys on this label.
*/
void
CAMenuLabel::handleKey (const CL_InputEvent &key) 
{
    // Do nothing
}


// EOF
