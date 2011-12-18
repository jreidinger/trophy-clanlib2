#include <sstream>
#include <algorithm>

#include "upgradespanel.h"
#include "caimageview.h"
#include "catrophy.h"

UpgradesPanel::UpgradesPanel( Player* player, CL_Font_Sprite carFont, CL_Font_Sprite upgradeFont, const int nvLeft, const int nvTop ):
    m_player (player),
    m_carfont  (carFont),
    m_upgradesFont (upgradeFont),
    m_barHeight (m_carfont.get_font_metrics().get_height() + 6),
    m_curTop(nvTop)
{
    left = nvLeft;
    top = nvTop;
    width = m_player->getCar()->surface3d.get_width();
    right = left + width;
    m_center = (left+right)/2;
}


void UpgradesPanel::display(const bool active)
{
    std::ostringstream ossMoney;
    ossMoney << "$" << m_player->getMoney();
    CL_Texture carimage(m_player->getCar()->surface3d);
    CAImageView carView(m_player->getName(), ossMoney.str(),  carimage);
    carView.move(left, top);
    carView.display();

    // PlayerUpgrade
    //
    m_curTop += carView.getHeight() + 0.5*m_barHeight;
    displayBarOption(m_player->getCar()->getMotor());

    m_curTop += 1.5*m_barHeight;
    displayBarOption(m_player->getCar()->getTires());

    m_curTop += 1.5*m_barHeight;
    displayBarOption(m_player->getCar()->getArmor());
}


void UpgradesPanel::displayBarOption(CarOption* carOption)
{
    std::string final = carOption->getName();
    std::transform(final.begin(), final.end(), final.begin(), (int(*)(int)) std::toupper);
    m_upgradesFont.draw_text( *CA_APP->graphicContext,m_center, m_curTop, final);
    const int barTop = m_curTop + m_upgradesFont.get_font_metrics().get_height();
    CL_Colorf tempColor;
    const int barMax = carOption->getMax();
    for (int i=0; i < barMax; i++)
    {
        if (i < carOption->getCurrent())
            tempColor = CL_Colorf (255, 0, 0, 255); // red
        else
            tempColor = CL_Colorf (125, 125, 125, 255); // gray
        CL_Draw::fill( *CA_APP->graphicContext, CL_Rectf(left+i*(right-left)/barMax + 1, barTop, right - (barMax-1-i)*(right-left)/barMax -1, barTop + 0.5*m_barHeight),  
                              tempColor);
    }
}

// EOF
