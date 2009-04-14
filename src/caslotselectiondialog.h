#ifndef CASLOTSELECTIONDIALOG_H
#define CASLOTSELECTIONDIALOG_H

#include <ClanLib/core.h>
#include "cadialog.h"

/** Dialog which shows the slots for saving or loading a game.
    @author Andrew Mustun
*/
class CASlotSelectionDialog : public CADialog  {
public:
    CASlotSelectionDialog( const std::string title );
    ~CASlotSelectionDialog();

    virtual void buildScreen();

    virtual void on_key_released (const CL_InputEvent &key);

private:
    //! Cursor (selected slot)

    int cursor;
};

#endif
