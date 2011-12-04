#ifdef W32_BCC551
  /* dont include unistd.h, but dir.h for windoof */
  #include <dir.h>
#else
  #include <unistd.h>
#endif

#include "caresources.h"
#include "catrophy.h"
#include "caloadingscreen.h"
#include "caimagemanipulation.h"

CAResources* CAResources::theOneAndOnlyResource = NULL;


/** Gets the one and only Resources-Object.
    Creates a new one on first call.
 */
CAResources*
CAResources::getResources() {
    if(theOneAndOnlyResource == NULL) {
        theOneAndOnlyResource = new CAResources;
        theOneAndOnlyResource->load();
    }
    return theOneAndOnlyResource;
}


/** Constructor.
 */
CAResources::CAResources() {}


/** Destructor.
*/
CAResources::~CAResources() {
    delete effectCrash;
    delete effectSlide;
    delete effectEngine;
    delete effectHorn;
    delete effectLight;
    delete effectMenu;
    delete effectSelector;
    delete effectGoodyMoney;
    delete effectGoodyBullets;
    delete effectGoodyTurbo;
    delete effectGoodyFogbomb;
    delete effectGoodyLife;
    delete effectShoot;
    delete effectFogBomb;
    delete effectFire;
    delete effectBackground01;
}


/** Load all resources.
*/
void
CAResources::load() {
    // Try to load resource file (resources.dat):
    //
    CL_GraphicContext& gc = *CA_APP->graphicContext;
    chdir(PKGDATADIR);

    try {
        // TODO : Can we just forget the boolean ?
        //resources = new CL_ResourceManager( "resources.dat", true );
        resources.load( "resources.xml" );
    }

    // Just use the resource config file (resources.scr) if there's no compiled .dat:
    //
    catch (CL_Exception err) {
        // TODO : Can we just forget the boolean ?
        //resources = new CL_ResourceManager( "resources.scr", false );
        resources.load( "../resources/resources.xml" );
    }

    // Load info frame for showing loading status:
    //
    gui_loading = CL_Image(gc, "gui/loading", &resources );
    gui_progressbar = CL_Image(gc, "gui/progressbar", &resources );

    // Show progress:
    //
    CA_APP->loading.begin();

    // Load fonts:
    //
    font_normal_11_white = CL_Font_Sprite(gc, "fonts/normal_11_white", &resources );
    font_normal_14_white = CL_Font_Sprite(gc, "fonts/normal_14_white", &resources );
    font_normal_22_white = CL_Font_Sprite(gc, "fonts/normal_22_white", &resources );
    font_normal_22_gray = CL_Font_Sprite(gc, "fonts/normal_22_gray", &resources );
    font_lcd_13_green = CL_Font_Sprite(gc, "fonts/lcd_13_green", &resources );

    CA_APP->loading.setProgress( 15 );

    // Load sounds:
    //

    sound_horn = CL_SoundBuffer( "sounds/horn", &resources );
    sound_menu = CL_SoundBuffer( "sounds/menu", &resources );
    sound_selector = CL_SoundBuffer( "sounds/selector", &resources );
    sound_slide = CL_SoundBuffer( "sounds/slide", &resources );
    sound_crash = CL_SoundBuffer( "sounds/crash", &resources );
    sound_engine = CL_SoundBuffer( "sounds/engine", &resources );
    sound_light = CL_SoundBuffer( "sounds/light", &resources );
    sound_goody_money = CL_SoundBuffer( "sounds/goody_money", &resources );
    sound_goody_bullets = CL_SoundBuffer( "sounds/goody_bullets", &resources );
    sound_goody_turbo = CL_SoundBuffer( "sounds/goody_turbo", &resources );
    sound_goody_fogbomb = CL_SoundBuffer( "sounds/goody_fogbomb", &resources );
    sound_goody_life = CL_SoundBuffer( "sounds/goody_life", &resources );
    sound_shoot = CL_SoundBuffer( "sounds/shoot", &resources );
    sound_fogbomb = CL_SoundBuffer( "sounds/fogbomb", &resources );
    sound_fire = CL_SoundBuffer( "sounds/fire", &resources );
    sound_background01 = CL_SoundBuffer( "sounds/background01", &resources );

    CA_APP->loading.setProgress( 25 );

    effectCrash  = new CASoundEffect( sound_crash, 2 );
    effectSlide  = new CASoundEffect( sound_slide, 1 );
    effectEngine = new CASoundEffect( sound_engine, 1 );
    effectHorn   = new CASoundEffect( sound_horn, 2 );
    effectLight  = new CASoundEffect( sound_light, 2 );
    effectMenu   = new CASoundEffect( sound_menu, 4 );
    effectSelector= new CASoundEffect( sound_selector, 10 );
    effectGoodyMoney  = new CASoundEffect( sound_goody_money, 5 );
    effectGoodyBullets  = new CASoundEffect( sound_goody_bullets, 5 );
    effectGoodyTurbo  = new CASoundEffect( sound_goody_turbo, 5 );
    effectGoodyFogbomb  = new CASoundEffect( sound_goody_fogbomb, 10 );
    effectGoodyLife  = new CASoundEffect( sound_goody_life, 5 );
    effectShoot  = new CASoundEffect( sound_shoot, 3 );
    effectFogBomb  = new CASoundEffect( sound_fogbomb, 10 );
    effectFire  = new CASoundEffect( sound_fire, 10 );
    effectBackground01  = new CASoundEffect( sound_background01, 1 );

    CA_APP->loading.setProgress( 30 );

    // Load generic sprites:
    //
    misc_cross = new CL_Image(gc, "misc/cross", resources );
    misc_flag = new CL_Image(gc, "misc/flag", resources );
    misc_light = new CL_Sprite( "misc/light", resources );
    misc_hitpoint = new CL_Sprite( "misc/hitpoint", resources );
    misc_gunfire = new CL_Sprite( "misc/gunfire", resources );
    misc_carfire = new CL_Sprite( "misc/carfire", resources );
    misc_checkflag = new CL_Sprite( "misc/checkflag", resources );
    misc_fog = new CL_Sprite( "misc/fog", resources );
    misc_dust = new CL_Sprite( "misc/dust", resources );
    misc_info = new CL_Image(gc, "misc/info", resources );
    misc_caution = new CL_Image(gc, "misc/caution", resources );


    // Load player effect sprites (dust, ...)
    //

    for( int i=0; i<CA_FPR; ++i ) {
        dust[i] = new CL_Image(gc, "cars/dust", resources );
        dust[i]->rotate( (float)i/CA_FPR*360.0 );
    }


    CA_APP->loading.setProgress( 35 );

    // Load Goodies:
    //
    goody_turbo = new CL_Image(gc, "goody/turbo", resources );
    goody_life  = new CL_Image(gc, "goody/life", resources );
    goody_money = new CL_Image(gc, "goody/money", resources );
    goody_bullets = new CL_Image(gc, "goody/bullets", resources );
    goody_fogbomb = new CL_Image(gc, "goody/fogbomb", resources );

    // Menu things:
    //
    menu_bg = new CL_Image(gc, "menu/bg", resources );
    menu_cursorani = new CL_Sprite( "menu/cursorani", resources );
    menu_bar = new CL_Image(gc, "menu/bar", resources );

    CA_APP->loading.setProgress( 40 );

    // Panel resources:
    //
    panel_label = new CL_Image(gc, "panel/label", resources );
    panel_button = new CL_Image(gc, "panel/button", resources );
    panel_life = new CL_Image(gc, "panel/life", resources );
    panel_infoview = new CL_Image(gc, "panel/infoview", resources );
    panel_speed = new CL_Sprite( "panel/speed", resources );
    panel_turbolabel = new CL_Image(gc, "panel/turbolabel", resources );
    panel_turbo = new CL_Sprite( "panel/turbo", resources );
    panel_ammo = new CL_Sprite( "panel/ammo", resources );
    panel_death = new CL_Image(gc, "panel/death", resources );

    CA_APP->loading.setProgress( 45 );

    // GUI resources:
    //
    gui_edge1 = new CL_Image(gc, "gui/edge1", resources );
    gui_edge2 = new CL_Image( gc, "gui/edge2", resources );
    gui_edge3 = new CL_Image( gc, "gui/edge3", resources );
    gui_edge4 = new CL_Image( gc, "gui/edge4", resources );
    gui_border1 = new CL_Image( gc, "gui/border1", resources );
    gui_border2 = new CL_Image( gc, "gui/border2", resources );
    gui_border3 = new CL_Image( gc, "gui/border3", resources );
    gui_border4 = new CL_Image( gc, "gui/border4", resources );
    gui_button = new CL_Image( gc, "gui/button", resources );
    gui_button_red = new CL_Image( gc,"gui/button", resources );
    gui_button_red.set_color (1.0f, 0.0f, 0.0f);
    gui_button_green = new CL_Image( gc,"gui/button", resources );
    gui_button_green.set_color (0.0f, 1.0f, 0.0f);
    gui_button_blue = new CL_Image( gc,"gui/button", resources );
    gui_button_blue.set_color (0.0f, 0.0f, 1.0f);
    gui_arrow_l = new CL_Image( gc, "gui/arrow_l", resources );
    gui_arrow_r = new CL_Image( gc, "gui/arrow_r", resources );
    gui_arrow_t = new CL_Image( gc, "gui/arrow_t", resources );
    gui_arrow_b = new CL_Image( gc, "gui/arrow_b", resources );

    CA_APP->loading.setProgress( 50 );
}

