#ifndef CAMENUINPUT_H
#define CAMENUINPUT_H

#include "camenulabel.h"
#include "calineedit.h"

/** A menu item for inputing user specific values (strings).
    @author Andrew Mustun
*/
class CAMenuInput : public CAMenuLabel  {
public:
    CAMenuInput( CAMenu* menu, int pos, const std::string label, std::string result, int maxLength );
    CAMenuInput( CAMenu* menu, int pos, const std::string label, int* result, int maxLength );
    ~CAMenuInput();

    /** Returns rtti of this menu item.
        CA_MI_MENUINPUT for menu inputs.
    */
    virtual int rtti() {
        return CA_MI_MENUINPUT;
    }

    void display( bool highlight );
    void handleKey (const CL_InputEvent &key);

private:
    //! The edit line. Created and destroyed by this class.

    CALineEdit* edit;
    //! The string typed by the user.
    std::string resultStr;
    //! The int value typed by the user.
    int* resultInt;
    //! Max number of letters to input.
    int maxLength;
    //! Whether the result should be a int or a string
    bool isInt;
};

#endif
