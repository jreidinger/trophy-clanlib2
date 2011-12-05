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
class CACarUpgrades;

/** The shop screen shown after every race where players can buy upgrade and new cars
   @author Matthieu Lecesne
*/

class ShopScreen : public CAScreen
{
public:
    ShopScreen(Player* player, CL_Image background, CL_Image button, CL_Font_Sprite font, CACarUpgrades* carRes);
    virtual ~ShopScreen();

    virtual int run();
    virtual void buildScreen();

    virtual void on_key_released (const CL_InputEvent &key, const CL_InputState&);

private:
    void updateText();

    
    //! the human player
    Player* m_player;
    //! The Background Surface
    CL_Image m_background;
    //! The button use
    CL_Image m_button;
    //! The font use for display car information
    CL_Font_Sprite m_font;
    //! Upgrades ressources manager
    CACarUpgrades* m_carRes;

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

    //! the 3 bottom boxes (upgrades)
    CAImageView* m_imageView[3];
    // ! the continue box
    CAImageView* m_continue;

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
