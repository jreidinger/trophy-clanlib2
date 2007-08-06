#ifndef CA_GOODYTYPE
#define CA_GOODYTYPE

#include <ClanLib/core.h>
#include <ClanLib/display.h>

/** Structure for goody types.
    @author Andrew Mustun
*/
struct CAGoodyType {
    //! Goody name
    std::string   name;
    //! Pixmaps for this goody
    CL_Surface* surface;
    //! Life time in milliseconds
    int life;
};

#endif

// EOF
