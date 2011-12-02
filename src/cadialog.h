#ifndef CADIALOG_H
#define CADIALOG_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "cascreen.h"
#include "guibox.h"

/** Base class for dialogs.
    @author Andrew Mustun
    @author Matthieu Lecesne
*/
class CADialog : public CAScreen {
public:
    CADialog( const bool modal=false, CAScreen* screen=0 );
    ~CADialog();

    virtual int run();
    virtual void buildScreen();

    virtual void on_key_released( const CL_InputEvent &key ) = 0;

    virtual void resize( int w, int h );

protected:
    //! Left position of the dialog
    int left;
    //! Right position of the dialog
    int right;
    //! Top position of the dialog
    int top;
    //! Bottom position of the dialog
    int bottom;
    //! Width of the dialog
    int width;
    //! Height of the dialog
    int height;
    /** Dialog modal? Modal means it's shown transparently
        over the current screen. A pointer to the current
        screen must be given in constructor if the dialog is
        modal.
    */
    bool modal;
    //! Pointer to screen (if dialog is modal) or NULL if it's not modal.
    CAScreen* screen;

protected:
    //! The white gui box
    GUIBox m_guiBox;

public:
    CL_Slot slot;
};

#endif
