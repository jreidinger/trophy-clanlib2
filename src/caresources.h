#ifndef CARESOURCES_H
#define CARESOURCES_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/sound.h>

#include "casoundeffect.h"
#include "utils/trophymath.h"

#define CA_RES CAResources::getResources()


/** Offers global access to all resources.
    This way we avoid double loading of resources.
    @author Andrew Mustun
    @author Colin Pitrat
    @author Matthieu Lecesne
*/
class CAResources {

protected:
    CAResources();
    void load();

private:
    static CAResources theOneAndOnlyResource;

public:
    enum AnimationMode { Forward, Backward, Revolving };

    static CAResources getResources();
    ~CAResources();

    /*
    static bool advanceAnimation( float* frameCounter,
                                  CL_Image* surface,
                                  float fps,
                                  AnimationMode mode = Forward );
                                  */
    static bool advanceAnimation( float* frameCounter,
                                  int numFrames,
                                  float fps,
                                  AnimationMode mode = Forward );

public:
    CL_ResourceManager resources;

    CL_Image gui_loading;
    CL_Image gui_progressbar;

    CL_Font font_normal_11_white;
    CL_Font font_normal_14_white;
    CL_Font font_normal_22_white;
    CL_Font font_normal_22_gray;
    CL_Font font_lcd_13_green;

    CL_Image misc_cross;
    CL_Image misc_flag;
    CL_Sprite misc_light;
    CL_Sprite misc_hitpoint;
    CL_Sprite misc_gunfire;
    CL_Sprite misc_carfire;
    CL_Sprite misc_checkflag;
    CL_Sprite misc_fog;
    CL_Sprite misc_dust;
    CL_Image misc_info;
    CL_Image misc_caution;

    CL_Image goody_turbo;
    CL_Image goody_life;
    CL_Image goody_money;
    CL_Image goody_bullets;
    CL_Image goody_fogbomb;

    //! Rendered sprites for the players fog. Shared by all players.
    CL_Image dust[CA_FPR];

    CL_Image menu_bg;
    CL_Sprite menu_cursorani;
    CL_Image menu_bar;

    CL_Image panel_label;
    CL_Image panel_button;
    CL_Image panel_life;
    CL_Image panel_infoview;
    CL_Sprite panel_speed;
    CL_Sprite panel_turbo;
    CL_Sprite panel_ammo;
    CL_Image panel_turbolabel;
    CL_Image panel_death;

    CL_Image gui_edge1;
    CL_Image gui_edge2;
    CL_Image gui_edge3;
    CL_Image gui_edge4;
    CL_Image gui_border1;
    CL_Image gui_border2;
    CL_Image gui_border3;
    CL_Image gui_border4;
    CL_Image gui_button;
    CL_Image gui_button_red;
    CL_Image gui_button_green;
    CL_Image gui_button_blue;
    CL_Image gui_arrow_l;
    CL_Image gui_arrow_r;
    CL_Image gui_arrow_t;
    CL_Image gui_arrow_b;

    CL_SoundBuffer sound_horn;
    CL_SoundBuffer sound_menu;
    CL_SoundBuffer sound_selector;
    CL_SoundBuffer sound_slide;
    CL_SoundBuffer sound_crash;
    CL_SoundBuffer sound_engine;
    CL_SoundBuffer sound_light;
    CL_SoundBuffer sound_goody_money;
    CL_SoundBuffer sound_goody_bullets;
    CL_SoundBuffer sound_goody_turbo;
    CL_SoundBuffer sound_goody_fogbomb;
    CL_SoundBuffer sound_goody_life;
    CL_SoundBuffer sound_shoot;
    CL_SoundBuffer sound_fogbomb;
    CL_SoundBuffer sound_fire;
    CL_SoundBuffer sound_background01;

    CASoundEffect effectCrash;
    CASoundEffect effectSlide;
    CASoundEffect effectEngine;
    CASoundEffect effectHorn;
    CASoundEffect effectLight;
    CASoundEffect effectMenu;
    CASoundEffect effectSelector;
    CASoundEffect effectGoodyMoney;
    CASoundEffect effectGoodyBullets;
    CASoundEffect effectGoodyTurbo;
    CASoundEffect effectGoodyFogbomb;
    CASoundEffect effectGoodyLife;
    CASoundEffect effectShoot;
    CASoundEffect effectFogBomb;
    CASoundEffect effectFire;
    CASoundEffect effectBackground01;

};

#endif
