#ifndef CASCREEN_H
#define CASCREEN_H

#include <ClanLib/core.h>

/** Base class for screens like menus or tables.
    @author Andrew Mustun
 */
class CAScreen 
{
public:
    CAScreen(const std::string& n_title = "", const std::string& n_help = "");
    virtual ~CAScreen();

    virtual int run() = 0;
    virtual void buildScreen() = 0;

    bool canceled() {
        return cancel;
    }
    void displayTitle();
    void displayHelp();

protected:
    //! Done -> go to next screen
    bool done;
    //! Screen canceled
    bool cancel;
    //! Screen title
    std::string title;
    //! Help text (shown at bottom)
    std::string help;
};

#endif
