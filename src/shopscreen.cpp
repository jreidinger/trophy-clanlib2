#include "shopscreen.h"
#include "catrophy.h"
#include "player.h"
#include "caimagehueselector.h"
#include "caimageselector.h"
#include "caresources.h" // TODO : A enlever
#include "upgradespanel.h"

#include <cctype>
#include <algorithm>

/** Constructor.
*/
ShopScreen::ShopScreen(Player* player, CL_Image background, CL_Image button, CL_Font_Sprite font, CACarUpgrades* carRes)
:   CAScreen("T H E  S H O P", "Choose a new car and press Enter to confirm"),
    m_player      (player),
    m_background  (background),
    m_button      (button),
    m_font        (font),
    m_carRes      (carRes),
    left          (0),
    right         (CA_APP->width - left),
    top           (CA_APP->headerHeight + 30),
    bottom        (CA_APP->headerHeight + 400),
    m_guiBox      (),
    m_barHeight   (m_font.get_font_metrics().get_height() + 6),
    m_confirmMode (false),
    m_cursor      (0),
    m_isAbleToBuy (false),
    m_curWidth    (12)
{
    m_carImage = new CAImageSelector();
    m_carImage->move(left + 16, top + 32);
    
    for (int carNum=0; carNum<CA_NUMCARTYPES; carNum++)
    {
        std::ostringstream ossPrice;
        ossPrice << "$" << CA_APP->carType[carNum].price;

        m_carImage->addImage(CA_APP->carType[carNum].name,
                             ossPrice.str(),
                             CA_APP->carType[carNum].surface3d,
                             true);    
    }
    if (m_player->getCarNumber() != CA_NUMCARTYPES-1)
    {
        m_carImage->setCurrentImage(m_player->getCarNumber()+1);
    }
    else
    {
        m_carImage->setCurrentImage(m_player->getCarNumber());
    }

    m_imageView[0] = new CAImageView();
    m_imageView[1] = new CAImageView();
    m_imageView[2] = new CAImageView();
    
    m_player->getCar()->getMotor()->updateImageView(m_imageView[0], m_carImage->getWidth());
    m_player->getCar()->getTires()->updateImageView(m_imageView[1], m_carImage->getWidth());
    m_player->getCar()->getArmor()->updateImageView(m_imageView[2], m_carImage->getWidth());

    m_continue = new CAImageView ( "Continue", "", CA_RES->misc_flag, true );
    m_continue->resize(m_carImage->getWidth(), -1);

    for (int i = 0; i < 3; i++)
    {
        m_imageView[i]->move(m_carImage->getLeft() + (m_carImage->getWidth()+32)*i, m_carImage->getBottom() + 32);
    }
    m_continue->move(m_carImage->getLeft() + (m_carImage->getWidth()+32)*3, m_carImage->getBottom() + 32);
    
    // Set the position of the text box on the right side of the car image
    m_guiBox.setPosition( m_carImage->getRight()+32, m_carImage->getTop(), m_continue->getRight(), m_carImage->getBottom());

    updateText();

    m_focus = m_carImage;
}

/** Destructor.
*/
ShopScreen::~ShopScreen()
{
    delete m_carImage;
    for (int i = 0; i < 3; i++)
    {
        delete m_imageView[i];
    }
    delete m_continue;
}



/** Runs the screen.
*/
int
ShopScreen::run() 
{
    slot = CA_APP->keyboard.sig_key_up().connect(this, &ShopScreen::on_key_released);

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

        CA_APP->display_window->flip();   // Copy framebufer to screen
        CL_KeepAlive::process();      // VERY VITAL for the system!

        CA_APP->measureFrameTime( false );
    }

    CA_APP->fadeScreen( false, this );
    CA_APP->waitForSilence();
    return (cancel);
}

