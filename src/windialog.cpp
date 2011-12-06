#include "windialog.h"
#include "catrophy.h"
#include <sstream>


/** Constructor.
*/
WinDialog::WinDialog(const std::string& playerName, const int nbTurns)
        : CACredits() {
    wintext.push_back("C O N G R A T U L A T I O N S");
    wintext.push_back("");
    wintext.push_back("You are the new 'trophy' champion");
    wintext.push_back("");
    std::ostringstream oss;
    oss << playerName << " champion in " << nbTurns << " turns";
    wintext.push_back(oss.str());
    wintext.push_back("");
    wintext.push_back("");
    wintext.push_back("================== TROPHY ==================");
    wintext.push_back("Thanks for playing !!!");
    wintext.push_back("");
    wintext.push_back("");
    wintext.push_back("");
    wintext.push_back("");     
    wintext.push_back("");
    wintext.push_back("");     
    setText(wintext);
}