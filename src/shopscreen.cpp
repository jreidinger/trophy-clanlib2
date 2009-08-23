#include "shopscreen.h"
#include "catrophy.h"
#include "player.h"
#include "caimagehueselector.h"
#include "caimageselector.h"

/** Constructor.
*/
ShopScreen::ShopScreen(Player* player, CL_Surface* background, CL_Surface* button, CL_Font* font, CACarUpgrades* carRes)
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
    m_barHeight   (m_font->get_height() + 6),
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

    
    m_imageView[0] = new CAImageView ( "Motor", "", m_carRes->getMotor(0), true );
    m_imageView[1] = new CAImageView ( "Tires", "", m_carRes->getTires(0), true );
    m_imageView[2] = new CAImageView ( "Armor", "", m_carRes->getTires(4), true );
    m_imageView[3] = new CAImageView ( "Continue", "", m_carRes->getMotor(4), true );
    for (int i = 0; i < 4; i++)
    {
        m_imageView[i]->resize(m_carImage->getWidth(), -1);
        m_imageView[i]->move(m_carImage->getLeft() + (m_carImage->getWidth()+32)*i, m_carImage->getBottom() + 32);
    }

    // Set the position of the text box on the right side of the car image
    m_guiBox.setPosition( m_carImage->getRight()+32, m_carImage->getTop(), m_imageView[3]->getRight(), m_carImage->getBottom());

    updateText();

    m_focus = m_carImage;
}

/** Destructor.
*/
ShopScreen::~ShopScreen()
{
    delete m_carImage;
    for (int i = 0; i < 4; i++)
    {
        delete m_imageView[i];
    }
}



/** Runs the screen.
*/
int
ShopScreen::run() 
{
    //CL_Input::chain_button_release.push_back( this );
    //slot = CL_Input::sig_button_release.connect(thCreateSlot(this, &ShopScreen::on_button_release));
    //slot = CL_Input::sig_button_press.connect(this, &ShopScreen::on_button_release);
    slot = CL_Keyboard::sig_key_up().connect(this, &ShopScreen::on_key_released);

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
    return (cancel);
}

/** Builds the screen.
*/
void
ShopScreen::buildScreen() 
{
    // Background:
    //
    m_background->draw ( CL_Rect(0, 0, CA_APP->width, CA_APP->height) );
    displayTitle();
    displayHelp();

    // Background
    //
    CL_Display::fill_rect( CL_Rect(left, top, right, bottom), CL_Color(0, 0, 0, 64) );

    // GuiBox:
    //
    m_guiBox.display();

    // Text:
    //
    const int topTxt = m_guiBox.getTop() + 8;
    m_font->draw ( m_guiBox.getHCenter(), topTxt, m_text );

    // Cursor:
    //
    static float cursorAnim = 0.0;    // Counter for cursor animation:
    
    // We need to display four blinking rectangles around the widget (we don't want blinking inside the blended part of the widget)
    const CL_Color blinkColor = CL_Color (255, 216, 84, (int)((cursorAnim/2)*255) );

    CL_Display::fill_rect( CL_Rect(m_focus->getLeft(), m_focus->getTop()-m_curWidth,
                    m_focus->getRight()+m_curWidth, m_focus->getTop()),
                    blinkColor);

    CL_Display::fill_rect( CL_Rect(m_focus->getLeft()-m_curWidth, m_focus->getTop()-m_curWidth,
                    m_focus->getLeft(), m_focus->getBottom()),
                    blinkColor);

    CL_Display::fill_rect( CL_Rect(m_focus->getLeft()-m_curWidth, m_focus->getBottom(),
                m_focus->getRight(), m_focus->getBottom()+m_curWidth),
                blinkColor);

    CL_Display::fill_rect( CL_Rect(m_focus->getRight(), m_focus->getTop(),
                        m_focus->getRight()+m_curWidth, m_focus->getBottom()+m_curWidth),
                        blinkColor);

    CA_RES->advanceAnimation( &cursorAnim, 1, 2.0, CAResources::Revolving );



    // Buying Cursor
    //
    if (m_isAbleToBuy)
    {
        const int fh      = m_font->get_height(); //Font Height
        const int curLeft = m_guiBox.getLeft() + CA_RES->gui_border2->get_width() + 5;
        const int curTop  = m_cursor*fh*2+topTxt+fh*2;
        CA_RES->menu_cursorani->draw (curLeft, curTop);
        CA_RES->menu_cursorani->update();
    }

    // Cars to buy
    //
    m_carImage->display();

    //
    // Upgrades
    for (int i = 0; i<4 ; i++)
    {
        m_imageView[i]->display();
    }


    const int pl_left = m_guiBox.getRight()+32;
    const int pl_top = top+32;
    CarType& pl_car = CA_APP->carType[m_player->getCarNumber()];
    const int pl_width = pl_car.surface3d->get_width();
    const int pl_right = pl_left + pl_width;
    
    // Player Name
    //
    drawTextButton(CL_Rect(pl_left, pl_top,  pl_right, pl_top + m_barHeight), m_player->getName());

    // Player Car
    //
    pl_car.surface3d->draw(pl_left, pl_top + m_barHeight);

    // Player Money
    //
    std::ostringstream ossMoney;
    ossMoney << "$" << m_player->getMoney();
    drawTextButton(CL_Rect(pl_left, pl_top + m_barHeight + pl_car.surface3d->get_height(), pl_right, pl_top + m_barHeight + pl_car.surface3d->get_height() + m_barHeight), ossMoney.str());
}

