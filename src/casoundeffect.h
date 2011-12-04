#ifndef CASOUNDEFFECT_H
#define CASOUNDEFFECT_H

#include <ClanLib/core.h>
#include <ClanLib/sound.h>


/**A sound effect is a sample which can be played. However, it's necessary to avoid playing the same sample too often at the same time.
  *@author Andrew Mustun
  */
class CASoundEffect {
public:
    CASoundEffect( CL_SoundBuffer sample, int maxPlays=1 );
    ~CASoundEffect();

    void play(int num=1, bool loop=false);
    void stop();
    void setVolume( float v );
    float getVolume( int ind=0 );
    void setFrequency( int f );
    int getFrequency( int ind=0 );

    static void playBackgroundMelody ();
    static void stopBackgroundMelody ();

private:
    //! The sample to play.

    CL_SoundBuffer sample;
    //! Max. number the sample can be played at once.
    int maxPlays;
    //! We play the sample already 'numPlays' times at once
    int numPlays;
    //! The current sound sessions for this sample
    CL_SoundBuffer_Session *session;

};

#endif
