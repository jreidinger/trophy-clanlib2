#ifndef CAINFODIALOG_H
#define CAINFODIALOG_H

#include <ClanLib/core.h>
#include "cadialog.h"
#include <vector>
#include <string>

/** A simple info dialog. Shows a title and a text.
    @author Andrew Mustun
*/
class CAInfoDialog : public CADialog  {
public:
    enum InfoType { Info, Warning };

    CAInfoDialog( const std::string& title,
                  const std::string& text,
                  const InfoType type=Info,
                  bool modal=false,
                  CAScreen* screen=0 );

    virtual void buildScreen();
    virtual void on_key_released (const CL_InputEvent &key);

private:
    //! Text list which contains the dialog text - one item is one line.
    std::vector<std::string> textList;
    //! Pointer to icon. Created and destroyed in this class.
    CL_Image icon;
    //! Type (Info, Warning).
    InfoType type;
};


#endif