/** Called on key release.
*/
void
ShopScreen::on_key_released (const CL_InputEvent &key) 
{
    switch( key.id ) 
    {
        // Cancel (ESC):
        //
    case CL_KEY_ESCAPE:
        if (m_confirmMode == false)
        {
            // TODO: should have a save menu on Escape
            done = true;
        }
        else
        {
            m_confirmMode = false;
        }
        // No cancel in this mode
        break;

        // Activate:
        //
    case CL_KEY_ENTER:
    case CL_KEY_SPACE:
    {
        if (m_focus == m_carImage)
        {
            if (m_confirmMode == false)
                m_confirmMode = true;
            else
            {
                m_confirmMode = false;
                if ((m_cursor == 0) && m_isAbleToBuy)
                {
                    m_player->setCarNumber(m_carImage->getSelectedImage());
                    m_player->setMoney( m_player->getMoney()-CA_APP->carType[m_carImage->getSelectedImage()].price );
                    m_player->setNewCar( true );
                }
            }
            m_isAbleToBuy = false;
        }
        else if (m_focus == m_imageView[3])
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
           int focusNum = 0;
           for (int i=0; i<4; i++)
              if (m_imageView[i] == m_focus)
                 focusNum = i;

           if (focusNum == 0 && key.id == CL_KEY_LEFT)
           {
               m_focus = m_carImage;
               if( CA_APP->sound ) CA_RES->effectMenu->play( 2 );
           }
           else
           { 
               if (key.id == CL_KEY_LEFT)
               {
                   focusNum--;
                   if( CA_APP->sound ) CA_RES->effectMenu->play( 2 );
               }
               else if (key.id == CL_KEY_RIGHT && focusNum!=3)
               {
                   focusNum++;
                   if( CA_APP->sound ) CA_RES->effectMenu->play( 2 );
               }
               m_focus = m_imageView[focusNum];
            }

        }
    }
        break;
    }
    
    // change have made so we update Text
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
        oss << "TODO: Add a nice description for " << CA_APP->carType[carNum].name << " car\n";
        oss << "Max speed : " << CA_APP->carType[carNum].maxSpeed << " px/s\n";
        oss << "Max turbo : " << CA_APP->carType[carNum].maxTurbo << " px\n";
        oss << "Acceleration : " << CA_APP->carType[carNum].acceleration << " px/(s*s)\n";
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
/**
 * Simply draw a text button
 */

void ShopScreen::drawTextButton(const CL_Rect& rect, const std::string& text)
{
    const int pl_ftpadding = 6;
    const int center = (rect.left + rect.right)/2;
    m_button->draw(rect);
    m_font->draw( center, rect.top + pl_ftpadding, text);
}

// EOF