/** Builds the screen.
*/
void
ShopScreen::buildScreen() 
{
    // Background:
    //
    m_background.draw ( *CA_APP->graphicContext, CL_Rect(0, 0, CA_APP->width, CA_APP->height) );
    displayTitle();
    displayHelp();

    // Background
    //
    CL_RoundedRect round_rect(CL_Sizef(right-left,bottom-top));
    round_rect.fill( *CA_APP->graphicContext, CL_Pointf(left, top), CL_Colorf(0, 0, 0, 64) );

    // GuiBox:
    //
    m_guiBox.display();

    // Text:
    //
    const int topTxt = m_guiBox.getTop() + 8;
    m_font.draw_text ( *CA_APP->graphicContext, m_guiBox.getHCenter(), topTxt, m_text );

    // Cursor:
    //
    static float cursorAnim = 0.0;    // Counter for cursor animation:
    
    // We need to display four blinking rectangles around the widget (we don't want blinking inside the blended part of the widget)
    const CL_Colorf blinkColor(255, 216, 84, (int)((cursorAnim/2)*255) );

    CL_RoundedRect vert_rect(CL_Sizef(m_curWidth,m_focus->getHeight()),0);
    CL_RoundedRect hor_rect(CL_Sizef(m_focus->getWidth()+2*m_curWidth,m_curWidth),0);
    vert_rect.fill( *CA_APP->graphicContext, CL_Pointf(m_focus->getLeft()-m_curWidth,m_focus->getTop()),blinkColor);
    vert_rect.fill( *CA_APP->graphicContext, CL_Pointf(m_focus->getRight(),m_focus->getTop()),blinkColor);
    hor_rect.fill( *CA_APP->graphicContext, CL_Pointf(m_focus->getLeft()-m_curWidth,m_focus->getTop()-m_curWidth),blinkColor);
    hor_rect.fill( *CA_APP->graphicContext, CL_Pointf(m_focus->getLeft()-m_curWidth,m_focus->getBottom()),blinkColor);

    CA_RES->advanceAnimation( &cursorAnim, 1, 2.0, CAResources::Revolving );



    // Buying Cursor
    //
    if (m_isAbleToBuy)
    {
        const int fh      = m_font.get_font_metrics().get_height(); //Font Height
        const int curLeft = m_guiBox.getLeft() + CA_RES->gui_border2.get_width() + 5;
        const int curTop  = m_cursor*fh*2+topTxt+fh*2;
        CA_RES->menu_cursorani.draw ( *CA_APP->graphicContext,curLeft, curTop);
        CA_RES->menu_cursorani.update();
    }

    // Cars to buy
    //
    m_carImage->display();

    //
    // Upgrades
    for (int i = 0; i<3 ; i++)
    {
        m_imageView[i]->display();
    }
    
    //
    // Continue
    m_continue->display();

    //
    // UpgradesPanel
    UpgradesPanel uPanel(m_player, m_font, CA_RES->font_lcd_13_green, m_guiBox.getRight()+32, top+32);
    uPanel.display();

}

/** Called on key release.
*/
void
ShopScreen::on_key_released (const CL_InputEvent &key, const CL_InputState&) 
{
    switch( key.id ) 
    {
        // Cancel (ESC):
        //
        case CL_KEY_ESCAPE:
        if (m_confirmMode == false)
        {
            done = true;
            cancel = true;
        }
        else
        {
            m_confirmMode = false;
            m_isAbleToBuy = false;
        }
            // No cancel in this mode
            break;

        // Activate:
            //
        case CL_KEY_ENTER:
        case CL_KEY_SPACE:
        {
            // TODO
            if (m_focus == m_carImage)
            {
                if (m_confirmMode == false)
                    m_confirmMode = true;
                else
                {
                    m_confirmMode = false;
                    if ((m_cursor == 0) && m_isAbleToBuy)
                    {
                        m_player->buyNewCar (m_carImage->getSelectedImage());
                        m_player->getCar()->getMotor()->updateImageView(m_imageView[0], m_carImage->getWidth());
                        m_player->getCar()->getTires()->updateImageView(m_imageView[1], m_carImage->getWidth());
                        m_player->getCar()->getArmor()->updateImageView(m_imageView[2], m_carImage->getWidth());
                    }
                }
                m_isAbleToBuy = false;
            }
            else if (m_focus == m_imageView[0])
            {
                if (m_player->getCar()->getMotor()->buyOption(m_player))
                {
                    m_player->getCar()->getMotor()->updateImageView(m_imageView[0], m_carImage->getWidth());
                    if( CA_APP->sound ) CA_RES->effectMenu->play( 2 );
                }
                else
                {
                    if( CA_APP->sound ) CA_RES->effectHorn->play( 2 );
                }
            }
            else if (m_focus == m_imageView[1])
            {
                if (m_player->getCar()->getTires()->buyOption(m_player))
                {
                    m_player->getCar()->getTires()->updateImageView(m_imageView[1], m_carImage->getWidth());
                    if( CA_APP->sound ) CA_RES->effectMenu->play( 2 );
                }
                else
                {
                    if( CA_APP->sound ) CA_RES->effectHorn->play( 2 );
                }
            }
            else if (m_focus == m_imageView[2])
            {
                if (m_player->getCar()->getArmor()->buyOption(m_player))
                {
                    m_player->getCar()->getArmor()->updateImageView(m_imageView[2], m_carImage->getWidth());
                    if( CA_APP->sound ) CA_RES->effectMenu->play( 2 );
                }
                else
                {
                    if( CA_APP->sound ) CA_RES->effectHorn->play( 2 );
                }
            }
            else if (m_focus == m_continue)
            {
                done = true;
            }
            else
            {
                if( CA_APP->sound ) CA_RES->effectHorn->play( 2 );
            }
        }
        break;

        case CL_KEY_UP:
        case CL_KEY_DOWN:
        {
            if (m_focus == m_carImage)
            {    
                if (m_confirmMode == true && m_isAbleToBuy)
                {
                    m_cursor = (m_cursor+1)%2;
                    if( CA_APP->sound ) CA_RES->effectMenu->play( 2 );
                }
                else if (key.id == CL_KEY_DOWN)
                {
                    m_focus = m_imageView[0];
                    if( CA_APP->sound ) CA_RES->effectMenu->play( 2 );
                }

            }
            else if (m_focus == m_imageView[0] && key.id == CL_KEY_UP)
            {
                m_focus = m_carImage;
                if( CA_APP->sound ) CA_RES->effectMenu->play( 2 );
            }
        }
        break;

        case CL_KEY_LEFT:
        case CL_KEY_RIGHT:
        {
            if (m_focus == m_carImage)
            {
                if (m_confirmMode == false || m_isAbleToBuy == false)
                {
                    m_carImage->handleKey( key );
                    m_confirmMode = false;
                }
            }
            else
            {
                if (m_focus == m_continue && key.id == CL_KEY_LEFT)
                {
                   m_focus = m_imageView[2];
                   if( CA_APP->sound ) CA_RES->effectMenu->play( 2 );
                }
                else if (m_focus != m_continue)
                {
                    if( CA_APP->sound ) CA_RES->effectMenu->play( 2 ); // we will make a change
                    
                    int focusNum = 0;
                    for (int i=0; i<3; i++)
                        if (m_imageView[i] == m_focus)
                           focusNum = i;

                    if (key.id == CL_KEY_LEFT)
                    {                        
                        focusNum--;
                    }
                    else if (key.id == CL_KEY_RIGHT)
                    {
                        focusNum++;
                    }
                    
                    if (focusNum < 0)
                    {
                        m_focus = m_carImage;
                    }
                    else if (focusNum > 2)
                    {
                        m_focus = m_continue;
                    }
                    else
                    {
                        m_focus = m_imageView[focusNum];
                    }
                }
            }
        }
        break;
    }

    // change have been made so we update Text
    updateText();
}


