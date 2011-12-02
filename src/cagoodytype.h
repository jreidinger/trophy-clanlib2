#ifndef CA_GOODYTYPE
#define CA_GOODYTYPE

#include <ClanLib/core.h>
#include <ClanLib/display.h>

class Player;
class CASoundEffect;

/** Structure for goody types.
    @author Andrew Mustun
    @author Matthieu Lecesne
*/
class CAGoodyType
{
public:
    CAGoodyType (CL_Image surface, int life, CASoundEffect* soundEffect);
    void catchGoodie(Player* pl);
    virtual void action(Player* pl) = 0;
    void playSound() const;
    void draw(int x, int y) const;
    int getLife() const { return m_life;}

private:
    //! Pixmaps for this goody
    CL_Image m_surface;
    //! Life time in milliseconds
    int m_life;
    //! The sound effect played when the layer catch the goody
    CASoundEffect* m_soundEffect;
};

enum TypeGoodie
{
    Turbo = 1,
    Life,
    Bullets,
    FogBombs,
    Money
};


template<TypeGoodie N>
class CAGoodyTypeDerived : public CAGoodyType
{
    public:
    CAGoodyTypeDerived(CL_Image surface, int life, CASoundEffect* soundEffect);
    void action(Player* pl);
};


#endif

// EOF
