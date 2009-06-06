#ifndef SHOPSCREEN_H
#define SHOPSCREEN_H

#include <ClanLib/display.h>

#include "cascreen.h"
#include "utils/trophymath.h"
#include "guibox.h"

class Player;
class CAImageHueSelector;
class CAImageSelector;
class CAImageView;

/** The shop screen shown after every race where players can buy upgrade and new cars
   @author Matthieu Lecesne
*/

class ShopScreen : public CAScreen
{
public:
    ShopScreen(Player* player, CL_Surface* background, CL_Surface* button, CL_Font* font);
    virtual ~ShopScreen();

    void reset();

    void setOffset( int o );

    virtual int run();
    virtual void buildScreen();

    virtual void on_key_released (const CL_InputEvent &key);

private:
    void updateText();

    void drawTextButton(const CL_Rect& rect, const std::string& text);

    
    //! the human player
    Player* m_player;
    //! The Background Surface
    CL_Surface* m_background;
    //! The button use
    CL_Surface* m_button;
    //! The font use for display car information
    CL_Font* m_font;

    //! Left border position.
    int left;
    //! Right border position.
    int right;
    //! Top border position.
    int top;
    //! Bottom border position.
    int bottom;

    //! Car image selector.
    CAImageSelector* m_carImage;

    //! the 4 bottom boxes (upgrades and continue)
    CAImageView* m_imageView[4];

    //! the widget which have focus
    CAWidget* m_focus;

    //! White Box
    GUIBox m_guiBox;

    int m_barHeight;

    std::string m_text;

    bool m_confirmMode;
    int m_cursor;
    bool m_isAbleToBuy;
    const int m_curWidth;

public:
    CL_Slot slot;
};

#endif
