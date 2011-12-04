#ifndef CARTYPE_H
#define CARTYPE_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "utils/trophymath.h"
#include "cacarupgrades.h"
#include <string>

class CAImageView;
class Player;

/** class for car options
    @author Matthieu Lecesne
*/
class CarOption
{
    public:
        CarOption(CL_ResourceManager resources, CACarUpgrades* carRes, int maxOpt, const std::string& pathPrice, const std::string& name);
        CL_Image getImage() const;
        int getCurrent() const { return m_current;}
        void setCurrent(const int current) {m_current = current;} // Must only be use by loadGame
        int getMax() const { return m_maxOpt;}
        bool isMax() const { return m_current == m_maxOpt;}
        int getPrice () const;
        bool buyOption (Player* pl);
        std::string getPriceString() const;
        void updateImageView(CAImageView* imageView, const int imageViewWidth);
        std::string getName() const {return m_name;}
    protected:
        //! Upgrades ressources manager
        const CACarUpgrades* m_carUp;
    private:
        virtual void upgrade() = 0;
        virtual CL_Image getImage(const int ImageNum) const = 0;

        // ! current option value
        int m_current;
        // ! max option value
        int m_maxOpt;
        // ! List of the upgrades price
        std::vector<int> m_PriceList;
        // ! Name of the option (Engine, Tires, Armor)
        std::string m_name;
};

//! Motor change the acceleration and the maxSpeed 
class CarMotor : public CarOption
{
    public:
        CarMotor(const std::string& mainPath, CL_ResourceManager resources, CACarUpgrades* carUp);
        inline float getAcceleration() const { return m_acceleration;}
        inline float getMaxSpeed() const { return m_maxSpeed;}
    private:
        void upgrade();
        CL_Image getImage(const int imageNum) const;
        //! Acceleration in pixels per square second
        float m_acceleration;
        //! Maximum speed in pixels per second
        float m_maxSpeed;
};


//! Tires change the slidingFactor 
class CarTires : public CarOption
{
    public:
        CarTires(const std::string& mainPath, CL_ResourceManager resources, CACarUpgrades* carUp);
        inline float getSlidingFactor() const { return m_slidingFactor; }
    private:
        void upgrade();
        CL_Image getImage(const int imageNum) const;
        //! Sliding factor (1=no, 0.8=much, ...)
        float m_slidingFactor;
};

// ! Armor change the armor ;-)
class CarArmor: public CarOption
{
      public:
        CarArmor(const std::string& mainPath, CL_ResourceManager resources, CACarUpgrades* carUp);
         inline float getArmor() const  { return m_armor;}
    private:
        void upgrade();
        CL_Image getImage(const int imageNum) const;
        // ! Armor value
        float m_armor;
};


/** class for car types.
    @author Andrew Mustun
    @author Matthieu Lecesne
*/
class CarType 
{
	public:

        CarType(const std::string& mainPath, CL_ResourceManager resources, CACarUpgrades* carUp, const bool debug = false);

        CarMotor* getMotor() {return &m_motor;}
        CarTires* getTires() {return &m_tires;}
        CarArmor* getArmor() {return &m_armor;}

        //! Car name (e.g. "Capri")
        std::string   name;

        //! Pointer to surface resource (r,g,b,m,y,c)
        CL_Image surface;
        //! Pointer to surface resource 3d (r,g,b,m,y,c)
        CL_Image surface3d;

        //! Current car length in pixel
        int length;
        //! Current car width in pixel
        int width;
        //! Half of the diagonal
        float radius;
        //! Angle to edge
        float angle;

        //! Minimum speed in pixels per second
        float minSpeed;
        //! Maximum turbo load in pixels
        float maxTurbo;

        //! Deceleration in pixels per square second
        float deceleration;
        //! Steering power in degrees per second
        float steeringPower;

        //! Price in USD
        int   price;

    private:
        CarMotor m_motor;
        CarTires m_tires;
        CarArmor m_armor;
};


#endif

// EOF
