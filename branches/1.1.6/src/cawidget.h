#ifndef CAWIDGET_H
#define CAWIDGET_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>

/** Base class for widgets.
    @author Andrew Mustun
*/
class CAWidget {
public:
    enum Alignment { Left, Right, Center };

    CAWidget( Alignment alignment=Left );
    virtual ~CAWidget();

    void move( int x, int y );
    void resize( int width, int height );

    virtual void display( bool active=true ) = 0;
    virtual void handleKey( const CL_InputEvent &key ) {}

    //! Returns left border position of this widget.

    int  getLeft() {
        return left;
    }
    //! Returns top border position of this widget.
    int  getTop() {
        return top;
    }
    //! Returns right border position of this widget.
    int  getRight() {
        return right;
    }
    //! Returns bottom border position of this widget.
    int  getBottom() {
        return bottom;
    }

    //! Returns horizontal center position.
    int  getHCenter() {
        return (left+right)/2;
    }
    //! Returns vertical center position.
    int  getVCenter() {
        return (top+bottom)/2;
    }

    //! Returns width.
    int  getWidth() {
        return width;
    }
    //! Returns height.
    int  getHeight() {
        return height;
    }

protected:
    //! Left border position.

    int left;
    //! Right border position.
    int right;
    //! Top border position.
    int top;
    //! Bottom border position.
    int bottom;
    //! Width.
    int width;
    //! Height.
    int  height;

    // Widget Alignment (Left, Center, Right)
    Alignment alignment;
};

#endif
