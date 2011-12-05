#ifndef CAESCAPEDIALOG_H
#define CAESCAPEDIALOG_H

#include "cainfodialog.h"

class CAEscapeDialog : public CAInfoDialog {
public:
        CAEscapeDialog( const std::string& title,
                  const std::string& text,
                  const InfoType type=Info,
                  bool modal=false,
                  CAScreen* screen=0 );
    
    void on_key_released (const CL_InputEvent &key, const CL_InputState &state);

bool isMustQuit();
private:
    bool mustQuit;
    bool firstEscape;
        
};

#endif
