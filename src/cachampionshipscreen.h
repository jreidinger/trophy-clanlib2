#ifndef CACHAMPIONSHIPSCREEN_H
#define CACHAMPIONSHIPSCREEN_H

#include <ClanLib/display.h>

#include "cascreen.h"

class Player;

/** The championship screen shown after every race
   @author Andrew Mustun
   @author Matthieu Lecesne
*/

/** Prediate to sort player by rank
*/
struct RankPredicate: public std::binary_function<Player*, Player*, bool>
{
   bool operator()(const Player* const p1, const Player* const p2);
};


enum DisplayMode
{
    DISPLAY_ADD_POINTS_EASY,
    DISPLAY_ADD_POINTS_MEDIUM,
    DISPLAY_ADD_POINTS_HARD,
    DISPLAY_CHAMPIONSHIP
};

class CAChampionshipScreen : public CAScreen {
public:
  
    CAChampionshipScreen(Player* humanPlayer, const std::vector<Player*> player, const std::vector<std::vector<Player*> > runningPlayer,
                                          CL_Image background, CL_Image button, CL_Image button_easy, CL_Image button_medium, CL_Image button_hard, CL_Font_Sprite font);
    ~CAChampionshipScreen();

    void reset();

    void setOffset( int o );

    virtual int run();
    virtual void buildScreen();

    virtual void on_key_released (const CL_InputEvent &key, const CL_InputState &state);

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
    //! Pointer to human player
    Player* m_humanPlayer;
    //! all players sorted by Totalrank
    std::vector<Player*> m_player;
    //! players running in the 3 races
    std::vector<std::vector <Player*> > m_runningPlayers;
    //! The Background Surface
    CL_Image m_background;
    //! The font use for display championship result
    CL_Font m_font;
    //! The button use in the championship table
    CL_Image m_button;
     //! The button use for easy race point
    CL_Image m_buttonEasy;
     //! The button use for medium race point
    CL_Image m_buttonMedium;
     //! The button use for hard race point
    CL_Image m_buttonHard;
    
     //! display mode (display the point earned in last Race)
    DisplayMode m_displayMode;

public:
    CL_Slot slot;
};

#endif
