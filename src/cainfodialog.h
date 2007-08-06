#ifndef CAINFODIALOG_H
#define CAINFODIALOG_H

#include <ClanLib/core.h>
#include "cadialog.h"
#include "castringlist.h"

/** A simple info dialog. Shows a title and a text.
    @author Andrew Mustun
*/
class CAInfoDialog : public CADialog  {
public:
    enum InfoType { Info, Warning };

    CAInfoDialog( const char* title,
                  const char* text,
                  InfoType type=Info,
                  bool modal=false,
                  CAScreen* screen=0 );
    ~CAInfoDialog();

    virtual void buildScreen();
    virtual void on_key_released (const CL_InputEvent &key);

private:
    //! Text list which contains the dialog text - one item is one line.
    CAStringList textList;
    //! Pointer to icon. Created and destroyed in this class.
    CL_Surface* icon;
    //! Type (Info, Warning).
    InfoType type;
};

#endif
