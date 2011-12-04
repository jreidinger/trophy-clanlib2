#include "cacarupgrades.h"
#include "caresources.h"
#include "catrophy.h"
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
        motors.push_back ( CL_Image(*CA_APP->graphicContext, oss.str(), &CA_RES->resources ));
    }
    for (int i = 0; i < 5; i++)
    {
        std::ostringstream oss;
        oss << upgradesPath << "/tires/tires" << i+1;
        tires.push_back ( CL_Image( *CA_APP->graphicContext, oss.str(), &CA_RES->resources ));
    }
    for (int i = 0; i < 5; i++)
    {
        std::ostringstream oss;
        oss << upgradesPath << "/armors/armor" << i+1;
        armors.push_back ( CL_Image( *CA_APP->graphicContext, oss.str(), &CA_RES->resources ));
    }
}

CACarUpgrades::~CACarUpgrades()
{
    motors.clear();
    tires.clear();
    armors.clear();
}


// EOF
