#include "catrophy.h"
#include "casoundeffect.h"

/** Constructor.
    \param resource The sample to play.
    \param maxPlays Maximal number the sound can be played at once.
*/
CASoundEffect::CASoundEffect( CL_SoundBuffer* sample, int maxPlays ) {
    this->maxPlays = maxPlays;
    numPlays = 0;
    session = new CL_SoundBuffer_Session[maxPlays];
    this->sample = sample;
}

/** Destructor.
*/
CASoundEffect::~CASoundEffect() {
    delete[] session;
    session = NULL;
}

/** Plays the sample if it isn't played already too often at once.
    \param num Play this sample 'num' times at once (makes it louder than 100%).
    \param loop Loop the sample (default false)
*/
void
CASoundEffect::play( int num, bool loop ) 
{
    if( !CA_APP->sound ) return;

    for( int i=0; i<num; ++i) 
    {
        // Are there samples which are finished?
        //
        while( numPlays!=0 && !session[0].is_playing() ) 
        {
            for( int c=1; c<numPlays; ++c ) {
                session[c-1] = session[c];
            }
            numPlays--;
        }

        // Play the sample:
        //
        if( numPlays<maxPlays ) 
        {
            session[numPlays] = sample->play(loop);
            numPlays++;
        }
    }
}

/** Stop all instances of the sample 
 */
void
CASoundEffect::stop( )
{
    if( !CA_APP->sound ) return;

    while( numPlays!=0 ) 
    {
        if(session[numPlays].is_playing()) session[numPlays].stop();
        numPlays--;
    }
}

/** Sets a new volume for all instances of this
    sample which are currently played.
*/
void
CASoundEffect::setVolume( float v ) {
    if( !CA_APP->sound ) return;

    for( int c=0; c<numPlays; ++c ) {
        if( session[c].is_playing() ) {
            session[c].set_volume( v );
        }
    }
}

/** Gets the volume of sample with index 'ind'.
*/
float
CASoundEffect::getVolume( int ind ) {
    if( ind>=0 && ind<numPlays ) {
        return session[ind].get_volume();
    }

    return 0.0;
}

/** Sets a new frequency for all instances of this
    sample which are currently played.
*/
void
CASoundEffect::setFrequency( int f ) {
    // TODO: I don't know what this define is for just copied from above (compilation without sound?)
    if( !CA_APP->sound ) return;

    for( int c=0; c<numPlays; ++c ) {
        if( session[c].is_playing() ) {
            session[c].set_frequency( f );
        }
    }
}

/** Gets the frequency of sample with index 'ind'.
*/
int
CASoundEffect::getFrequency(int ind) {
    if( ind>=0 && ind<numPlays ) {
        return session[ind].get_frequency();
    }

    return 0;
}

/** Plays a background melody.
*/
void
CASoundEffect::playBackgroundMelody() {
    CA_RES->effectBackground01->play( 8 );
}

/** Stop the background melody.
*/
void
CASoundEffect::stopBackgroundMelody() {
    CA_RES->effectBackground01->stop();
}

// EOF
