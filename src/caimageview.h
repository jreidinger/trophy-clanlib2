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
    CAImageData():image() {}

    //! Upper text
    std::string upperText;
    //! Lower text
    std::string lowerText;
    //! Pointer to image surface
    CL_Image image;
};



/** An Image with text above and below it.
    This class is also used as basis class for image selectors.
    @author Andrew Mustun
    @author Matthieu Lecesne
*/
class CAImageView : public CAWidget  {
public:
    enum Direction { Vertical, Horizontal };

    CAImageView();
    CAImageView( const std::string& upperText,
                 const std::string& lowerText,
                 CL_Image image,
                 bool autoResize=true );
    ~CAImageView();

    virtual void setImageSize( int w, int h );
    virtual void setImage(CL_Image image);
    virtual void setUpperText(const std::string& upperText) {image.upperText = upperText;}
    virtual void setLowerText(const std::string& lowerText) {image.lowerText = lowerText;}
    virtual void setAutoresize(const bool autoresize) {m_autoresize = autoresize;}

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
    //! must autoresize
    bool m_autoresize;
};

#endif
