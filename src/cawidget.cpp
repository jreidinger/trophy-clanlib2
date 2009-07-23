#include "cawidget.h"
#include "catrophy.h"

/** Constructor.
*/
CAWidget::CAWidget( Alignment alignment ) {
    if( CA_APP->debug ) std::cout << "CAWidget() begin" << std::endl;

    left = right = top = bottom = 0;  // Coordinates of the widget
    width = height = 0;               // Size of widget
    this->alignment = alignment;

    if( CA_APP->debug ) std::cout << "CAWidget() end" << std::endl;
}



/** Destructor.
*/
CAWidget::~CAWidget() {}



/** Moves the widget to the given point.
    The new position depends also on the alignment.
*/
void
CAWidget::move( int x, int y ) {
    switch( alignment ) {
    case Left:
        left = x;
        right = left+width;
        break;

    case Right:
        right = x;
        left = right-width;
        break;

    case Center:
        left = x-width/2;
        right = x+width/2;
        break;
    }

    top = y;
    bottom = top+height;
}



/** Resizes this widget.
    The new position depends also on the alignment.
    -1 don't resize
*/
void
CAWidget::resize( int width, int height )
{
    if (width != -1) this->width = width;
    if (height != -1) this->height = height;

    switch( alignment ) {
    case Left:
        right = left+width;
        break;

    case Right:
        left = right-width;
        break;

    case Center:
        left = (left+right)/2 - width/2;
        right = (left+right)/2 + width/2;
        break;
    }

    bottom = top+height;

}

// EOF
