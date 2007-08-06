#ifndef CALINEEDIT_H
#define CALINEEDIT_H

#include <ClanLib/core.h>
#include "cawidget.h"


/** A simple oneline edit (GUI).
    @author Andrew Mustun
*/
class CALineEdit : public CAWidget {
public:
    CALineEdit( const char* text,
                unsigned int letters,
                Alignment alignment,
                CL_Font* font );
    ~CALineEdit();

    const char* getText() {
        return text.c_str();
    }

    virtual void display( bool active=true );
    virtual void handleKey (const CL_InputEvent &key);

private:
    //! Number of letters shown at max.

    unsigned int letters;
    //! Cursor position.
    unsigned int cursor;
    //! Pointer to font.
    CL_Font*  font;
    //! The input text
    std::string text;
};

#endif
