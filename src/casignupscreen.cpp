#include "casignupscreen.h"
#include "catrophy.h"

/** Constructor.
*/
CASignUpScreen::CASignUpScreen()
        : CAScreen() {
    cursor = 0;
    offset = 0;
    title = "P R E S S   E N T E R   T O   S I G N   U P";
    help = "Use Arrow Keys to change selection and press Enter to\nconfirm";

    left = (CA_APP->width - 640)/2;
    right = CA_APP->width - left;
    top = CA_APP->headerHeight;
    bottom = top + 400;

    numTracks = CA_APP->trackList.size();

    for( int i=0; i<3; ++i ) {
        image[i] = 0;
        racePreview[i] = 0;
    }

    setOffset( 0 );
}

/** Destructor.
*/
CASignUpScreen::~CASignUpScreen() {
    reset();
}

/** Resets/destroys the images and previews.
*/
void
CASignUpScreen::reset() 
{
    for( int i=0; i<3; ++i ) 
    {
        if( racePreview[i] ) {
            delete racePreview[i];
            racePreview[i] = 0;
        }
        if( image[i] ) {
            delete image[i];
            image[i] = 0;
        }
        // CAImageData doesn't delete image[i] !!
    }
}

/** Set new offset for the shown tracks. Offset 2 means
    that tracks 2-4 are shown.
    \param o New Offset.
*/
void
CASignUpScreen::setOffset( int o ) 
{
    std::string trackPath = (std::string) "tracks/";

    offset = o;

    reset();

    for( int i=0; i<3; ++i ) 
    {
        //printf( "\ni: %d", i );

        if( i<numTracks ) 
        {
            //image[i] = CL_TargaProvider::create( trackPath + CA_APP->trackList.getItem(i+offset) + "/thumb.tga", NULL );
            try
            {
                image[i] = new CL_Surface (CL_TargaProvider( trackPath + CA_APP->trackList[i+offset] + "/thumb.tga" ));
            }
            catch(CL_Error err)
            {
                trackPath = (std::string) "../resources/tracks/";
                image[i] = new CL_Surface (CL_TargaProvider( trackPath + CA_APP->trackList[i+offset] + "/thumb.tga" ));
            }
            racePreview[i] = new CAImageView( CA_APP->trackList[i+offset], "", image[i], false );

            racePreview[i]->setImageSize( 150, 92 );
            racePreview[i]->move( left + 50 + 195*i, top + 50 );
        } 
        else 
        {
            image[i] = 0;
            racePreview[i] = 0;
        }
    }
}

/** Runs the screen.
*/
int
CASignUpScreen::run() 
{
    //CL_Input::chain_button_release.push_back( this );
    //slot = CL_Input::sig_button_release.connect(thCreateSlot(this, &CASignUpScreen::on_button_release));
    //slot = CL_Input::sig_button_press.connect(this, &CASignUpScreen::on_button_release);
    slot = CL_Keyboard::sig_key_up().connect(this, &CASignUpScreen::on_key_released);

    CA_APP->fadeScreen( true, this );
    done = false;
    cancel = false;

    // Screen loop:
    //
    while( !done ) 
    {
        CA_APP->measureFrameTime( true );

        buildScreen();

        // Play background sound:
        CASoundEffect::playBackgroundMelody();

        CL_Display::flip();   // Copy framebufer to screen
        CL_System::keep_alive();      // VERY VITAL for the system!

        CA_APP->measureFrameTime( false );
    }

    CA_APP->fadeScreen( false, this );
    CA_APP->waitForSilence();

    //CL_Input::chain_button_release.remove( this );

    CL_Keyboard::sig_key_up().disconnect(slot);
    return (cancel ? -1 : cursor+offset);
}

/** Builds the screen.
*/
void
CASignUpScreen::buildScreen() 
{
    // Counter for cursor animation:
    static float cursorAnim = 0.0;

    // Backgroud:
    //
    CA_RES->menu_bg->draw ( CL_Rect(0, 0, CA_APP->width, CA_APP->height) );

    // Title / help:
    //
    displayTitle();
    displayHelp();

    // Cursor:
    //
    CL_Display::fill_rect( CL_Rect(racePreview[cursor]->getLeft()-12, racePreview[cursor]->getTop()-28,
                           racePreview[cursor]->getRight()+12, racePreview[cursor]->getBottom()+12),
                           CL_Color (255, 216, 84, (int)((cursorAnim/2)*255) ));

    CA_RES->advanceAnimation( &cursorAnim, 1, 2.0, CAResources::Revolving );

    // Race previews:
    //
    for( int i=0; i<3; ++i ) {
        if( racePreview[i] ) {
            racePreview[i]->display();
            //CA_RES->font_normal_14_white->print_center( racePreview[i]->getHCenter(),
            //                                            racePreview[i]->getTop()-22,
            //                                            (i==0 ? "Easy" : (i==1 ? "Medium" : "Hard")) );
        }

    }
}

/** Called on key release.
*/
void
CASignUpScreen::on_key_released (const CL_InputEvent &key) 
{
    switch( key.id ) 
    {
        // Cancel (ESC):
        //
    case CL_KEY_ESCAPE:
        cancel = true;
        done = true;
        break;

        // Left:
        //
    case CL_KEY_LEFT:
    case CL_KEY_N:
        if( cursor>0 ) {
            cursor--;
            playSwitchSound();
        } else if( offset>0 ) {
            setOffset( offset-1 );
            playSwitchSound();
        }
        break;

        // Right:
        //
    case CL_KEY_RIGHT:
    case CL_KEY_M:
        if( cursor<2 && cursor<numTracks-1 ) {
            cursor++;
            playSwitchSound();
        } else if( offset<numTracks-3 ) {
            setOffset( offset+1 );
            playSwitchSound();
        }
        break;

        // Activate:
        //
    case CL_KEY_ENTER:
    case CL_KEY_SPACE:
        done = true;
        break;

    default:
        break;
    }
}

void
CASignUpScreen::playSwitchSound() {
    if( CA_APP->sound ) CA_RES->effectMenu->play( 2 );
}

// EOF
