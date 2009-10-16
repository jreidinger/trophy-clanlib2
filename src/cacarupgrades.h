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
    CL_Surface* getMotor(const int num) const { return motors[num];}
    CL_Surface* getTires(const int num) const { return tires[num];}
private:
    std::vector<CL_Surface*> motors;
    std::vector<CL_Surface*> tires;

};

#endif
