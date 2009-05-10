#ifndef CACHAMPIONSHIPSCREEN_H
#define CACHAMPIONSHIPSCREEN_H

#include <ClanLib/display.h>

#include "cascreen.h"

class Player;

/** The championship screen shown after every race
   @author Andrew Mustun
   @author Matthieu Lecesne
*/
enum DisplayMode
{
   DISPLAY_ADD_POINTS,
   DISPLAY_CHAMPIONSHIP
};

class CAChampionshipScreen : public CAScreen {
public:
    CAChampionshipScreen(const std::vector<Player*> player, CL_Surface* background, CL_Surface* button, CL_Font* font);
    ~CAChampionshipScreen();

    void reset();

    void setOffset( int o );

    virtual int run();
    virtual void buildScreen();

    virtual void on_key_released (const CL_InputEvent &key);

    void playSwitchSound();

private:

    //! Left border position.
    int left;
    //! Right border position.
    int right;
    //! Top border position.
    int top;
    //! Bottom border position.
    int bottom;
    //! Height of the button
    int barHeight;
    //! player sorted by Totalrank
    std::vector<Player*> m_player;
    //! The Background Surface
    CL_Surface* m_background;
    //! The font use for display championship result
    CL_Font* m_font;
    //! The button use in the championship table
    CL_Surface* m_button;
     //! display mode (display the point earned in last Race)
    DisplayMode m_displayMode;

public:
    CL_Slot slot;
};

#endif