/** Prepare the text displayed in the textBox
 */
void ShopScreen::updateText()
{

    const int carNum = m_carImage->getSelectedImage();
    std::ostringstream oss;

    if (m_confirmMode == false)
    {

        if (m_focus == m_imageView[0])
        {
            if ( m_player->getCar()->getMotor()->isMax() == false)
            {
                oss << "TODO: Add a description for this engine (" << m_player->getCar()->getMotor()->getCurrent() + 1 << ")\n";
                oss << "You should buy this engine" << "\n";
            }
            else
            {
                oss << "Perfect is Perfect !!" << "\n";
                oss << "You already have the best engine" << "\n";
            }
        }
        else if (m_focus == m_imageView[1])
        {
            if ( m_player->getCar()->getTires()->isMax() == false)
            {
                oss << "TODO: Add a description for these tires (" << m_player->getCar()->getTires()->getCurrent() + 1 << ")\n";
                oss << "You should buy those tires" << "\n";
            }
            else
            {
                oss << "Perfect is Perfect !!" << "\n";
                oss << "You already have the best available tires" << "\n";
            }
        }
        else if (m_focus == m_imageView[2])
        {
            if (m_player->getCar()->getArmor()->isMax() == false )
            {
                oss << "TODO: Add a description for this armor (" << m_player->getCar()->getArmor()->getCurrent() + 1 << ")\n";
                oss << "You should buy this armor" << "\n";
            }
            else
            {
                oss << "Perfect is Perfect !!" << "\n";
                oss << "You already have the best available armor" << "\n";
            }

        }
        else
        {
            oss << "TODO: Add a nice description for " << CA_APP->carType[carNum].name << " car\n";
            oss << "Max speed : " << CA_APP->carType[carNum].getMotor()->getMaxSpeed() << " px/s\n";
            oss << "Max turbo : " << CA_APP->carType[carNum].maxTurbo << " px\n";
            oss << "Acceleration : " << CA_APP->carType[carNum].getMotor()->getAcceleration() << " px/(s*s)\n";
        }
    }
    else
    {
        m_isAbleToBuy = false;
        if (carNum != m_player->getCarNumber())
        {
            if (m_player->getMoney() >=  CA_APP->carType[carNum].price)
            {
                oss << "Would you like to buy a " << CA_APP->carType[carNum].name << " for " << CA_APP->carType[carNum].price << "$ ?\n";
                oss << "\n    Yes\n";
                oss << "\n    No\n";
                m_isAbleToBuy = true;
            }
            else
            {
                m_isAbleToBuy = false;
                oss << "You don't have enough money.\nYou are " << CA_APP->carType[carNum].price - m_player->getMoney() << "$ short\n";
                if( CA_APP->sound ) CA_RES->effectHorn->play( 2 );
                m_confirmMode = false;
            }
        }
        else
        {
            m_isAbleToBuy = false;
            oss << "You already have a " <<  CA_APP->carType[carNum].name;
            if( CA_APP->sound ) CA_RES->effectHorn->play( 2 );
            m_confirmMode = false;
        }
    }
    m_text = oss.str();
}


// EOF
