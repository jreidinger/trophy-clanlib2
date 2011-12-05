#include <sstream>

#include "caplayersettingsdialog.h"
#include "caimagehueselector.h"
#include "catrophy.h"

#define DEFAULT_CARTYPE 0

std::string priceToString(const int price)
{
    std::ostringstream ossPrice;
    ossPrice << "$" << price;
    return ossPrice.str();
}

/** Constructor.
*/
CAPlayerSettingsDialog::CAPlayerSettingsDialog()
        : CADialog(),
          nameInput("", 10, CAWidget::Left, CA_RES->font_normal_14_white),
          carImage(CA_APP->carType[DEFAULT_CARTYPE].name.c_str(),
                   priceToString(CA_APP->carType[DEFAULT_CARTYPE].price),
                   CA_APP->carType[DEFAULT_CARTYPE].surface3d,
                   true,
                   CAImageView::Vertical )
{
    if( CA_APP->debug ) std::cout << "CAPlayerSettingsDialog() begin" << std::endl;

    resize( 480, 200 );

    std::ostringstream ossPrice;
    ossPrice << "$" << CA_APP->carType[DEFAULT_CARTYPE].price;



    nameInput.move( left + 32, (top+bottom)/2 - nameInput.getHeight()/2 );


    carImage.move( right - carImage.getWidth() - 32,
                    (top+bottom)/2 - carImage.getHeight()/2 );

    help = "Type your Nickname and use the Arrow Keys to change your Color.";

    if( CA_APP->debug ) std::cout << "CAPlayerSettingsDialog() end" << std::endl;
}


/** Builds the dialog screen.
*/
void
CAPlayerSettingsDialog::buildScreen() {
    CADialog::buildScreen();

    CA_RES->font_normal_14_white.draw_text( *CA_APP->graphicContext, left+32, top+58, "Please enter your nickname:" );

    nameInput.display();
    carImage.display();

    displayHelp();
}

/** Called on key release.
*/
void
CAPlayerSettingsDialog::on_key_released (const CL_InputEvent &key, const CL_InputState &state) 
{
    switch( key.id ) 
    {
        case CL_KEY_ENTER:
        {
            if (nameInput.getText()=="")
            {
                if( CA_APP->sound ) CA_RES->effectHorn->play( 2 );
            }
            else
                done = true;
            break;
        }

        case CL_KEY_ESCAPE:
            done = true;
            cancel = true;
            break;

        case CL_KEY_UP:
        case CL_KEY_DOWN:
            carImage.handleKey( key );
            break;

        default:
            nameInput.handleKey( key );
            break;
    }

}

/** Returns the player's name the user has entered.
*/
const std::string
CAPlayerSettingsDialog::getPlayerName() {
    return nameInput.getText();
}

/** Returns the player's color the user has chosen.
*/
int
CAPlayerSettingsDialog::getPlayerHue() {
    return carImage.getHue();
}

// EOF
