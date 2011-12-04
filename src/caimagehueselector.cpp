#include "caimagehueselector.h"
#include "caimagemanipulation.h"
#include "caresources.h"
#include "catrophy.h"


/** Constructor for an empty image hue selector.
    \param direction Vertical selector? (Use Up/Down arrows to control)
*/
CAImageHueSelector::CAImageHueSelector( Direction direction )
        : CAImageView(),hueImage() {
    if( CA_APP->debug ) std::cout << "CAImageHueSelector() begin" << std::endl;

    this->direction = direction;
    hue = 0;

    if( CA_APP->debug ) std::cout << "CAImageHueSelector() end" << std::endl;
}



/** Constructor for an image hue selector with an image.
    \param upperText Text above image
    \param lowerText Text under image
    \param image The image itself
    \param type Selector type (Image, Selector, HueSelector)
    \param direction Vertical selector? (Use Up/Down arrows to control)
*/
CAImageHueSelector::CAImageHueSelector( const std::string upperText,
                                        const std::string lowerText,
                                        CL_Image image,
                                        bool autoResize,
                                        Direction direction )
        : CAImageView( upperText,
                       lowerText,
                       image,
               autoResize ) {
    if( CA_APP->debug ) std::cout << "CAImageHueSelector() begin 2" << std::endl;

    this->direction = direction;
    hueImage = CAImageManipulation::changeHSV( image, 0, 0, 0 );
    hue = 0;
    setImageSize( image.get_width(), image.get_height() );

    if( CA_APP->debug ) std::cout << "CAImageHueSelector() end 2" << std::endl;
}

/** Destructor.
*/
CAImageHueSelector::~CAImageHueSelector() {
}

/** Sets the size of the image view (without top/bottom button).
    Bigger images get cropped (top left part shown).
    \param w Width of image view
    \param h Height of image view
*/
void
CAImageHueSelector::setImageSize( int w, int h ) {
    CAImageView::setImageSize( w,h );

    if( direction==Horizontal ) {
        width += 2*CA_RES.gui_arrow_l.get_width();
    } else {
        height += 2*CA_RES.gui_arrow_t.get_height();
    }

    right = left + width;
    bottom = top + height;
}

/** Changes the images hue (only for hue selectors with one image).
    The old image gets deleted.
*/
void
CAImageHueSelector::changeImageHue( bool forward ) {
    if( !image.image.is_null() ) {
        // TODO : clean the constants
        int amount = 5 * (forward ? 1 : -1);
        hue += amount;
        if( hue > 360 ) hue -= 360;
        if( hue < 0   ) hue += 360;

        hueImage = CAImageManipulation::changeHSV( image.image, hue, 0, 0 );
    }
}

/** Displays the image view with its current image/ texts.
*/
void
CAImageHueSelector::display( bool active ) 
{
    //CAImageView::display( active );

    CL_Rect crAll( 0, 0, CA_APP->width, CA_APP->height );
    CL_Rect crImage( left, top, right, bottom );
    CA_APP->graphicContext->set_cliprect( crImage );

    CL_Draw::fill( *CA_APP->graphicContext, CL_Rect(left, top, right, bottom), CL_Colorf(0, 0, 0, 64) );

    displayTextButtons( active );

    if (!hueImage.is_null())
    {
        // center the image
       const int leftPos = left + (width  - hueImage.get_width()) /2;
       const int topPos  = top  + (height - hueImage.get_height())/2;
       hueImage.draw( *CA_APP->graphicContext,leftPos, topPos);
    }

    displayArrows( active );

    CA_APP->graphicContext->set_cliprect( crAll );
}

/** Handles key events for this view / selector.
*/
void
CAImageHueSelector::handleKey (const CL_InputEvent &key) 
{
    if( (key.id==CL_KEY_LEFT && direction==Horizontal) || (key.id==CL_KEY_DOWN && direction==Vertical) || key.id==CL_KEY_PRIOR) 
    {
        changeImageHue( false );
    }

    if( (key.id==CL_KEY_RIGHT && direction==Horizontal) || (key.id==CL_KEY_UP && direction==Vertical) || key.id==CL_KEY_NEXT) 
    {
        changeImageHue( true );
    }
}

// EOF
