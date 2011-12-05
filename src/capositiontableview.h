#ifndef CAPOSITIONTABLEVIEW_H
#define CAPOSITIONTABLEVIEW_H

#include "cadialog.h"
#include "calabel.h"

/**Shows and controls the position table.
  *@author Andrew Mustun
  *@author Matthieu Lecesne
  */
class CAPositionTableView : public CADialog {
public:
    CAPositionTableView();
    ~CAPositionTableView();

    virtual void buildScreen();
    virtual void on_key_released (const CL_InputEvent &key, const CL_InputState &state);

private:
    //! Label for table output
    CALabel* label;
};

#endif
