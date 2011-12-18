#ifndef CACARUPGRADES_H
#define CACARUPGRADES_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <vector>




/** Handler for upgrades ressources
    @author Matthieu Lecesne
*/
class CACarUpgrades
{
public:
    CACarUpgrades();
    ~CACarUpgrades();
    CL_Texture getMotor(const int num) const { return motors[num];}
    CL_Texture getTires(const int num) const { return tires[num];}
    CL_Texture getArmor(const int num) const { return armors[num];}
private:
    std::vector<CL_Texture> motors;
    std::vector<CL_Texture> tires;
    std::vector<CL_Texture> armors;

};

#endif
