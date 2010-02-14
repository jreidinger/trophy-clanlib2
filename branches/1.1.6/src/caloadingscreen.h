#ifndef CALOADINGSCREEN_H
#define CALOADINGSCREEN_H

#include "cascreen.h"

/** Screen shown on startup to inform the user about the loading status.
    @author Andrew Mustun
*/
class CALoadingScreen : public CAScreen  {
public:
    CALoadingScreen();
    ~CALoadingScreen();

    virtual int run() {
        return 0;
    }
    virtual void buildScreen();

    void begin();
    void setProgress( float p );
    void addProgress( float p );
    void end();

private:
    //! Current progress shown (0-100).

    float progress;        // Progress in percent
}
;

#endif
