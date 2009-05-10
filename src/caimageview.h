#ifndef CAIMAGEVIEW_H
#define CAIMAGEVIEW_H

#include <ClanLib/core.h>
#include "cawidget.h"


/** Struct for one image and its texts in a CAImageView.
    @author Andrew Mustun
*/
struct CAImageData {
    /** Constructor.
    */
    CAImageData() {
        image=0;
    }

    //! Upper text
    std::string upperText;
    //! Lower text
    std::string lowerText;
    //! Pointer to image surface
    CL_Surface* image;
};



/** An Image with text above and below it.
    This class is also used as basis class for image selectors.
    @author Andrew Mustun
*/
class CAImageView : public CAWidget  {
public:
    enum Direction { Vertical, Horizontal };

    CAImageView();
    CAImageView( const std::string upperText,
                 const std::string lowerText,
                 CL_Surface* image,
                 bool autoResize=true );
    ~CAImageView();

    virtual void setImageSize( int w, int h );

    virtual void display( bool active=true );
    virtual void displayArrows( bool active=true );
    virtual void displayTextButtons( bool active=true );
    virtual void handleKey( const CL_InputEvent &key );

protected:
    //! Image data (surface, texts)
    CAImageData image;
    //! 'Vertical' or 'Horizontal' selector (used for child classes)
    Direction direction;
    //! Height of top/ bottom bar
    int barHeight;
};

#endif
