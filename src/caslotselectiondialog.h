#ifndef CASLOTSELECTIONDIALOG_H
#define CASLOTSELECTIONDIALOG_H

#include <ClanLib/core.h>
#include "cadialog.h"
class HumanPlayer;

/** Dialog which shows the slots for saving or loading a game.
    @author Andrew Mustun
*/
class CASlotSelectionDialog : public CADialog  {
public:
    CASlotSelectionDialog( const std::string title, CL_Font_Sprite font, const std::string& homedir );
    ~CASlotSelectionDialog();

    std::string display();
    virtual void buildScreen();

    virtual void on_key_released (const CL_InputEvent &key);

private:
    //! Cursor (selected slot)
    int cursor;
    
    //! The font use for displaying information
    CL_Font_Sprite m_font;

    //! home directory where the saveFile are
    const std::string& m_homedir;

    //! PreLoaded Humans Players for all save Slot (use in Upgrade Panel)
    std::vector<HumanPlayer*> m_slotPlayers;

    // Player names in each save slot
    std::vector<std::string> m_slotNames;

    
};

#endif
