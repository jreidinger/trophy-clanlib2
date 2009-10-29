#include "cacarupgrades.h"
#include "caresources.h"
#include <sstream>

/** Constructor
*/
CACarUpgrades::CACarUpgrades()
{
    const std::string upgradesPath = "upgrades";
    for (int i = 0; i < 5; i++)
    {
        std::ostringstream oss;
        oss << upgradesPath << "/motors/motor" << i+1;
        motors.push_back ( new CL_Surface( oss.str(), CA_RES->resources ));
    }
    for (int i = 0; i < 5; i++)
    {
        std::ostringstream oss;
        oss << upgradesPath << "/tires/tires" << i+1;
        tires.push_back ( new CL_Surface( oss.str(), CA_RES->resources ));
    }
    for (int i = 0; i < 5; i++)
    {
        std::ostringstream oss;
        oss << upgradesPath << "/armors/armor" << i+1;
        armors.push_back ( new CL_Surface( oss.str(), CA_RES->resources ));
    }
}

CACarUpgrades::~CACarUpgrades()
{
    for (int i = 0; i < 5; i++)
    {
        if (motors[i] != 0)
        {
            delete motors[i];
            motors[i] = 0;
        }
    }
    motors.clear();
    for (int i = 0; i < 5; i++)
    {
        if (tires[i] != 0)
        {
            delete tires[i];
            tires[i] = 0;
        }
    }
    tires.clear();
    for (int i = 0; i < 5; i++)
    {
        if (armors[i] != 0)
        {
            delete armors[i];
            armors[i] = 0;
        }
    }
    armors.clear();
}


// EOF
