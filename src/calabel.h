#ifndef CALABEL_H
#define CALABEL_H

#include "cawidget.h"

/** A text label with fixed with text. (used usually for tables)
    @author Andrew Mustun
*/
class CALabel : public CAWidget  {
public:
    CALabel( std::string text, Alignment alignment, CL_Font font );
    ~CALabel();

    virtual void display( bool active=true );
    //! Sets a nwe text for this label.
    void setText( const std::string& text ) {
        this->text = text;
    }

private:
    //! The text of the label.

    std::string text;
    //! Pointer to the font.
    CL_Font font;
};

#endif
