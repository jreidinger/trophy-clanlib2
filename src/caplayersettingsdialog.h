#ifndef CAPLAYERSETTINGSDIALOG_H
#define CAPLAYERSETTINGSDIALOG_H

#include <ClanLib/core.h>
#include "caimagehueselector.h"
#include "calineedit.h"
#include "cadialog.h"

/** Dialog for adjusting player settings (Name, color, car, face)
    @author Andrew Mustun
    @author Matthieu Lecesne
*/
class CAPlayerSettingsDialog : public CADialog {
public:
    CAPlayerSettingsDialog();

    virtual void buildScreen();
    virtual void on_key_released (const CL_InputEvent &key, const CL_InputState &state);

    const std::string getPlayerName();
    int getPlayerHue();

private:
    //! Line edit for players name
    CALineEdit  nameInput;
    //! Car image hue selector.
    CAImageHueSelector carImage;
};

#endif
