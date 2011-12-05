#ifndef CACREDITS_H
#define CACREDITS_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "cadialog.h"
#include <vector>
#include <string>

/** Credits screen.
    @author Andrew Mustun
*/
class CACredits : public CADialog  {
public:
    CACredits();
    ~CACredits();

    virtual void buildScreen();
    virtual void on_key_released( const CL_InputEvent &key, const CL_InputState & state );

protected:
    void setText(const std::vector<std::string>& newText);

private:
    //! Position of scrolling text
    float scrollPos;
    //! The scrolling text
    std::vector<std::string> text;
    
};

#endif
