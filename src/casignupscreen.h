#ifndef CASIGNUPSCREEN_H
#define CASIGNUPSCREEN_H

#include <ClanLib/display.h>


#include "cascreen.h"
#include <vector>

class Player;
class CAImageView;

/** The sign-up screen shown before the race to choose the track.
    @author Andrew Mustun
    @author Matthieu Lecesne
*/
class CASignUpScreen : public CAScreen {
public:
    CASignUpScreen(std::vector<Player*> player, std::vector<int> trackNumbers);
    ~CASignUpScreen();

    void reset();

    void setOffset( int offset );

    virtual int run();
    virtual void buildScreen();

    virtual void on_key_released (const CL_InputEvent &key);

    void playSwitchSound();

    int getRaceLevel();

    std::vector<Player*> getRacePlayers();

    std::vector<std::vector<Player*> > getAllRunningPlayers();

    void addVirtualPoints();

    std::vector<int> getTrackNumbers() const;
    
private:
    //! Cursor (selected race)

    int cursor;
    //! Offset for shown tracks. 1 -> left shown track is no 1 (not 0)
    int offset;
    //! The three race preview images
    CL_Image  image[3];
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
    //! to know if the player has selected a race
    bool m_selected;
    //! Array of Players
    std::vector<Player*> m_Player;
    //! Array of Players who are running in the 3 races
    std::vector<std::vector<Player*> > m_RacePlayer;
    //! Array of the string uses to show the competitor of a race
    std::vector<std::vector<std::string> > m_StringRacePlayer;
    //! 3 Track directory chosen
    std::vector<std::string> m_trackList;
    //! 3 Track number chosen
    std::vector<int> m_trackNumbers;


public:
    CL_Slot slot;
};

#endif
