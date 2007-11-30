#ifndef CACREDITS_H
#define CACREDITS_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "cadialog.h"
#include "utils/stringlist.h"

/** Credits screen.
    @author Andrew Mustun
*/
class CACredits : public CADialog  {
public:
    CACredits();
    ~CACredits();

    virtual void buildScreen();
    virtual void on_key_released( const CL_InputEvent &key );

private:
    //! Position of scrolling text


    float scrollPos;
    //! The scrolling text
    StringList text;
};

#endif
