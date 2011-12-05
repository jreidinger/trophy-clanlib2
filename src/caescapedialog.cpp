#include "caescapedialog.h"

CAEscapeDialog::CAEscapeDialog( const std::string& title,
                               const std::string& text,
                               const InfoType type,
                               bool modal,
                               CAScreen* screen )
                               :CAInfoDialog(title, text, type, modal, screen),
mustQuit(false),
firstEscape(false)
{}
    
void CAEscapeDialog::on_key_released (const CL_InputEvent &key, const CL_InputState &event)
{
    switch( key.id ) {
        case CL_KEY_Y: 
            mustQuit = true;
            done = true;
            break;
        case CL_KEY_ESCAPE:
            if (firstEscape)
                done = true;
            firstEscape = true;
            mustQuit = false;
            break;
        case CL_KEY_ENTER: 
            mustQuit = false;
            done = true;
            break;         
        default:
            break;
    }
}

bool CAEscapeDialog::isMustQuit()
{
    return mustQuit;
}
