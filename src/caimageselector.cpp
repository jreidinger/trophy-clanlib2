#include "caimageselector.h"
#include "caresources.h"
#include "catrophy.h"


/** Constructor for an empty image selector.
    \param direction Vertical selector? (Use Up/Down arrows to control)
*/
CAImageSelector::CAImageSelector( Direction direction )
        : CAImageView() {
    numImages=0;
    currentImage = -1.0;
    newImage = -1;
    this->direction = direction;
}



/** Destructor.
*/
CAImageSelector::~CAImageSelector() {}



/** Sets the size of the image selector (without top/bottom button).
    Bigger images get cropped (top left part shown). This method
    overwrites the method from CAImageView to add space for the
    selector arrows.
    \param w Width of image view
    \param h Height of image view
*/
void
CAImageSelector::setImageSize( int w, int h ) {
    CAImageView::setImageSize( w, h );

    if( direction==Horizontal ) {
        width += 2*CA_RES->gui_arrow_l.get_width();
    } else {
        //height += 2*CA_RES->gui_arrow_t->get_height();
    }


    right = left + width;
    bottom = top + height;
}



/** Add image to this selector.
    \param upperText Text shown above image
    \param lowerText Text shown under image
    \param image Pointer to surface to be shown
    \param autoResize Automatic fit size of the widget to image size
*/
void
CAImageSelector::addImage( const std::string upperText,
                           const std::string lowerText,
                           CL_Image image,
                           bool autoResize ) {
    this->image[numImages].upperText = upperText;
    this->image[numImages].lowerText = lowerText;
    this->image[numImages].image = image;

    if( currentImage<0.0 ) {
        currentImage = numImages;
        newImage = numImages;
        CAImageView::image.upperText = this->image[newImage].upperText;
        CAImageView::image.lowerText = this->image[newImage].lowerText;
    }

    if( numImages<CA_IV_MAXIMAGES ) numImages++;

    if( autoResize ) {
        setImageSize( image.get_width(), image.get_height() );
    }
}



/** Displays the image view with its current image/ texts.
*/
void
CAImageSelector::display( bool active ) {
    CAImageView::display( active );

    CL_Rect crAll( 0,0, CA_APP->width, CA_APP->height );
    CL_Rect crImage( left,top, right, bottom );

    CA_APP->graphicContext->set_cliprect( crImage );

    // Animate:
    //
    float diff = (float)newImage - currentImage;
    float numImages2 = (float)numImages/2.0;
    bool forward = false;

    // Move forward or back?
    //
    if( (diff <  numImages2 && diff > 0.0) ||
            (diff < -numImages2) ) {
        forward = true;
    }

    // Correct frame difference:
    //
    if( diff < -numImages2 ) diff += numImages;
    if( diff >  numImages2 ) diff -= numImages;

    if( forward ) currentImage += fabs( diff ) / 6.0;
    else          currentImage -= fabs( diff ) / 6.0;

    if( currentImage< 0.0 )             currentImage+=(float)numImages;
    if( currentImage>(float)numImages ) currentImage-=(float)numImages;

    int image1 = (int)floor( currentImage );
    int image2 = (int)ceil( currentImage );
    if( image2>=numImages ) image2=0;
    float rest = currentImage-image1;
    int center;

    if( direction==Horizontal ) {
        center = (left+right)/2;
        int image1Pos = (int)(center - rest*width - image[image1].image.get_width()/2);
        image[image1].image.draw ( *CA_APP->graphicContext,image1Pos, top+barHeight);
        image[image2].image.draw ( *CA_APP->graphicContext,image1Pos + width, top+barHeight);
    } else {
        center = (top+bottom)/2;
        int image1Pos = (int)(center - rest*height - image[image1].image.get_height()/2);
        image[image1].image.draw ( *CA_APP->graphicContext,(left+right-image[image1].image.get_width())/2, image1Pos);
        image[image2].image.draw ( *CA_APP->graphicContext,(left+right-image[image1].image.get_width())/2, image1Pos + height);
    }

    displayArrows( active );

    CA_APP->graphicContext->set_cliprect( crAll );
}



/** Handles key events for this view / selector.
*/
void
CAImageSelector::handleKey( const CL_InputEvent &key ) {
    // Selectors allow the user to change the image:
    //
    // Increase:
    //
    if( (key.id==CL_KEY_LEFT && direction==Horizontal) ||
            (key.id==CL_KEY_DOWN && direction==Vertical)      ) {
        newImage--;
        if( newImage<0 ) newImage = numImages-1;
        if( CA_APP->sound ) CA_RES->effectSelector->play();
    }

    // Decrease:
    //
    if( (key.id==CL_KEY_RIGHT && direction==Horizontal) ||
            (key.id==CL_KEY_UP    && direction==Vertical)      ) {
        newImage++;
        if( newImage>=numImages ) newImage = 0;
        if( CA_APP->sound ) CA_RES->effectSelector->play();
    }

    CAImageView::image.upperText = image[newImage].upperText;
    CAImageView::image.lowerText = image[newImage].lowerText;
}


// EOF
