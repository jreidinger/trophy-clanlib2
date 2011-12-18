#ifndef CAIMAGESELECTOR_H
#define CAIMAGESELECTOR_H

#include <ClanLib/core.h>
#include "caimageview.h"

#define CA_IV_MAXIMAGES 128


/** Images with text above and below them. This widget is an
    interactive selector. Arrows are displayed at the top/ 
    bottom or right/ left of it to show the selecting direction.
    @author Andrew Mustun
*/
class CAImageSelector : public CAImageView  {
public:
    CAImageSelector( Direction vertical=Horizontal );
    ~CAImageSelector();

    void setImageSize( int w, int h );

    void addImage( const std::string upperText,
                   const std::string lowerText,
                   CL_Texture texture,
                   bool autoResize=true );

    void setCurrentImage( int index ) {
        currentImage = newImage = index;
        CAImageView::image.upperText = image[newImage].upperText;
        CAImageView::image.lowerText = image[newImage].lowerText;
    }
    int  getSelectedImage() {
        return newImage;
    }

    virtual void display( bool active=true );
    virtual void handleKey( const CL_InputEvent &key );

private:
    //! Pointer to surfaces which can be selected
    CAImageData image[CA_IV_MAXIMAGES];
    //! Number of surfaces
    int numImages;
    //! Index of current image (can be somewhere between images)
    float currentImage;
    //! Index of new image selected with keys
    int newImage;
};

#endif
