#ifndef CASIGNUPSCREEN_H
#define CASIGNUPSCREEN_H

#include <ClanLib/display.h>

#include "caimageview.h"
#include "cascreen.h"

/** The sign-up screen shown before the race to choose the track.
    @author Andrew Mustun
*/
class CASignUpScreen : public CAScreen {
public:
    CASignUpScreen();
    ~CASignUpScreen();

    void reset();

    void setOffset( int offset );

    virtual int run();
    virtual void buildScreen();

    virtual void on_key_released (const CL_InputEvent &key);

    void playSwitchSound();

private:
    //! Cursor (selected race)

    int cursor;
    //! Offset for shown tracks. 1 -> left shown track is no 1 (not 0)
    int offset;
    //! The three race preview images
    CL_Surface*  image[3];
    //! Images of races
    CAImageView* racePreview[3];
    //! Left border position.
    int left;
    //! Right border position.
    int right;
    //! Top border position.
    int top;
    //! Bottom border position.
    int bottom;
    //! Number of tracks available
    int numTracks;

public:
    CL_Slot slot;
};

#endif
