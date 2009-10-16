
#include "upgradespanel.h"
#include "caimageview.h"
#include <sstream>

UpgradesPanel::UpgradesPanel( Player* player, CL_Font* carFont, CL_Font* upgradeFont, const int nvLeft, const int nvTop ):
    m_player (player),
    m_carfont  (carFont),
    m_upgradesFont (upgradeFont),
    m_barHeight (m_carfont->get_height() + 6),
    m_curTop(nvTop)
{
    m_carfont->set_alignment(origin_top_center, 0, 0);
    m_upgradesFont->set_alignment(origin_top_center, 0, 0);
    left = nvLeft;
    top = nvTop;
    width = m_player->getCar()->surface3d->get_width();
    right = left + width;
    m_center = (left+right)/2;
}


void UpgradesPanel::display(const bool active)
{
    std::ostringstream ossMoney;
    ossMoney << "$" << m_player->getMoney();
    CAImageView carView(m_player->getName(), ossMoney.str(),  m_player->getCar()->surface3d);
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
    m_upgradesFont->draw(m_center, m_curTop, final);
    const int barTop = m_curTop + m_upgradesFont->get_height();
    CL_Color tempColor;
    const int barMax = carOption->getMax();
    for (int i=0; i < barMax; i++)
    {
        if (i < carOption->getCurrent())
            tempColor = CL_Color (255, 0, 0, 255); // red
        else
            tempColor = CL_Color (125, 125, 125, 255); // gray
        CL_Display::fill_rect( CL_Rect(left+i*(right-left)/barMax + 1, barTop, right - (barMax-1-i)*(right-left)/barMax -1, barTop + 0.5*m_barHeight),  
                              tempColor);
    }
}

// EOF