/** Updates the frameCounter for the given surface so it shows
    fps frames per second.
    \return true if the zero frame was passed during animation. Otherwise false.
*/
/*
bool
CAResources::advanceAnimation( float* frameCounter,
                               CL_Surface* surface,
                               float fps,
                               AnimationMode mode ) {
    //return advanceAnimation( frameCounter, surface->get_num_frames(), fps, mode );
    return advanceAnimation( frameCounter, 0, fps, mode );
}
*/

/** Updates the frameCounter for the given maximal number of frames so it shows
    fps frames per second.
*/
bool
CAResources::advanceAnimation( float* frameCounter,
                               int numFrames,
                               float fps,
                               AnimationMode mode ) 
{
    bool ret = false;
    static bool fw = true;  // Forward? for revolving mode
    float amount = (fps / CA_APP->framesPerSec);

    switch( mode ) {
    case Forward:
        *frameCounter += amount;
        if( *frameCounter>=(float)numFrames ) {
            *frameCounter -= (float)numFrames;
            ret = true;
        }
        break;

    case Backward:
        *frameCounter -= amount;
        if( *frameCounter<0.0 ) {
            *frameCounter += (float)(numFrames);
            ret = true;
        }
        break;

    case Revolving:
        *frameCounter += (fw ? amount : -amount);

        if( *frameCounter>=(float)numFrames ) {
            *frameCounter -= amount;
            fw = false;
            ret = true;
        }
        if( *frameCounter<0.0 ) {
            *frameCounter += amount;
            fw = true;
            ret = true;
        }
        break;
    }

    return ret;
}
