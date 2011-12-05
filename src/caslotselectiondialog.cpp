#include "caslotselectiondialog.h"
#include "catrophy.h"
#include "upgradespanel.h"
#include "humanplayer.h"
#include <fstream>



/** Constructor.
    \param title Title of the dialog (Usually "Save..." or "Load...")
*/
CASlotSelectionDialog::CASlotSelectionDialog( const std::string title, CL_Font_Sprite font, const std::string& homedir )
        : CADialog(),
          m_font(font),
          m_homedir(homedir)
{
    cursor = 0;
    this->title = title;
    help = "Use Arrow Keys to change selection and press Enter to\nconfirm";
    left = (CA_APP->width - 320)/2;
    right = CA_APP->width - left;
    top = CA_APP->headerHeight+20;
    bottom = top + 390;
    // Set the position of the text box on the right side of the car image
    m_guiBox.setPosition( left, top, right, bottom);
    // check for existing file
    for (int i=0; i<10; ++i)
    {
        std::ostringstream oss;
        oss << m_homedir << "trophySave" << i << ".sav";
        if( CA_APP->debug ) std::cout << "File Saved to " << oss.str() << std::endl;
        std::ifstream loadFile(oss.str().c_str());
        if (loadFile)
        {
            std::string name;
            // Version, difficulty, nbTurns, currentTrack1, currentTrack2, currentTrack3, name
            for (int i =0; i<7; ++i)
                std::getline( loadFile, name ); // name is on the seventh line
            
            // TODO: This code should probably be in player.cpp
            int carNumber, currentMotor, currentTires, currentArmor, h, s, v, money, totalPoints;
            loadFile >> carNumber >> currentMotor >> currentTires >> currentArmor >> h >> s >> v >> money >> totalPoints;
            HumanPlayer* player = new HumanPlayer(0, name,carNumber);
            player->setName(name);
            player->setCarNumber(carNumber);
            player->getCar()->getMotor()->setCurrent(currentMotor);
            player->getCar()->getTires()->setCurrent(currentTires);
            player->getCar()->getArmor()->setCurrent(currentArmor);
            HSVColor color(h, s, v);
            player->setColor(color);
            player->setMoney(money);
            player->setTotalPoints(totalPoints);

            m_slotPlayers.push_back(player);
            m_slotNames.push_back(name);
        }
        else
        {
            m_slotPlayers.push_back(NULL);
            m_slotNames.push_back("Free Slot");
        }
    }
}



/** Destructor.
*/
CASlotSelectionDialog::~CASlotSelectionDialog()
{
    for (unsigned int i=0; i<m_slotPlayers.size(); ++i)
        delete m_slotPlayers[i];
}



/** Builds the screen.
*/
void
CASlotSelectionDialog::buildScreen() {
    // Counter for cursor animation:
    //static float cursorAnim = 0.0;

    // Backgroud:
    //
    CA_RES->menu_bg.draw ( *CA_APP->graphicContext, CL_Rect(0, 0, CA_APP->width, CA_APP->height) ); // TODO don't use CA_APP

    // Title / help:
    //
    displayTitle();
    displayHelp();

    // GuiBox:
    //
    m_guiBox.display();
    
    // Text:
    //
    std::string slotsText;
    for (unsigned int i=0; i<m_slotNames.size(); ++i)
    {
        slotsText += m_slotNames[i] + "\n\n";
    }
    const int topTxt = m_guiBox.getTop() + CA_RES->gui_border1.get_height() + 10;
    m_font.draw_text ( *CA_APP->graphicContext, m_guiBox.getHCenter(), topTxt, slotsText);

    // Cursor
    //
    const int fh      = m_font.get_font_metrics().get_height(); //Font Height
    const int curLeft = m_guiBox.getLeft() + CA_RES->gui_border2.get_width() + 5;
    const int curTop  = cursor*fh*2+topTxt;
    CA_RES->menu_cursorani.draw ( *CA_APP->graphicContext,curLeft, curTop);
    CA_RES->menu_cursorani.update();
    

    // Cursor:
    //
    /*CL_Display::fill_rect( racePreview[cursor]->getLeft()-12, racePreview[cursor]->getTop()-28,
                           racePreview[cursor]->getRight()+12, racePreview[cursor]->getBottom()+12,
                           1.0,0.85,0.33, (cursorAnim/2) );
    */

    //CA_RES->advanceAnimation( &cursorAnim, 1, 2.0, CAResources::Revolving );
    
    //
    // UpgradesPanel
    // TODO: Should use something better to know if it's save or load
    if (title[0] == 'S')
    {
        UpgradesPanel uPanelCurrent(CA_APP->player[0], m_font, CA_RES->font_lcd_13_green, m_guiBox.getLeft()-CA_APP->player[0]->getCar()->surface3d.get_width()-32, top+32);
        uPanelCurrent.display();
    }

    if (m_slotPlayers[cursor])
    {
        UpgradesPanel uPanel(m_slotPlayers[cursor], m_font, CA_RES->font_lcd_13_green, m_guiBox.getRight()+32, top+32);
        uPanel.display();
    }
        
}

/** Runs the dialog and return file string
*/
std::string
CASlotSelectionDialog::display()
{
    int isDialogOK = CADialog::run();
    if (isDialogOK)
    {
        std::ostringstream oss;
        oss << "trophySave" << cursor << ".sav"; 
        return oss.str();
    }
    else
        return "";     
}

/** Called on key release.
*/
void
CASlotSelectionDialog::on_key_released (const CL_InputEvent &key, const CL_InputState &state) 
{
    switch( key.id ) {

        // Cancel (ESC):
        //
    case CL_KEY_ESCAPE:
        cancel = true;
        done = true;
        break;

        // Left:
        //
    case CL_KEY_UP:
        if( cursor>0 ) {
            cursor--;
            if( CA_APP->sound ) CA_RES->effectMenu->play( 2 );
        }
        break;

        // Right:
        //
    case CL_KEY_DOWN:
        if( cursor<CA_NUMSLOTS-1 ) {
            cursor++;
            if( CA_APP->sound ) CA_RES->effectMenu->play( 2 );
        }
        break;

        // Activate:
        //
    case CL_KEY_ENTER:
    case CL_KEY_SPACE:
        {
            if (m_slotPlayers[cursor] || title[0] == 'S')
                done = true;
            else
                if( CA_APP->sound ) CA_RES->effectHorn->play( 2 );
        }
        break;

    default:
        break;
    }
}


// EOF
