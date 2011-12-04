#ifndef UPGRADES_PANEL_H
#define UPGRADES_PANEL_H


#include "player.h"
#include "cawidget.h"
#include <ClanLib/display.h>
#include <sstream>


/** This panel is shown to see the car and the upgrades of the player
   @author Matthieu Lecesne
*/

class UpgradesPanel : public CAWidget
{
    public:
        UpgradesPanel( Player* player, CL_Font carFont, CL_Font upgradeFont, const int nvLeft, const int nvTop );

        virtual ~UpgradesPanel() {}

        void display(const bool active = true);

        void displayBarOption(CarOption* carOption);

    private:
        //! the human player
        Player* m_player;
        //! the font uses in the panel    
        CL_Font* m_carfont;
        //! the font uses for the upgrades   
        CL_Font* m_upgradesFont;
        //! the center of the bar
        int m_center;
        //! height of a bar
        int m_barHeight;
        // current top Position
        int m_curTop;
};


#endif

// EOF
