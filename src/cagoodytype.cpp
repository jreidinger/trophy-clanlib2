#include <iostream>

#include "cagoodytype.h"
#include "casoundeffect.h"
#include "player.h"
#include "catrophy.h"

CAGoodyType::CAGoodyType (CL_Image surface, int life, CASoundEffect* soundEffect):
    m_surface(surface),
    m_life(life),
    m_soundEffect(soundEffect)
{}
 
/** Play the sound associated with the goody */
void CAGoodyType::playSound() const
{
    m_soundEffect->play();
}

/** Play the sound associated with the goody */
void CAGoodyType::draw(int x, int y) const
{
  if (m_surface.is_null())
    std::cerr << "Attempt to draw invalid image";
  else
    m_surface.draw( *CA_APP->graphicContext, x-8, y-8 );
}

void CAGoodyType::catchGoodie(Player* pl)
{
    if( !pl->isDeath() && !pl->hasFinished() && !pl->isLapped())
    {
        action(pl);
        if( pl->getId()==0) playSound(); // TODO : Only sound of first player is played
    }
}




template<TypeGoodie N>
CAGoodyTypeDerived<N>::CAGoodyTypeDerived (CL_Image surface, int life, CASoundEffect* soundEffect)
: CAGoodyType(surface, life, soundEffect)
{}

template<>
void CAGoodyTypeDerived<Turbo>::action(Player* pl) 
{
    pl->setTurbo (pl->getTurbo() + 1000);
}

template<>
void CAGoodyTypeDerived<Life>::action(Player* pl)
{
    pl->setLife( pl->getLife()+25.0 );
}

template<>
void CAGoodyTypeDerived<Bullets>::action(Player* pl)
{
    pl->setBullets( pl->getBullets()+100 );
}

template<>
void CAGoodyTypeDerived<FogBombs>::action(Player* pl)
{
    pl->setFogBombs( pl->getFogBombs()+1 );
}

template<>
void CAGoodyTypeDerived<Money>::action(Player* pl)
{
    pl->addRaceMoney(100);
}

template class CAGoodyTypeDerived<Turbo>;
template class CAGoodyTypeDerived<Life>;
template class CAGoodyTypeDerived<Bullets>;
template class CAGoodyTypeDerived<FogBombs>;
template class CAGoodyTypeDerived<Money>;


