#include "caimageview.h"
#include "caresources.h"
#include "catrophy.h"

/** Constructor for an empty image view.
*/
CAImageView::CAImageView()
        : CAWidget( CAWidget::Left ) {
    if( CA_APP->debug ) std::cout << "CAImageView() begin" << std::endl;

    barHeight = CA_RES->font_normal_14_white->get_height() + 6;
    image.image = 0;

    if( CA_APP->debug ) std::cout << "CAImageView() end" << std::endl;
}

/** Constructor for an image view with already one image.
    \param upperText Text above image
    \param lowerText Text under image
    \param image The image itself
    \param autoresize Automatically resize the image preview 
                      to fit the image.
*/
CAImageView::CAImageView( const char* upperText,
                          const char* lowerText,
                          CL_Surface* image,
                          bool autoResize )
        : CAWidget( CAWidget::Left ) {
    if( CA_APP->debug ) std::cout << "CAImageView() begin 2" << std::endl;

    barHeight = CA_RES->font_normal_14_white->get_height() + 6;
    this->image.upperText = upperText;
    this->image.lowerText = lowerText;
    this->image.image = image;

    if( autoResize ) {
        setImageSize( image->get_width(), image->get_height() );
    }

    if( CA_APP->debug ) std::cout << "CAImageView() end 2" << std::endl;
}

/** Destructor.
*/
CAImageView::~CAImageView() {}

/** Sets the size of the image view (without top/bottom button).
    Bigger images get cropped (top left part shown).
    \param w Width of image view
    \param h Height of image view
*/
void
CAImageView::setImageSize( int w, int h ) {
    width = w;
    height = h + barHeight * 2;
    right = left + width;
    bottom = top + height;
}

/** Displays the image view with its current image/ texts.
*/
void
CAImageView::display( bool active ) {
    CL_Rect crAll( 0,0, CA_APP->width, CA_APP->height );
    CL_Rect crImage( left, top, right, bottom );
    CL_Display::set_cliprect( crImage );

    CL_Display::fill_rect( CL_Rect(left, top, right, bottom), CL_Color(0, 0, 0, 64) );

    if( image.image ) image.image->draw( left, top+barHeight );

    displayTextButtons( active );

    CL_Display::set_cliprect( crAll );
}

/** Displays the arrows for selectors (used for base classes).
*/
void
CAImageView::displayArrows( bool active ) 
{
    // Selector arrows show the direction of possible movements:
    //
    if( direction==Horizontal ) {
        int arrowHeight = CA_RES->gui_arrow_l->get_height();
        int arrowWidth = CA_RES->gui_arrow_l->get_width();
        CA_RES->gui_arrow_l->draw (left, top+height/2-arrowHeight/2);
        CA_RES->gui_arrow_r->draw (right-arrowWidth, top+height/2-arrowHeight/2);
    } else {
        int arrowHeight = CA_RES->gui_arrow_t->get_height();
        int arrowWidth = CA_RES->gui_arrow_t->get_width();
        CA_RES->gui_arrow_t->draw ((left+right)/2-arrowWidth/2, top+barHeight);
        CA_RES->gui_arrow_b->draw ((left+right)/2-arrowWidth/2, bottom-barHeight-arrowHeight);
    }
}

/** Displays the buttons on top/ button with their texts.
*/
void
CAImageView::displayTextButtons( bool active ) 
{
    // Buttons:
    //
    CA_RES->gui_button->draw ( CL_Rect(left, top, left+width, top+barHeight) );
    CA_RES->gui_button->draw ( CL_Rect(left, top+height-barHeight, left+width, top+height-barHeight+barHeight) );

    // Texts:
    //
    int textPosX = left + width/2;
    int textPosY = top + 6;
    CA_RES->font_normal_14_white->set_alignment(origin_top_center, 0, 0);
    CA_RES->font_normal_14_white->draw( textPosX, textPosY, image.upperText );
    textPosY = top + height - barHeight + 6;
    CA_RES->font_normal_14_white->draw( textPosX, textPosY, image.lowerText );
}

/** Handles key events for this view (no action).
*/
void
CAImageView::handleKey( const CL_InputEvent &key ) {}

// EOF
