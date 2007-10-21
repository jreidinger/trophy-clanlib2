#include "calabel.h"

/** Constructor.
    \param text The label text
    \param align Alignment (Left, Right, Center)
    \param font Font
*/
CALabel::CALabel( std::string text, Alignment alignment, CL_Font* font )
        : CAWidget( alignment ) {
    this->text = text;
    this->font = font;
}



/** Destructor.
*/
CALabel::~CALabel() {}


/** Displays the label.
*/
void
CALabel::display( bool active ) {
    char cStr[2];
    cStr[1] = '\0';
    int pos;

    // Letter width is the width of character '|':
    //
    int letterWidth = font->get_width( "0" );
    resize( letterWidth*text.length(), font->get_height() );

    pos = left + letterWidth/2;

    for( unsigned int i = 0; i < text.length(); ++i ) 
    {
        cStr[0] = text[i];
        font->set_alignment(origin_top_center, 0, 0);
        font->draw ( pos, top, cStr );
        pos += letterWidth;
    }
}


// EOF
