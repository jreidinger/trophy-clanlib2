#ifndef GUIBOX_H
#define GUIBOX_H

#include "cawidget.h"
/** A class utily for displain a white box
   @author Matthieu Lecesne
*/

class GUIBox : public CAWidget
{
public:
    GUIBox(const int n_left=0, const int n_top=0, const int n_right=0, const int n_bottom=0);
    virtual ~GUIBox();

    void setPosition(const int n_left, const int n_top, const int n_right, const int n_bottom);
    void display(bool active=true);

private:

    //! width of an edge
    const int ew;
    //! height of an edge
    const int eh;
    //! width of a border
    const int bw;
    //! height of a border
    const int bh;
};

#endif
