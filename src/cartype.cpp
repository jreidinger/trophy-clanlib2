#include "cartype.h"
#include "cacarupgrades.h"
#include "caimageview.h"
#include "player.h"


CarOption::CarOption(CL_ResourceManager resources, CACarUpgrades* carUp, int maxOption, const std::string& pathPrice, const std::string& name)
:  m_carUp(carUp), m_current(0), m_maxOpt(maxOption), m_name(name)
{
    for (int i=0; i < m_maxOpt; i++)
        m_PriceList.push_back((i*CL_Integer( pathPrice, resources ))/2+CL_Integer( pathPrice, resources ));
}


CL_Surface* CarOption::getImage() const
{
    int imageNum = m_current;
    if (m_current == m_maxOpt)
        imageNum--;
     return getSurface(imageNum);
}

int CarOption::getPrice () const
{
    int price = m_PriceList[m_current];
    if (m_current == m_maxOpt)
    {
        price = 0; // Should be impossible
    }
    return price;
}

bool CarOption::buyOption(Player* pl)
{
    bool hasBought = false;
    if (m_current < m_maxOpt)
    {
        if (pl->spendMoney(getPrice()))
        {
            m_current++;
            upgrade();   // increase the value(s) of car settings
            hasBought = true;
        }
    }
    return hasBought;
}

std::string CarOption::getPriceString() const
{
    int price = m_PriceList[m_current];
    if (m_current == m_maxOpt)
    {
        price = 0;
    }
    std::ostringstream oss;
    oss << "$" << price;
    return oss.str();
}

void CarOption::updateImageView(CAImageView* imageView, const int imageViewWidth)
{
    imageView->setAutoresize(true);
    imageView->setImage(getImage());
    imageView->setUpperText(m_name);
    imageView->setLowerText(getPriceString());
    imageView->resize(imageViewWidth, -1);
}


CarMotor::CarMotor(const std::string& mainPath, CL_ResourceManager* resources, CACarUpgrades* carUp)
: CarOption(resources, carUp, 5, mainPath + "motorPrice", "Engine")
{
    std::string path = mainPath + "acceleration";
    m_acceleration = 6 * CL_Integer( path, resources );

    path = mainPath + "maxSpeed";
    m_maxSpeed = 6 * CL_Integer( path, resources );
}


void CarMotor::upgrade()
{
     m_acceleration += 10;
     m_maxSpeed += 5;
}


CL_Surface* CarMotor::getSurface(const int imageNum) const
{
    return m_carUp->getMotor(imageNum);
}


CarTires::CarTires(const std::string& mainPath, CL_ResourceManager* resources, CACarUpgrades* carUp)
: CarOption(resources, carUp, 5, mainPath + "tiresPrice", "Tires")
{
    std::string path = mainPath + "slidingFactor";
    m_slidingFactor = 0.01 * CL_Integer( path, resources );
}

void CarTires::upgrade()
{
    m_slidingFactor += 0.05f;
}


CL_Surface* CarTires::getSurface(const int imageNum) const
{
    return m_carUp->getTires(imageNum);
}


CarArmor::CarArmor(const std::string& mainPath, CL_ResourceManager* resources, CACarUpgrades* carUp)
: CarOption(resources, carUp, 5, mainPath + "armorPrice", "Armor")
{
    m_armor = 0; //For now all cars start with zero armor, this might change in the future
}


void CarArmor::upgrade()
{
    m_armor += 10;
}


CL_Surface* CarArmor::getSurface(const int imageNum) const
{
    return m_carUp->getArmor(imageNum);
}


CarType::CarType(const std::string& mainPath, CL_ResourceManager* resources, CACarUpgrades* carUp, const bool debug)
     :
         m_motor (mainPath, resources, carUp),
         m_tires (mainPath, resources, carUp),
         m_armor (mainPath, resources, carUp)
{
    if(debug) std::cout << "  name" << std::endl;

    std::string path = mainPath + "name";
    name = CL_String::load( path, resources );

    if(debug) std::cout << "  surface" << std::endl;

    path = mainPath + "surface";
    surface = new CL_Surface( path, resources );

    if(debug) std::cout << "  surface3d" << std::endl;

    path = mainPath + "surface3d";
    surface3d = new CL_Surface( path, resources );

    path = mainPath + "length";
    length = CL_Integer( path, resources );
    path = mainPath + "width";
    width = CL_Integer( path, resources );
    path = mainPath + "minSpeed";
    minSpeed = 6 * CL_Integer( path, resources );
    path = mainPath + "maxTurbo";
    maxTurbo = CL_Integer( path, resources );
    path = mainPath + "acceleration";
    deceleration = 6 * CL_Integer( path, resources );
    path = mainPath + "steeringPower";
    steeringPower = CL_Integer( path, resources );
    path = mainPath + "price";
    price = CL_Integer( path, resources );


    radius = std::sqrt( (double)width/2 * (double)width/2 + (double)length/2 * (double)length/2 );
    angle = atan( (double)(width/2) / (double)(length/2) ) * ARAD;
}


// EOF
