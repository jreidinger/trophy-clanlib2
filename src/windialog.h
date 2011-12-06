#ifndef WINDIALOG_H
#define WINDIALOG_H


#include "cacredits.h"

/** Text that is display when the player wins
    @author Matthieu Lecesne
*/
class WinDialog : public CACredits  {
public:
    WinDialog(const std::string& playerName, const int nbTurns);

private:
    std::vector<std::string> wintext;
    
};

#endif
